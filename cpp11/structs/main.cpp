/*
    Pass to and mutate struct in functions.
*/
#include <cstdio>
#include <cstdint>
#include <cassert>

struct SlaveState {

    uint16_t fanLevel;
    uint16_t lightLevel;
    uint16_t damperClosed;
    SlaveState(uint16_t fanLevel, uint16_t lightLevel, uint16_t damperClosed):
        fanLevel(fanLevel), lightLevel(lightLevel), damperClosed(damperClosed) {}
};

struct MasterSlaveState:SlaveState {
    uint8_t online;
    uint32_t latency;
    MasterSlaveState(uint16_t fanLevel, uint16_t lightLevel, uint16_t damperClosed,
                     uint8_t online, uint32_t latency):
        SlaveState(fanLevel, lightLevel, damperClosed), online(online), latency(latency)   {}
};

struct DesktopRequest {
    uint16_t backlight;
    struct MasterSlaveState slave1 {
        11, 12, 0, 0, 111
    };
    struct MasterSlaveState slave2 {
        21, 22, 0, 0, 211
    };
};

void changeState(DesktopRequest state) {
    state.backlight = 101;
    state.slave1.fanLevel = 101;
    state.slave1.lightLevel = 102;
    state.slave1.damperClosed = 1;
    state.slave1.online = 0;
    state.slave1.latency = 1111;
    state.slave2 = MasterSlaveState{121, 122, 1, 1, 1211};
}

void changeStateRef(DesktopRequest &state) {
    state.backlight = 201;
    state.slave1.fanLevel = 211;
    state.slave1.lightLevel = 212;
    state.slave1.damperClosed = 1;
    state.slave1.online = 0;
    state.slave1.latency = 2111;
    state.slave2 = MasterSlaveState{221, 222, 1, 1, 2211};
}


void changeStatePtr(DesktopRequest *state) {
    state->backlight = 301;
    state->slave1.fanLevel = 311;
    state->slave1.lightLevel = 312;
    state->slave1.damperClosed = 1;
    state->slave1.online = 0;
    state->slave1.latency = 3111;
    state->slave2 = MasterSlaveState{321, 322, 1, 1, 3211};
}

void changeStateDeref(DesktopRequest *state) {
    DesktopRequest  &stateRef = *state;
    stateRef.backlight = 401;

    stateRef.slave1.fanLevel = 411;
    stateRef.slave1.lightLevel = 412;
    stateRef.slave1.damperClosed = 1;
    stateRef.slave1.online = 0;
    stateRef.slave1.latency = 4111;
    stateRef.slave2 = MasterSlaveState{421, 422, 1, 1, 4211};
}

void printState(DesktopRequest state) {
    std::printf("mb=%u s1fl=%u s1ll=%u s2fl=%u s2ll=%u\n",
                state.backlight,
                state.slave1.fanLevel, state.slave1.lightLevel,
                state.slave2.fanLevel, state.slave2.lightLevel);
}


int main() {
    DesktopRequest state;
    state.backlight = 123;
    printState(state);

    changeState(state);
    printState(state);
    assert(state.backlight==123);
    assert(state.slave1.fanLevel==11);
    assert(state.slave1.lightLevel==12);
    assert(state.slave1.damperClosed==0);
    assert(state.slave1.online==0);
    assert(state.slave1.latency==111);
    assert(state.slave2.fanLevel==21);
    assert(state.slave2.lightLevel==22);
    assert(state.slave2.damperClosed==0);
    assert(state.slave2.online==0);
    assert(state.slave2.latency==211);

    changeStateRef(state);
    printState(state);

    assert(state.backlight==201);
    assert(state.slave1.fanLevel==211);
    assert(state.slave1.lightLevel==212);
    assert(state.slave1.damperClosed==1);
    assert(state.slave1.online==0);
    assert(state.slave1.latency==2111);
    assert(state.slave2.fanLevel==221);
    assert(state.slave2.lightLevel==222);
    assert(state.slave2.damperClosed==1);
    assert(state.slave2.online==1);
    assert(state.slave2.latency==2211);

    changeStatePtr(&state);
    printState(state);

    assert(state.backlight==301);
    assert(state.slave1.fanLevel==311);
    assert(state.slave1.lightLevel==312);
    assert(state.slave1.damperClosed==1);
    assert(state.slave1.online==0);
    assert(state.slave1.latency==3111);
    assert(state.slave2.fanLevel==321);
    assert(state.slave2.lightLevel==322);
    assert(state.slave2.damperClosed==1);
    assert(state.slave2.online==1);
    assert(state.slave2.latency==3211);

    changeStateDeref(&state);
    printState(state);

    assert(state.backlight==401);
    assert(state.slave1.fanLevel==411);
    assert(state.slave1.lightLevel==412);
    assert(state.slave1.damperClosed==1);
    assert(state.slave1.online==0);
    assert(state.slave1.latency==4111);
    assert(state.slave2.fanLevel==421);
    assert(state.slave2.lightLevel==422);
    assert(state.slave2.damperClosed==1);
    assert(state.slave2.online==1);
    assert(state.slave2.latency==4211);
}
