#include <gtest/gtest.h>

#include "A.h"

TEST(ClassATest, Addition) {
  A obj;
  int result = obj.Addition(2, 3);
  EXPECT_EQ(result, 5);
  EXPECT_TRUE(result == 5);
}

TEST(ClassATest, Subtraction) {
  A obj;
  EXPECT_EQ(obj.Subtraction(5, 3), 2);
  EXPECT_EQ(obj.Subtraction(5, 2), 3);
  EXPECT_EQ(obj.Subtraction(5, 1), 4);
  EXPECT_EQ(obj.Subtraction(1, 3), -2);
}