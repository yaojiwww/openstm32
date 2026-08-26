#include "board_port.h"
#include "storage.h"

static bool send_line(const char *line, void *context) {
    (void)context;
    board_log(line);
    return true;
}

void lab4_replay(void) {
    if (!storage_mount()) {
        board_log("ERR: SD mount\r\n");
        return;
    }
    board_log("BEGIN REPLAY\r\n");
    if (!storage_replay(send_line, NULL)) board_log("ERR: replay\r\n");
    board_log("END REPLAY\r\n");
}
