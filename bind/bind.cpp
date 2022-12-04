#include "condorcet_domain.h"
#include "utils.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>


namespace py = pybind11;

PYBIND11_MODULE(cdl, m) {
    m.doc() = "Core objects and functions of the Condorcet Domain Library (CDL)";
    m.attr("__version__") = 1.3;

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
            .def_readonly("rules", &CondorcetDomain::rules)   // member variables
            .def_readonly("num_triplets", &CondorcetDomain::num_triplets)
            .def("init_empty", &CondorcetDomain::init_empty)  // creating and manipulating TRS
            .def("init_by_scheme", &CondorcetDomain::init_by_scheme, py::arg("scheme"))
            .def("assign", &CondorcetDomain::assign, py::arg("trs"), py::arg("triplet"), py::arg("rule"))
            .def("assign_by_index", &CondorcetDomain::assign_by_index, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("unassigned_triplets", &CondorcetDomain::unassigned_triplets, py::arg("trs"))
            .def("evaluate_rules_on_triplet", &CondorcetDomain::evaluate_rules_on_triplet, py::arg("trs"), py::arg("triplet"))
            .def("dynamic_triplet_ordering", &CondorcetDomain::dynamic_triplet_ordering, py::arg("trs"))
            .def("condorcet_domain", &CondorcetDomain::condorcet_domain, py::arg("trs"))  // manipulating CDs
            .def("subset_cd_sizes", &CondorcetDomain::subset_cd_sizes, py::arg("trs"), py::arg("sub_n"))
            .def("hash_cd_brothers", &CondorcetDomain::hash_cd_brothers, py::arg("cds"))
            .def("domain_brothers", &CondorcetDomain::domain_brothers, py::arg("cd"))
            .def("domain_to_trs", &CondorcetDomain::domain_to_trs, py::arg("cd"));

    // print function
    m.def("print_trs", [](TRS trs) {
        py::scoped_ostream_redirect stream(
                std::cout,
                py::module_::import("sys").attr("stdout")
        );
        print_trs(trs);
        }, py::arg("trs"));
}