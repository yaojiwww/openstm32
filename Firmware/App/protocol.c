#include "protocol.h"

#include <string.h>

uint8_t protocol_crc8(const uint8_t *data, size_t length) {
    uint8_t crc = 0;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x80u) ? (uint8_t)((crc << 1u) ^ 0x07u)
                                : (uint8_t)(crc << 1u);
        }
    }
    return crc;
}

void protocol_parser_init(ProtocolParser *parser) {
    memset(parser, 0, sizeof(*parser));
    parser->state = PARSER_WAIT_SOF_1;
}

static void parser_restart(ProtocolParser *parser, uint8_t byte) {
    parser->index = 0;
    parser->state = (byte == PROTOCOL_SOF_1) ? PARSER_WAIT_SOF_2
                                             : PARSER_WAIT_SOF_1;
}

bool protocol_parser_feed(ProtocolParser *parser, uint8_t byte,
                          ProtocolFrame *completed) {
    switch (parser->state) {
    case PARSER_WAIT_SOF_1:
        if (byte == PROTOCOL_SOF_1) parser->state = PARSER_WAIT_SOF_2;
        break;
    case PARSER_WAIT_SOF_2:
        if (byte == PROTOCOL_SOF_2) parser->state = PARSER_WAIT_LENGTH;
        else parser_restart(parser, byte);
        break;
    case PARSER_WAIT_LENGTH:
        if (byte > PROTOCOL_MAX_PAYLOAD) parser_restart(parser, byte);
        else {
            parser->frame.length = byte;
            parser->state = PARSER_WAIT_COMMAND;
        }
        break;
    case PARSER_WAIT_COMMAND:
        parser->frame.command = byte;
        parser->index = 0;
        parser->state = parser->frame.length == 0 ? PARSER_WAIT_CRC
                                                  : PARSER_WAIT_PAYLOAD;
        break;
    case PARSER_WAIT_PAYLOAD:
        parser->frame.payload[parser->index++] = byte;
        if (parser->index == parser->frame.length) parser->state = PARSER_WAIT_CRC;
        break;
    case PARSER_WAIT_CRC: {
        uint8_t body[PROTOCOL_MAX_PAYLOAD + 2];
        body[0] = parser->frame.length;
        body[1] = parser->frame.command;
        memcpy(&body[2], parser->frame.payload, parser->frame.length);
        bool valid = byte == protocol_crc8(body, (size_t)parser->frame.length + 2u);
        if (valid && completed != NULL) *completed = parser->frame;
        parser_restart(parser, byte);
        return valid;
    }
    default:
        protocol_parser_init(parser);
        break;
    }
    return false;
}

size_t protocol_encode(const ProtocolFrame *frame, uint8_t *output,
                       size_t capacity) {
    const size_t total = (size_t)frame->length + 5u;
    if (frame->length > PROTOCOL_MAX_PAYLOAD || output == NULL ||
        capacity < total) return 0;
    output[0] = PROTOCOL_SOF_1;
    output[1] = PROTOCOL_SOF_2;
    output[2] = frame->length;
    output[3] = frame->command;
    memcpy(&output[4], frame->payload, frame->length);
    output[total - 1u] = protocol_crc8(&output[2], (size_t)frame->length + 2u);
    return total;
}
