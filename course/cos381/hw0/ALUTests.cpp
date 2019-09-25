#include <random>

#include "ALU.h"

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

class ALUTest : public ::testing::TestWithParam<ALUop> {
  protected:
    // allow second_value to only be < 32 so shifts will not be
    // invalid
    ALUTest() : first_value{u_numbers(engine)}, port{s_numbers(engine)},
	second_value{u_numbers(engine) & 0x1F}, new_value{u_numbers(engine)}
    {
	EXPECT_CALL(alu_listener, update_input(port, _))
	    .Times(1);
	alu.Reporter<32>::register_listener(&alu_listener, port);
    }

    const word first_value;
    const word second_value;
    const word new_value;
    const int port;

    MockListener<32> alu_listener;
    ALU alu;
};

#define MAP_OP_TO_CPP(op, operation, prefix) \
    case ALUop::op: return prefix(a operation b)

word ALU_computation(ALUop op, word a, word b) {
    switch(op) {
	MAP_OP_TO_CPP(Sum, +,);
	MAP_OP_TO_CPP(Sub, -,);
	MAP_OP_TO_CPP(Or, |,);
	MAP_OP_TO_CPP(LeftShift, <<,);
	MAP_OP_TO_CPP(RightShift, >>,);
	MAP_OP_TO_CPP(Xor, ^,);
	MAP_OP_TO_CPP(Nor, |, ~);
	MAP_OP_TO_CPP(SetLessThan, <,);
    }

    return 0;
}

TEST_P(ALUTest, TestOperations) {
    ALUop op = GetParam();
    word expected_value = ALU_computation(op, first_value, second_value);

    { InSequence seq;
	EXPECT_CALL(alu_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(alu_listener, update_input(port, expected_value))
	    .Times(AtLeast(1));
    }

    alu.update_input(0, toValue(op));
    alu.update_input(0, first_value);
    alu.update_input(1, second_value);
}

TEST_P(ALUTest, ChangeFirstInput) {
    ALUop op = GetParam();
    word expected_value = ALU_computation(op, first_value, second_value);
    word updated_value = ALU_computation(op, new_value, second_value);

    { InSequence seq;
	EXPECT_CALL(alu_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(alu_listener, update_input(port, expected_value))
	    .Times(AtLeast(1));
	EXPECT_CALL(alu_listener, update_input(port, updated_value))
	    .Times(AtLeast(1));
    }

    alu.update_input(0, toValue(op));
    alu.update_input(0, first_value);
    alu.update_input(1, second_value);
    alu.update_input(0, new_value);
}

TEST_P(ALUTest, ChangeOp) {
    ALUop op = GetParam();
    ALUop new_op = op != ALUop::Sum ? ALUop::Sum : ALUop::Sub;
    word expected_value = ALU_computation(op, first_value, second_value);
    word updated_value = ALU_computation(new_op, first_value, second_value);

    { InSequence seq;
	EXPECT_CALL(alu_listener, update_input(_, _))
	    .Times(AnyNumber());
	EXPECT_CALL(alu_listener, update_input(port, expected_value))
	    .Times(AtLeast(1));
	EXPECT_CALL(alu_listener, update_input(port, updated_value))
	    .Times(AtLeast(1));
    }

    alu.update_input(0, toValue(op));
    alu.update_input(0, first_value);
    alu.update_input(1, second_value);
    alu.update_input(0, toValue(new_op));
}

TEST_P(ALUTest, ConditionCodes) {
    ALUop op = GetParam();
    MockListener<1> zero_listener;
//    MockListener<1> neg_listener;

    { InSequence seq;
	EXPECT_CALL(zero_listener, update_input(_, _))
	    .Times(AnyNumber());
//	EXPECT_CALL(neg_listener, update_input(_, _))
//	    .Times(AnyNumber());

	word b = 5;
	for (word a = -2; a < 3; ++a) {
	    word output = ALU_computation(op, a, b);
	    EXPECT_CALL(zero_listener, update_input(port, Value<1>{output == 0}))
		.Times(1);
//	    EXPECT_CALL(neg_listener, update_input(port, Value<1>{output >= 0x80000000UL}))
//		.Times(AtLeast(1));

	    for (b = -2; b < 3; ++b) {
		word output = ALU_computation(op, a, b);
		EXPECT_CALL(zero_listener, update_input(port, Value<1>{output == 0}))
		    .Times(1);
//		EXPECT_CALL(neg_listener, update_input(port, Value<1>{output >= 0x80000000UL}))
//		    .Times(AtLeast(1));
	    }
	}
    }

    alu.Reporter<32>::unregister_listener(&alu_listener);
    alu.MultiReporter<1,num_codes>::register_listener(ALU::Zero, &zero_listener, port);
//    alu.MultiReporter<1,num_codes>::register_listener(ALU::Negative, &neg_listener, port);
    alu.update_input(0, toValue(op));

    alu.update_input(1, Value<32>{5});
    for (word a = -2; a < 3; ++a) {
	alu.update_input(0, a);
	for (word b = -2; b < 3; ++b) {
	    alu.update_input(1, b);
	}
    }
}

#ifdef INSTANTIATE_TEST_SUITE_P
INSTANTIATE_TEST_SUITE_P(OpTests, ALUTest,
			 testing::Values(ALUop::Sum, ALUop::Sub, ALUop::Or,
					 ALUop::LeftShift, ALUop::RightShift,
					 ALUop::Xor, ALUop::Nor,
					 ALUop::SetLessThan) );
#else
INSTANTIATE_TEST_CASE_P(OpTests, ALUTest,
			 testing::Values(ALUop::Sum, ALUop::Sub, ALUop::Or,
					 ALUop::LeftShift, ALUop::RightShift,
					 ALUop::Xor, ALUop::Nor,
					 ALUop::SetLessThan) );
#endif
