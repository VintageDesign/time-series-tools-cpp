#include <cstdint>
#include <vector>

namespace tst {
class MannKendall {
public:
  MannKendall(double significance = .05);
  ~MannKendall() = default;

  enum Trend {
    no_trend,
    upward,
    downward

  };

  struct Result {
    Trend trend = no_trend;
    double probability = 0;
    double slope = 0;
  };

  Result set_data(const std::vector<double> &data);

private:
  double _significance;
  std::vector<double> _data;

  Result perform_test();
  std::int8_t compute_sign(double lhs, double rhs);

  double calculate_p_value(double z);
  double calculate_slope();

  std::vector<std::uint32_t> find_ties();
};
} // namespace  tst
