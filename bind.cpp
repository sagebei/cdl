#include "condorcet_domain.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


namespace py = pybind11;

PYBIND11_MODULE(cd, m) {
    m.doc() = "functions for CDs"; // optional module docstring

    py::bind_vector<TRS>(m, "TRS"); // https://github.com/pybind/pybind11/issues/1668
    py::class_<TripletRule>(m, "TripletRule")
            .def(py::init<>())
            .def_readwrite("triplet", &TripletRule::triplet)
            .def_readwrite("rule", &TripletRule::rule);

    py::class_<CondorcetDomain>(m, "CondorcetDomain")
            .def(py::init<int>())
            .def("initialize", &CondorcetDomain::initialize)
            .def("condorcet_domain", &CondorcetDomain::condorcet_domain)
            .def("print_cd", &CondorcetDomain::print_cd)
            .def("print_tr", &CondorcetDomain::print_tr);
}