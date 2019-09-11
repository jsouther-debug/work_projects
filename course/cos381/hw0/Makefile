#CXXFLAGS = -std=c++17 -O3 -g
CXXFLAGS = -std=c++17 -g
LDFLAGS = ${CXXFLAGS}

GTEST_DIR = /usr/src/googletest

libgtest.a:
	${CXX} ${CXXFLAGS} -isystem ${GTEST_DIR}/googletest/include -I${GTEST_DIR}/googletest -pthread -c ${GTEST_DIR}/googletest/src/gtest-all.cc
	${CXX} ${CXXFLAGS} -isystem ${GTEST_DIR}/googlemock/include -I${GTEST_DIR}/googletest/include -I${GTEST_DIR}/googlemock -pthread -c ${GTEST_DIR}/googlemock/src/gmock-all.cc
	ar -rv libgtest.a gtest-all.o
	ar -rv libgtest.a gmock-all.o

Tests: libgtest.a Tests.cpp
	${CXX} ${CXXFLAGS} -isystem ${GTEST_DIR}/googletest/include -isystem ${GTEST_DIR}/googlemock/include -pthread Tests.cpp libgtest.a -o Tests
