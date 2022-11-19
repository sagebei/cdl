#include "condorcet_domain.h"
#include "utils.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


namespace py = pybind11;

PYBIND11_MODULE(cdl, m) {
    m.doc() = "functions for CDs"; // optional module docstring

//    py::bind_vector<TRS>(m, "TRS"); // https://github.com/pybind/pybind11/issues/1668

    py::class_<TripletRule>(m, "TripletRule")
            .def(py::init<>())
            .def_readwrite("triplet", &TripletRule::triplet)
            .def_readwrite("rule", &TripletRule::rule);

    py::class_<RuleScheme>(m, "RuleScheme")
            .def(py::init<>())
            .def_readwrite("numbers", &RuleScheme::numbers)
            .def_readwrite("rules", &RuleScheme::rules)
            .def("add", &RuleScheme::add);

    py::class_<CondorcetDomain>(m, "CondorcetDomain")
            .def(py::init<int>(), py::arg("n")=5)
            .def("init", &CondorcetDomain::init, py::arg("sort")=false)
            .def("init_by_scheme", &CondorcetDomain::init_by_scheme, py::arg("scheme"))
            .def("assign_rule", &CondorcetDomain::assign_rule, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("condorcet_domain", &CondorcetDomain::condorcet_domain, py::arg("trs"))
            .def("hash_cd_brothers", &CondorcetDomain::hash_cd_brothers, py::arg("cds"))
            .def("cd_brothers", &CondorcetDomain::cd_brothers, py::arg("cd"))
            .def("cd_to_trs", &CondorcetDomain::cd_to_trs, py::arg("cd"))
            .def("print_cd", &CondorcetDomain::print_cd, py::arg("cd"))
            .def("print_trs", &CondorcetDomain::print_trs, py::arg("trs"));
}