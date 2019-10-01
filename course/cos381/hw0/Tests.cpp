#include "gtest/gtest.h"

#include "common.h"

long current_time = 0; // global used by Event and Clock
std::priority_queue<Event> events;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
