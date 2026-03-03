//
// Created by Rodney on 2 March 2026
//
#include "gtest/gtest.h"
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    int res = 0;
    res = RUN_ALL_TESTS();
    return res;
}