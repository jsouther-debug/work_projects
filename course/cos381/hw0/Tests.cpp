#include "gtest/gtest.h"

long current_time = 0; // global used by Event and Clock

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
