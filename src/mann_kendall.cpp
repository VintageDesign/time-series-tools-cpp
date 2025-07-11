#include "time_series_tools/mann_kendall.h"
#include <algorithm>
#include <cmath>

using namespace tst;

MannKendall::MannKendall(double significance) : _significance(significance) {}

MannKendall::Result MannKendall::set_data(const std::vector<double> &data) {
  _data = data;

  return perform_test();
}

MannKendall::Result MannKendall::perform_test() {
  std::vector<std::int8_t> signs;
  signs.reserve((_data.size() * (_data.size() - 1)) / 2);

  for (int i = 0; i < _data.size() - 1; i++) {
    for (int j = i + 1; j < _data.size(); j++) {
      signs.push_back(compute_sign(_data[j], _data[i]));
    }
  }

  std::uint32_t positive_count = 0;
  std::uint32_t negative_count = 0;

  std::for_each(signs.begin(), signs.end(),
                [&positive_count, &negative_count](std::int8_t &value) {
                  if (value > 0) {
                    positive_count++;
                  } else if (value < 0) {
                    negative_count++;
                  }
                });

  std::int32_t s_value = positive_count - negative_count;

  std::vector<uint32_t> ties = find_ties();

  std::int32_t tie_calculation = 0;

  for (auto t : ties) {
    tie_calculation += t * (t - 1) * ((2 * t) + 5);
  }

  double variance =
      (_data.size() * (_data.size() - 1) * ((2 * _data.size()) + 5) -
       tie_calculation) /
      18.0;

  double z_value = 0;
  if (s_value > 0) {
    z_value = (s_value - 1) / std::sqrt(variance);
  } else if (s_value < 0) {
    z_value = (s_value + 1) / std::sqrt(variance);
  }

  MannKendall::Result result;
  result.probability = z_value;
  if (z_value >= (1.0 - _significance)) {
    result.trend = Trend::upward;
  }
  if (z_value <= -1.0 * (1.0 - _significance)) {
    result.trend = Trend::downward;
  }

  return result;
}

std::int8_t MannKendall::compute_sign(double lhs, double rhs) {
  double difference = lhs - rhs;

  std::uint8_t retval = 0;

  if (difference > 0) {
    retval = 1;
  } else if (difference < 0) {
    retval = -1;
  }
  return retval;
}

std::vector<std::uint32_t> MannKendall::find_ties() {
  std::vector<double> ties_already_found;
  std::vector<std::uint32_t> ties_count;

  for (auto point : _data) {
    if (ties_already_found.end() == std::find(ties_already_found.begin(),
                                              ties_already_found.end(),
                                              point)) {
      continue;
    }

    std::uint32_t count = 0;
    for (auto second_point : _data) {
      if (point == second_point) {
        count += 1;
      }
    }

    if (count > 1) {
      ties_already_found.push_back(point);
      ties_count.push_back(
          count - 1); // Account for the comparison of the point with itself
    }
  }

  return ties_count;
}
