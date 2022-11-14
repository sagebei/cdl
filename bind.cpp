#include "condorcet_domain.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


namespace py = pybind11;



PYBIND11_MODULE(cd, m) {
    m.doc() = "functions for CDs"; // optional module docstring

    py::bind_vector<TRS>(m, "TRS");
    py::class_<TripletRule>(m, "TripletRule")
            .def(py::init<>())
            .def_readwrite("triplet", &TripletRule::triplet)
            .def_readwrite("rule", &TripletRule::rule);

    m.def("initialize", &initialize);
}