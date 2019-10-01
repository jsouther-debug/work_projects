#include <random>

#include "RegisterFile.h"

#include "gtest/gtest.h"
#include "LogicUnitTest.h"
#include "ClockTest.h"

using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::_;

static std::uniform_int_distribution<unsigned> u_numbers;
static std::uniform_int_distribution<int> s_numbers;
static std::random_device rd;
static std::default_random_engine engine{rd()};

class RegisterFileTest : public ::testing::Test {
  protected:
    RegisterFileTest()
	: first_port{s_numbers(engine)}, second_port{s_numbers(engine)},
	  first_value{u_numbers(engine)}, second_value{u_numbers(engine)},
	  clock{"Mock"}, reg_file(&clock)
    {
	EXPECT_CALL(first_listener, update_input(first_port, _))
	    .Times(AtLeast(1));
	EXPECT_CALL(second_listener, update_input(second_port, _))
	    .Times(AtLeast(1));
	EXPECT_CALL(clock, get_output()).Times(AtLeast(31));

	reg_file.register_listener(0, &first_listener, first_port);
	reg_file.register_listener(1, &second_listener, second_port);
    }

    const int first_port;
    const int second_port;

    const word first_value;
    const word second_value;

    MockListener<32> first_listener;
    MockListener<32> second_listener;
    NiceMock<MockClock> clock;

    RegisterFile reg_file;
};

TEST_F(RegisterFileTest, ConstructWithClock) {
    EXPECT_NO_FATAL_FAILURE(({
	RegisterFile reg_file2{&clock};
    }) );
}

TEST_F(RegisterFileTest, FirstOutput) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);

    EXPECT_NE(reg_file.get_output(0, 0), first_value);
    clock.tick();
    EXPECT_EQ(reg_file.get_output(0, 1), first_value);
}

TEST_F(RegisterFileTest, SecondOutput) {
    reg_file.update_input(1, Value<5>{2});
    reg_file.update_input(2, Value<5>{2});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, second_value);

    EXPECT_NE(reg_file.get_output(1, 0), second_value);
    clock.tick();
    EXPECT_EQ(reg_file.get_output(1, 1), second_value);
}

TEST_F(RegisterFileTest, RegisterZero) {
    reg_file.update_input(0, Value<5>{0});
    reg_file.update_input(1, Value<5>{0});
    reg_file.update_input(2, Value<5>{0});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);

    EXPECT_EQ(reg_file.get_output(0, 0), 0);
    EXPECT_EQ(reg_file.get_output(1, 0), 0);
    clock.tick();
    EXPECT_EQ(reg_file.get_output(0, 1), 0);
    EXPECT_EQ(reg_file.get_output(1, 1), 0);
}

TEST_F(RegisterFileTest, ChangeOutputOne) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(1, Value<5>{1});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);
    reg_file.update_input(0, Value<5>{2});

    EXPECT_NE(reg_file.get_output(0, 0), first_value);
    EXPECT_NE(reg_file.get_output(1, 0), first_value);
    clock.tick();
    EXPECT_NE(reg_file.get_output(0, 1), first_value);
    EXPECT_EQ(reg_file.get_output(1, 1), first_value);
}

TEST_F(RegisterFileTest, ChangeOutputTwo) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(1, Value<5>{1});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, second_value);
    reg_file.update_input(1, Value<5>{2});

    EXPECT_NE(reg_file.get_output(0, 0), second_value);
    EXPECT_NE(reg_file.get_output(1, 0), second_value);
    clock.tick();
    EXPECT_EQ(reg_file.get_output(0, 1), second_value);
    EXPECT_NE(reg_file.get_output(1, 1), second_value);
}

TEST_F(RegisterFileTest, ChangeInputLocation) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(1, Value<5>{2});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);
    reg_file.update_input(2, Value<5>{2});

    EXPECT_NE(reg_file.get_output(0, 0), first_value);
    EXPECT_NE(reg_file.get_output(1, 0), first_value);
    clock.tick();
    EXPECT_NE(reg_file.get_output(0, 1), first_value);
    EXPECT_EQ(reg_file.get_output(1, 1), first_value);
}

TEST_F(RegisterFileTest, ChangeWE) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);
    reg_file.update_input(4, Value<1>{0});

    EXPECT_NE(reg_file.get_output(0, 0), first_value);
    clock.tick();
    EXPECT_NE(reg_file.get_output(0, 1), first_value);
}

TEST_F(RegisterFileTest, ChangeInputData) {
    reg_file.update_input(0, Value<5>{1});
    reg_file.update_input(2, Value<5>{1});
    reg_file.update_input(4, Value<1>{1});
    reg_file.update_input(3, first_value);
    reg_file.update_input(3, second_value);

    EXPECT_NE(reg_file.get_output(0, 0), first_value);
    EXPECT_NE(reg_file.get_output(0, 1), second_value);
    clock.tick();
    EXPECT_NE(reg_file.get_output(0, 2), first_value);
    EXPECT_EQ(reg_file.get_output(0, 3), second_value);
}

TEST_F(RegisterFileTest, CompleteRegisterSweepOutputOne) {
    for (Value<5> i = 31; i > 0; --i) {
	reg_file.update_input(0, i);
	reg_file.update_input(2, i);
	reg_file.update_input(4, Value<1>{1});
	reg_file.update_input(3, first_value + i);

	EXPECT_NE(reg_file.get_output(0, 0), first_value + i);
	clock.tick();
	EXPECT_EQ(reg_file.get_output(0, 1), first_value + i);
    }
}

TEST_F(RegisterFileTest, CompleteRegisterSweepOutputTwo) {
    for (Value<5> i = 31; i > 0; --i) {
	reg_file.update_input(1, i);
	reg_file.update_input(2, i);
	reg_file.update_input(4, Value<1>{1});
	reg_file.update_input(3, second_value + i);

	EXPECT_NE(reg_file.get_output(1, 0), second_value + i);
	clock.tick();
	EXPECT_EQ(reg_file.get_output(1, 1), second_value + i);
    }
}
