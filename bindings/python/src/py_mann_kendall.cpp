#include <time_series_tools/mann_kendall.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace tst;

PYBIND11_MODULE(time_series_tools, m) {
  m.doc() = "A collection of timeseries analysis tools";

  py::class_<MannKendall>(m, "MannKendall")
      .def(py::init<double>())
      .def("set_data", &MannKendall::set_data);

  py::enum_<MannKendall::Trend>(m, "Trend")
      .value("no_trend", MannKendall::Trend::no_trend)
      .value("upward", MannKendall::Trend::upward)
      .value("downward", MannKendall::Trend::downward);

  py::class_<MannKendall::Result>(m, "Result")
      .def_readwrite("trend", &MannKendall::Result::trend)
      .def_readwrite("probability", &MannKendall::Result::probability)
      .def_readwrite("slope", &MannKendall::Result::slope);
}
