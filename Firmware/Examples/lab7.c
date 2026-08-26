#include "board_port.h"
#include "protocol.h"
#include "workstation.h"

static ProtocolParser parser;
static Workstation *target;

void lab7_init(Workstation *station) {
    target = station;
    protocol_parser_init(&parser);
}

static void send_response(uint8_t command, ProtocolError error,
                          const uint8_t *extra, uint8_t extra_length) {
    ProtocolFrame response = {
        .command = (uint8_t)(command | 0x80u),
        .length = (uint8_t)(extra_length + 1u),
        .payload = {(uint8_t)error},
    };
    for (uint8_t i = 0; i < extra_length; ++i) response.payload[i + 1u] = extra[i];
    uint8_t wire[PROTOCOL_MAX_PAYLOAD + 5u];
    size_t length = protocol_encode(&response, wire, sizeof(wire));
    if (length > 0) (void)board_uart_send(wire, length);
}

static void execute(const ProtocolFrame *frame) {
    ProtocolError error = PROTOCOL_OK;
    uint8_t status[6];
    uint8_t status_length = 0;
    switch (frame->command) {
    case CMD_START:
        if (frame->length != 0) error = PROTOCOL_ERR_LENGTH;
        else target->running = true;
        break;
    case CMD_STOP:
        if (frame->length != 0) error = PROTOCOL_ERR_LENGTH;
        else workstation_dispatch(target, EVENT_STOP);
        break;
    case CMD_SET_RATE:
        if (frame->length == 4) {
            uint32_t rate = (uint32_t)frame->payload[0] |
                            ((uint32_t)frame->payload[1] << 8u) |
                            ((uint32_t)frame->payload[2] << 16u) |
                            ((uint32_t)frame->payload[3] << 24u);
            if (rate >= 1 && rate <= 10000) target->config.sample_rate_hz = rate;
            else error = PROTOCOL_ERR_RANGE;
        } else error = PROTOCOL_ERR_LENGTH;
        break;
    case CMD_SET_MODE:
        if (frame->length == 1 && frame->payload[0] < MODE_COUNT) {
            target->mode = (WorkMode)frame->payload[0];
            target->entry_pending = true;
        } else error = frame->length == 1 ? PROTOCOL_ERR_RANGE
                                          : PROTOCOL_ERR_LENGTH;
        break;
    case CMD_QUERY_STATUS:
        if (frame->length != 0) {
            error = PROTOCOL_ERR_LENGTH;
            break;
        }
        status[0] = (uint8_t)target->mode;
        status[1] = target->running ? 1u : 0u;
        status[2] = (uint8_t)target->config.sample_rate_hz;
        status[3] = (uint8_t)(target->config.sample_rate_hz >> 8u);
        status[4] = (uint8_t)(target->config.sample_rate_hz >> 16u);
        status[5] = (uint8_t)(target->config.sample_rate_hz >> 24u);
        status_length = sizeof(status);
        break;
    case CMD_REPLAY:
        if (frame->length != 0) error = PROTOCOL_ERR_LENGTH;
        else {
            target->mode = MODE_REPLAY;
            target->entry_pending = true;
        }
        break;
    default:
        error = PROTOCOL_ERR_COMMAND;
        break;
    }
    send_response(frame->command, error, status, status_length);
}

/* Feed bytes received by UART interrupt or DMA one by one. */
void lab7_on_uart_byte(uint8_t byte) {
    ProtocolFrame frame;
    if (protocol_parser_feed(&parser, byte, &frame)) execute(&frame);
}
