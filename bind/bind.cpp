#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

#include "condorcet_domain.h"
#include "wrapper.h"
#include "utils.h"

namespace py = pybind11;

typedef std::map<std::tuple<int, int, int>, int> TripletTupleIndex;


PYBIND11_MODULE(cdl, m) {
    m.doc() = "Core objects and functions of the Condorcet Domain Library (CDL)";
    m.attr("__version__") = "1.0.4";

    py::class_<TripletRule>(m, "TripletRule")
            .def(py::init<>())
            .def_readwrite("triplet", &TripletRule::triplet)
            .def_readwrite("rule_id", &TripletRule::rule_id)
            .def(py::pickle(
                    [](const TripletRule& tr)
                    {
                        return py::make_tuple(tr.triplet, tr.rule_id);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 2)
                            throw std::runtime_error("Invalid state for TripletRule object!");

                        TripletRule tr{};
                        tr.triplet = t[0].cast<Triplet>();
                        tr.rule_id = t[1].cast<int>();

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
            .def_readonly("rules", &CondorcetDomain::m_rules)   // member variables
            .def_readonly("rule_id", &CondorcetDomain::m_rule_id)
            .def_readonly("num_triplets", &CondorcetDomain::m_num_triplets)
            .def_readonly("triplet_elems", &CondorcetDomain::m_triplet_elems)
            .def_readonly("triplet_index", &CondorcetDomain::m_triplet_index)
            .def_readonly("sub_n", &CondorcetDomain::m_sub_n)
            .def_readonly("subset_size", &CondorcetDomain::m_subset_size)
            .def_readonly("subsets", &CondorcetDomain::m_subsets)
            .def_readonly("subset_dicts", &CondorcetDomain::m_subset_dicts)

            // creating and manipulating TRS
            .def("build_triplet_index", &CondorcetDomain::build_triplet_index, py::arg("trs"))
            .def("init_random", &CondorcetDomain::init_random, py::arg("is_sorted")=false)
            .def("init_trs", &CondorcetDomain::init_trs, py::arg("rule")="")
            .def("init_by_scheme", &CondorcetDomain::init_by_scheme, py::arg("scheme_fun"))

            .def("clear_trs", &CondorcetDomain::clear_trs, py::arg("trs"))
            .def("shuffle_trs", &CondorcetDomain::shuffle_trs, py::arg("trs"), py::arg("seed")=0)
            .def("transfer_trs", &CondorcetDomain::transfer_trs, py::arg("from"), py::arg("to"))

            .def("assign_id", &CondorcetDomain::assign_id, py::arg("trs"), py::arg("triplet"), py::arg("rule_id"))
            .def("assign_rule", &CondorcetDomain::assign_rule, py::arg("trs"), py::arg("triplet"), py::arg("rule"))
            .def("assign_id_by_index", &CondorcetDomain::assign_id_by_index, py::arg("trs"), py::arg("index"), py::arg("rule_id"))
            .def("assign_rule_by_index", &CondorcetDomain::assign_rule_by_index, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("unassigned_triplets", &CondorcetDomain::unassigned_triplets, py::arg("trs"))
            .def("assigned_triplets", &CondorcetDomain::assigned_triplets, py::arg("trs"))
            .def("evaluate_rules_on_triplet", &CondorcetDomain::evaluate_rules_on_triplet, py::arg("trs"), py::arg("triplet"))
            .def("dynamic_triplet_ordering", &CondorcetDomain::dynamic_triplet_ordering, py::arg("trs"))
            .def("uplift_trs", &CondorcetDomain::uplift_trs, py::arg("large"), py::arg("small"), py::arg("subset"))
            .def("trs_to_state", &CondorcetDomain::trs_to_state, py::arg("trs"))
            .def("state_to_trs", &CondorcetDomain::state_to_trs, py::arg("state"))

            .def("condorcet_domain", &CondorcetDomain::condorcet_domain, py::arg("trs"))  // manipulating CDs
            .def("size", &CondorcetDomain::size, py::arg("trs"))

            .def("init_subset", &CondorcetDomain::init_subset, py::arg("sub_n")=5)
            .def("subset_weights", &CondorcetDomain::subset_weights)
            .def("subset_trs_list", &CondorcetDomain::subset_trs_list, py::arg("trs"))
            .def("subset_states", &CondorcetDomain::subset_states, py::arg("trs"))
            .def("subset_states_any_ordering", &CondorcetDomain::subset_states_any_ordering, py::arg("trs"))
            .def("subset_cd_sizes", &CondorcetDomain::subset_cd_sizes, py::arg("trs"))

            .def("hash_cd_brothers", &CondorcetDomain::hash_cd_brothers, py::arg("cds"))
            .def("domain_brothers", &CondorcetDomain::domain_brothers, py::arg("cd"))
            .def("domain_to_trs", &CondorcetDomain::domain_to_trs, py::arg("cd"), py::arg("is_sorted")=false)
            .def(py::pickle(
                    [](const CondorcetDomain& cd)
                    {
                        TripletTupleIndex tti{};
                        for (auto const& [key, val] : cd.m_triplet_index)
                        {
                            tti[std::make_tuple(key[0], key[1], key[2])] = val;
                        }
                        return py::make_tuple(cd.n,
                                              cd.m_rules,
                                              cd.m_rule_id,
                                              cd.m_num_triplets,
                                              cd.m_triplet_elems,
                                              tti,
                                              cd.m_sub_n,
                                              cd.m_subset_size,
                                              cd.m_subsets,
                                              cd.m_subset_dicts);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 10)
                            throw std::runtime_error("Invalid state for CondorcetDomain object!");

                        CondorcetDomain cd(t[0].cast<int>());
                        cd.m_rules = t[1].cast<std::array<std::string, 6>>();
                        cd.m_rule_id = t[2].cast<std::map<std::string, int>>();
                        cd.m_num_triplets = t[3].cast<int>();
                        cd.m_triplet_elems = t[4].cast<std::vector<int>>();
                        TripletIndex ti{};
                        for (auto const& [key, val] : t[5].cast<TripletTupleIndex>())
                        {
                            Triplet triple = {std::get<0>(key), std::get<1>(key), std::get<2>(key)};
                            ti[triple] = val;
                        }
                        cd.m_triplet_index = ti;
                        cd.m_sub_n = t[6].cast<int>();
                        cd.m_subset_size = t[7].cast<int>();
                        cd.m_subsets = t[8].cast<std::vector<std::vector<int>>>();
                        cd.m_subset_dicts = t[9].cast<std::vector<std::map<int, int>>>();

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