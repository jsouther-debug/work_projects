#ifndef CLOCK_TEST_H
#define CLOCK_TEST_H

#include "Clock.h"
#include "gmock/gmock.h"

class MockClock : public Clock {
  public:
    MockClock(std::string name);
    void tick() const;

    MOCK_METHOD(Value<1>, get_output, (), (const override));
};

MockClock::MockClock(std::string name) : Clock(name) {
}

void MockClock::tick() const {
    update_listeners();
}

#endif
