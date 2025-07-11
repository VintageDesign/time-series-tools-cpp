#include <gtest/gtest.h>
#include <stdio.h>
#include <time_series_tools/mann_kendall.h>

using namespace tst;

TEST(MannKendallTests, TestUpwardTrend) {
  std::vector<double> data = {1, -2, 3, -2, 5, -2, 6, -2, 8, -2, 10};

  MannKendall algorithm(.05);

  auto result = algorithm.set_data(data);

  EXPECT_EQ(result.trend, MannKendall::Trend::upward);
}

TEST(MannKendallTests, TestDownwardTrend) {
  std::vector<double> data = {-1, 2, -3, 2, -5, 2, -6, 2, -8, 2, -10};

  MannKendall algorithm(.05);

  auto result = algorithm.set_data(data);

  EXPECT_EQ(result.trend, MannKendall::Trend::downward);
}

TEST(MannKendallTests, TestNoTrend) {
  std::vector<double> data = {-1, 1, -1, 1, -1, 1};

  MannKendall algorithm(.05);

  auto result = algorithm.set_data(data);

  EXPECT_EQ(result.trend, MannKendall::Trend::no_trend);
}
