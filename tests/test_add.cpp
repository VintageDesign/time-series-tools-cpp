#include "add.h"
#include <gtest/gtest.h>

TEST(TestAdd, ShouldPass) { EXPECT_EQ(2, add(1, 1)); }
