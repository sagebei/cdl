#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

#include "condorcet_domain.h"
#include "wrapper.h"
#include "utils.h"

namespace py = pybind11;


PYBIND11_MODULE(cdl, m) {
    m.doc() = "Core objects and functions of the Condorcet Domain Library (CDL)";
    m.attr("__version__") = "1.0.4";

    py::class_<TripletRule>(m, "TripletRule")
            .def(py::init<>())
            .def_readwrite("triplet", &TripletRule::triplet)
            .def_readwrite("rule", &TripletRule::rule)
            .def(py::pickle(
                    [](const TripletRule& tr)
                    {
                        return py::make_tuple(tr.triplet, tr.rule);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 2)
                            throw std::runtime_error("Invalid state for TripletRule object!");

                        TripletRule tr{};
                        tr.triplet = t[0].cast<Triplet>();
                        tr.rule = t[1].cast<std::string>();

                        return tr;
                    }
            ));

    py::class_<TRSWrapper>(m, "TRSWrapper")
            .def(py::init<CondorcetDomain>(), py::arg("cd"))
            .def_readonly("allowed_rules", &TRSWrapper::allowed_rules)
            .def("change_allowed_rules", &TRSWrapper::change_allowed_rules, py::arg("triplets"), py::arg("rules"))
            .def("remove_rules", &TRSWrapper::remove_rules, py::arg("trs"), py::arg("contains"))
            .def("next_unassigned_triplet", &TRSWrapper::next_unassigned_triplet, py::arg("trs"))
            .def("dynamic_triplet_ordering", &TRSWrapper::dynamic_triplet_ordering, py::arg("trs"))
            .def(py::pickle(
                    [](const TRSWrapper& wrapper)
                    {
                        return py::make_tuple(wrapper.cd, wrapper.allowed_rules);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 2)
                            throw std::runtime_error("Invalid state for TRSWrapper object!");

                        TRSWrapper wrapper(t[0].cast<CondorcetDomain>());
                        wrapper.allowed_rules = t[1].cast<AllowedRules>();
                        return wrapper;
                    }
            ));

    py::class_<CondorcetDomain>(m, "CondorcetDomain")
            .def(py::init<int>(), py::arg("n")=8)
            .def_readonly("n", &CondorcetDomain::n)
            .def_readonly("rules", &CondorcetDomain::rules)   // member variables
            .def_readonly("num_triplets", &CondorcetDomain::num_triplets)

            .def("init_empty", &CondorcetDomain::init_empty, py::arg("is_sorted")=false)  // creating and manipulating TRS
            .def("init_random", &CondorcetDomain::init_random, py::arg("is_sorted")=false)
            .def("init_by_scheme", &CondorcetDomain::init_by_scheme, py::arg("scheme_fun"), py::arg("is_sorted")=false)

            .def("clear_trs", &CondorcetDomain::clear_trs, py::arg("trs"))
            .def("shuffle_trs", &CondorcetDomain::shuffle_trs, py::arg("trs"), py::arg("seed"))
            .def("transfer_trs", &CondorcetDomain::transfer_trs, py::arg("from"), py::arg("to"))

            .def("assign", &CondorcetDomain::assign, py::arg("trs"), py::arg("triplet"), py::arg("rule"))
            .def("assign_by_index", &CondorcetDomain::assign_by_index, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("unassigned_triplets", &CondorcetDomain::unassigned_triplets, py::arg("trs"))
            .def("assigned_triplets", &CondorcetDomain::assigned_triplets, py::arg("trs"))
            .def("evaluate_rules_on_triplet", &CondorcetDomain::evaluate_rules_on_triplet, py::arg("trs"), py::arg("triplet"))
            .def("dynamic_triplet_ordering", &CondorcetDomain::dynamic_triplet_ordering, py::arg("trs"))
            .def("uplift_trs", &CondorcetDomain::uplift_trs, py::arg("large"), py::arg("small"), py::arg("subset"))
            .def("trs_to_state", &CondorcetDomain::trs_to_state, py::arg("trs"))
            .def("state_to_trs", &CondorcetDomain::state_to_trs, py::arg("state"))

            .def("condorcet_domain", &CondorcetDomain::condorcet_domain, py::arg("trs"))  // manipulating CDs
            .def("subset_weights", &CondorcetDomain::subset_weights, py::arg("sub_n")=5)
            .def("subset_trss", &CondorcetDomain::subset_trss, py::arg("trs"), py::arg("sub_n")=5)
            .def("subset_states", &CondorcetDomain::subset_states, py::arg("trs"), py::arg("sub_n")=5)
            .def("subset_cd_sizes", &CondorcetDomain::subset_cd_sizes, py::arg("trs"), py::arg("sub_n")=5)
            .def("hash_cd_brothers", &CondorcetDomain::hash_cd_brothers, py::arg("cds"))
            .def("domain_brothers", &CondorcetDomain::domain_brothers, py::arg("cd"))
            .def("domain_to_trs", &CondorcetDomain::domain_to_trs, py::arg("cd"), py::arg("is_sorted")=false)
            .def(py::pickle(
                    [](const CondorcetDomain& cd)
                    {
                        return py::make_tuple(cd.n,
                                              cd.rules,
                                              cd.num_triplets,
                                              cd.triplet_elems,
                                              cd.triplet_index);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 5)
                            throw std::runtime_error("Invalid state for CondorcetDomain object!");

                        CondorcetDomain cd(t[0].cast<int>());
                        cd.rules = t[1].cast<std::array<std::string, 4>>();
                        cd.num_triplets = t[2].cast<int>();
                        cd.triplet_elems = t[3].cast<std::vector<int>>();
                        cd.triplet_index = t[4].cast<TripletIndex>();

                        return cd;
                    }
            ));

    // print function
    m.def("print_trs", [](TRS trs) {
        py::scoped_ostream_redirect stream(
                std::cout,
                py::module_::import("sys").attr("stdout")
        );
        print_trs(trs);
        }, py::arg("trs"));

    m.def("benchmark_size", &benchmark_size, py::arg("n"));

}