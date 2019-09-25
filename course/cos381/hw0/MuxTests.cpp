#include <random>

#include "Mux.h"

#include "gtest/gtest.h"
#include "LogicUnitTest.h"

using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::_;

static std::uniform_int_distribution<unsigned> u_numbers;
static std::uniform_int_distribution<int> s_numbers;
static std::random_device rd;
static std::default_random_engine engine{rd()};

class MuxTest : public ::testing::Test {
  protected:
    MuxTest() : first_value{u_numbers(engine)}, port{s_numbers(engine)},
	second_value{u_numbers(engine)}
    {
	EXPECT_CALL(mux_listener, update_input(port, _))
	    .Times(1);
	mux.register_listener(&mux_listener, port);
    }

    const Value<32> first_value;
    const Value<32> second_value;
    const int port;

    MockListener<32> mux_listener;
    Mux<2, 32> mux;
};

TEST_F(MuxTest, ConstructWithName) {
    EXPECT_NO_FATAL_FAILURE(({
	Mux<32, 32> mux{"32x32 mux"};
    }) );
}

TEST_F(MuxTest, InputThenSelect) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, first_value);
    mux.update_input(0, Value<1>{0});
}

TEST_F(MuxTest, SelectThenInput) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
}

TEST_F(MuxTest, ChangeInput) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(1);
	EXPECT_CALL(mux_listener, update_input(port, second_value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(0, second_value);
}

TEST_F(MuxTest, ChangeOutput) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, second_value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(1, second_value);
    mux.update_input(0, Value<1>{1});
}

TEST_F(MuxTest, ChangeNonSelectedInput) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(1, second_value);
}

TEST_F(MuxTest, BigMux) {
    Mux<16, 32> big_mux;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{first_value}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{first_value * 10}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{first_value * 20}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{first_value * 30}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{first_value * 40}))
	    .Times(AtLeast(1));
    }

    big_mux.register_listener(&mux_listener, port);

    big_mux.update_input(0, Value<32>{first_value});
    big_mux.update_input(1, Value<32>{first_value * 10});
    big_mux.update_input(4, Value<32>{first_value * 20});
    big_mux.update_input(7, Value<32>{first_value * 30});
    big_mux.update_input(15, Value<32>{first_value * 40});

    big_mux.update_input(0, Value<4>{0});
    big_mux.update_input(0, Value<4>{1});
    big_mux.update_input(0, Value<4>{4});
    big_mux.update_input(0, Value<4>{7});
    big_mux.update_input(0, Value<4>{15});
}

TEST_F(MuxTest, RegisterAfterUpdates) {
    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
    }

    mux.unregister_listener(&mux_listener);
    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.register_listener(&mux_listener, port);
}
