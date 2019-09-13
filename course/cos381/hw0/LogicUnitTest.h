#ifndef LOGIC_UNIT_TEST_H
#define LOGIC_UNIT_TEST_H

#include "LogicUnit.h"
#include "gmock/gmock.h"

template<int N>
class MockListener : public Listener<N> {
  public:
#ifdef MOCK_METHOD
    MOCK_METHOD(void, update_input, (int num, Value<N> value), (override));
#else
    MOCK_METHOD2_T(update_input, void(int num, Value<N> value));
#endif
};

template<int N>
class MockReporter : public Reporter<N> {
  public:
#ifdef MOCK_METHOD
    MOCK_METHOD(Value<N>, get_output, (), (const override));
#else
    MOCK_CONST_METHOD0_T(get_output, Value<N>());
#endif
};

#endif
