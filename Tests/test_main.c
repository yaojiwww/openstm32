#include "button.h"
#include "config.h"
#include "filter.h"
#include "fault.h"
#include "protocol.h"
#include "sampler.h"
#include "workstation.h"

#include <assert.h>
#include <stdio.h>

static void test_button(void) {
    Button button;
    button_init(&button, true, 20, 800);
    assert(button_update(&button, false, 10) == BUTTON_EVENT_NONE);
    assert(button_update(&button, true, 15) == BUTTON_EVENT_NONE); /* bounce */
    assert(button_update(&button, false, 30) == BUTTON_EVENT_NONE);
    assert(button_update(&button, false, 50) == BUTTON_EVENT_NONE); /* pressed */
    assert(button_update(&button, true, 100) == BUTTON_EVENT_NONE);
    assert(button_update(&button, true, 120) == BUTTON_EVENT_CLICK);
}

static void test_config(void) {
    AppConfig original = config_defaults();
    StoredConfig stored = config_pack(&original);
    AppConfig loaded = {0};
    assert(config_unpack(&stored, &loaded));
    assert(loaded.sample_rate_hz == 100);
    stored.checksum ^= 1u;
    assert(!config_unpack(&stored, &loaded));
}

static void test_filter(void) {
    MovingAverage filter;
    filter_init(&filter, 4);
    assert(filter_push(&filter, 100) == 100);
    assert(filter_push(&filter, 200) == 150);
    assert(filter_push(&filter, 300) == 200);
    assert(filter_push(&filter, 400) == 250);
    assert(filter_push(&filter, 500) == 350);
}

static void test_sampler_and_faults(void) {
    Sampler sampler;
    sampler_init(&sampler, 2, 3300, 4095);
    SampleRecord first = sampler_process(&sampler, 0, 10);
    SampleRecord second = sampler_process(&sampler, 4095, 20);
    assert(first.millivolts == 0);
    assert(second.filtered == 2047);

    FaultManager faults;
    fault_init(&faults);
    fault_raise(&faults, FAULT_SD_MISSING);
    fault_raise(&faults, FAULT_SD_MISSING);
    assert(fault_is_active(&faults, FAULT_SD_MISSING));
    assert(faults.occurrence_count == 1);
    fault_clear(&faults, FAULT_SD_MISSING);
    assert(!fault_is_active(&faults, FAULT_SD_MISSING));
}

static void test_protocol(void) {
    ProtocolFrame input = {.command = CMD_SET_RATE, .length = 4,
                           .payload = {100, 0, 0, 0}};
    uint8_t wire[32];
    size_t length = protocol_encode(&input, wire, sizeof(wire));
    assert(length == 9);
    ProtocolParser parser;
    ProtocolFrame output;
    protocol_parser_init(&parser);
    bool completed = false;
    for (size_t i = 0; i < length; ++i)
        completed = protocol_parser_feed(&parser, wire[i], &output) || completed;
    assert(completed);
    assert(output.command == CMD_SET_RATE && output.payload[0] == 100);

    wire[length - 1] ^= 1u;
    protocol_parser_init(&parser);
    completed = false;
    for (size_t i = 0; i < length; ++i)
        completed = protocol_parser_feed(&parser, wire[i], &output) || completed;
    assert(!completed);
}

static void test_state_machine(void) {
    AppConfig config = config_defaults();
    Workstation station;
    workstation_init(&station, &config);
    station.running = true;
    workstation_dispatch(&station, EVENT_TIMER_TICK);
    assert(station.sample_requested);
    workstation_dispatch(&station, EVENT_MODE_BUTTON);
    assert(station.mode == MODE_MANUAL_SAMPLE);
    workstation_dispatch(&station, EVENT_STOP);
    assert(!station.running);
}

int main(void) {
    test_button();
    test_config();
    test_filter();
    test_sampler_and_faults();
    test_protocol();
    test_state_machine();
    puts("All host-side logic tests passed.");
    return 0;
}
