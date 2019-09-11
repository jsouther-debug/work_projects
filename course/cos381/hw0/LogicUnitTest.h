#ifndef LOGIC_UNIT_TEST_H
#define LOGIC_UNIT_TEST_H

#include "LogicUnit.h"
#include "gmock/gmock.h"

template<int N>
class MockListener : public Listener<N> {
  public:
    MOCK_METHOD(void, update_input, (int num, Value<N> value), (override));
};

template<int N>
class MockReporter : public Reporter<N> {
  public:
    MOCK_METHOD(Value<N>, get_output, (), (const override));
};

#endif
