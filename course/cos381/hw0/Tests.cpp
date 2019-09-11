#include <iostream>

#include "gtest/gtest.h"
#include "Mux.h"
#include "LogicUnitTest.h"

using ::testing::InSequence;
using ::testing::AnyNumber;
using ::testing::AtLeast;
using ::testing::_;

TEST(TestMux, ConstructWithName) {
    EXPECT_NO_FATAL_FAILURE(({
	Mux<32, 32> mux{"32x32 mux"};
    }) );
}

TEST(TestMux, InputThenSelect) {
    Mux<2, 32> mux;
    Value<32> value = 3;
    int port = 1;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, value))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);
    mux.update_input(0, value);
    mux.update_input(0, Value<1>{0});
}

TEST(TestMux, SelectThenInput) {
    Mux<2, 32> mux;
    Value<32> value = 19;
    int port = 2;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, value))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);
    mux.update_input(0, Value<1>{0});
    mux.update_input(0, value);
}

TEST(TestMux, ChangeInput) {
    Mux<2, 32> mux;
    Value<32> first_value = 78;
    Value<32> second_value = 168;
    int port = 3;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(1);
	EXPECT_CALL(mux_listener, update_input(port, second_value))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);
    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(0, second_value);
}

TEST(TestMux, ChangeOutput) {
    Mux<2, 32> mux;
    Value<32> first_value = 55;
    Value<32> second_value = 999;
    int port = 4;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, second_value))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);
    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(1, second_value);
    mux.update_input(0, Value<1>{1});
}

TEST(TestMux, ChangeNonSelectedInput) {
    Mux<2, 32> mux;
    Value<32> first_value = 765;
    Value<32> second_value = 1023;
    int port = 5;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, first_value))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);
    mux.update_input(0, Value<1>{0});
    mux.update_input(0, first_value);
    mux.update_input(1, second_value);
}

TEST(TestMux, BigMux) {
    Mux<16, 32> mux;
    Value<32> value = 10;
    int port = 6;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{value}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{value * 10}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{value * 20}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{value * 30}))
	    .Times(AtLeast(1));
	EXPECT_CALL(mux_listener, update_input(port, Value<32>{value * 40}))
	    .Times(AtLeast(1));
    }

    mux.register_listener(&mux_listener, port);

    mux.update_input(0, Value<32>{value});
    mux.update_input(1, Value<32>{value * 10});
    mux.update_input(4, Value<32>{value * 20});
    mux.update_input(7, Value<32>{value * 30});
    mux.update_input(15, Value<32>{value * 40});

    mux.update_input(0, Value<4>{0});
    mux.update_input(0, Value<4>{1});
    mux.update_input(0, Value<4>{4});
    mux.update_input(0, Value<4>{7});
    mux.update_input(0, Value<4>{15});
}

TEST(TestMux, RegisterAfterUpdates) {
    Mux<2, 32> mux;
    Value<32> value = 65535;
    int port = 7;
    MockListener<32> mux_listener;

    { InSequence seq;
	EXPECT_CALL(mux_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(mux_listener, update_input(port, value))
	    .Times(AtLeast(1));
    }

    mux.update_input(0, Value<1>{0});
    mux.update_input(0, value);
    mux.register_listener(&mux_listener, port);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
