#CXXFLAGS = -std=c++17 -O3 -g
CXXFLAGS = -std=c++17 -g
LDFLAGS = ${CXXFLAGS}

GTEST_DIR = /usr/src/googletest

TEST_CXXFLAGS = ${CXXFLAGS} -isystem ${GTEST_DIR}/googletest/include -isystem ${GTEST_DIR}/googlemock/include -pthread

OBJS = ALU.o

TEST_OBJS = MuxTests.o \
	    ALUTests.o

libgtest.a:
	${CXX} ${CXXFLAGS} -isystem ${GTEST_DIR}/googletest/include -I${GTEST_DIR}/googletest -pthread -c ${GTEST_DIR}/googletest/src/gtest-all.cc
	${CXX} ${CXXFLAGS} -isystem ${GTEST_DIR}/googlemock/include -I${GTEST_DIR}/googletest/include -I${GTEST_DIR}/googlemock -pthread -c ${GTEST_DIR}/googlemock/src/gmock-all.cc
	ar -rv libgtest.a gtest-all.o
	ar -rv libgtest.a gmock-all.o

Tests: ${OBJS} ${TEST_OBJS} libgtest.a Tests.cpp
	${CXX} ${TEST_CXXFLAGS} Tests.cpp ${OBJS} ${TEST_OBJS} libgtest.a -o Tests

ALU.o: ALU.cpp ALU.h LogicUnit.h common.h

MuxTests.o: Mux.h

${TEST_OBJS}: %.o: %.cpp
	${CXX} ${TEST_CXXFLAGS} -c $<
