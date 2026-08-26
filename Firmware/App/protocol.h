#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "app_types.h"

#define PROTOCOL_SOF_1 0xAAu
#define PROTOCOL_SOF_2 0x55u
#define PROTOCOL_MAX_PAYLOAD 16u

typedef enum {
    CMD_START = 0x01,
    CMD_STOP = 0x02,
    CMD_QUERY_STATUS = 0x03,
    CMD_SET_RATE = 0x04,
    CMD_SET_MODE = 0x05,
    CMD_REPLAY = 0x06
} ProtocolCommand;

typedef enum {
    PROTOCOL_OK = 0,
    PROTOCOL_ERR_LENGTH = 1,
    PROTOCOL_ERR_RANGE = 2,
    PROTOCOL_ERR_COMMAND = 3
} ProtocolError;

typedef struct {
    uint8_t command;
    uint8_t length;
    uint8_t payload[PROTOCOL_MAX_PAYLOAD];
} ProtocolFrame;

typedef enum {
    PARSER_WAIT_SOF_1 = 0,
    PARSER_WAIT_SOF_2,
    PARSER_WAIT_LENGTH,
    PARSER_WAIT_COMMAND,
    PARSER_WAIT_PAYLOAD,
    PARSER_WAIT_CRC
} ParserState;

typedef struct {
    ParserState state;
    ProtocolFrame frame;
    uint8_t index;
} ProtocolParser;

void protocol_parser_init(ProtocolParser *parser);
bool protocol_parser_feed(ProtocolParser *parser, uint8_t byte,
                          ProtocolFrame *completed);
size_t protocol_encode(const ProtocolFrame *frame, uint8_t *output,
                       size_t capacity);
uint8_t protocol_crc8(const uint8_t *data, size_t length);

#endif
