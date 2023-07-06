#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>
#include <pybind11/functional.h>

#include "condorcet_domain.h"
#include "forbidden_permutation.h"
#include "wrapper.h"
#include "utils.h"

namespace py = pybind11;

typedef std::map<std::tuple<Int8, Int8, Int8>, Int32> TripleTupleIndex;


PYBIND11_MODULE(cdl, m) {
    m.doc() = "Core objects and functions of the Condorcet Domain Library (CDL)";
    m.attr("__version__") = "2.2.4";

    py::class_<TripleRule>(m, "TripleRule")
            .def(py::init<>())
            .def_readwrite("triple", &TripleRule::triple)
            .def_readwrite("rule_id", &TripleRule::rule_id)
            .def(py::pickle(
                    [](const TripleRule& tr)
                    {
                        return py::make_tuple(tr.triple, tr.rule_id);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 2)
                            throw std::runtime_error("Invalid state for tripleRule object!");

                        TripleRule tr{};
                        tr.triple = t[0].cast<Triple>();
                        tr.rule_id = t[1].cast<int>();

                        return tr;
                    }
            ));

    py::class_<TRSWrapper>(m, "TRSWrapper")
            .def(py::init<CondorcetDomain>(), py::arg("cd"))
            .def_readonly("allowed_rules", &TRSWrapper::allowed_rules)
            .def("change_allowed_rules", &TRSWrapper::change_allowed_rules, py::arg("triples"), py::arg("rules"))
            .def("remove_rules", &TRSWrapper::remove_rules, py::arg("trs"), py::arg("contains"))
            .def("next_unassigned_triple", &TRSWrapper::next_unassigned_triple, py::arg("trs"))
            .def("dynamic_triple_ordering", &TRSWrapper::dynamic_triple_ordering, py::arg("trs"))
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
            .def(py::init<Int8>(), py::arg("n")=8)
            .def_readonly("n", &CondorcetDomain::n)
            .def_readonly("rules", &CondorcetDomain::m_rules)   // member variables
            .def_readonly("rule_id", &CondorcetDomain::m_rule_id)
            .def_readonly("num_triples", &CondorcetDomain::m_num_triples)
            .def_readonly("triple_elems", &CondorcetDomain::m_triple_elems)
            .def_readonly("triple_index", &CondorcetDomain::m_triple_index)
            .def_readonly("sub_n", &CondorcetDomain::m_sub_n)
            .def_readonly("subset_size", &CondorcetDomain::m_subset_size)
            .def_readonly("subsets", &CondorcetDomain::m_subsets)
            .def_readonly("subset_dicts", &CondorcetDomain::m_subset_dicts)

            // creating and manipulating TRS
            .def("build_triple_index", &CondorcetDomain::build_triple_index, py::arg("trs"))
            .def("init_trs_random", &CondorcetDomain::init_trs_random, py::arg("is_sorted")=false)
            .def("init_trs", &CondorcetDomain::init_trs)
            .def("init_trs_lex", &CondorcetDomain::init_trs_lex)
            .def("init_trs_colex", &CondorcetDomain::init_trs_colex)
            .def("init_trs_by_scheme", &CondorcetDomain::init_trs_by_scheme, py::arg("scheme_fun"))

            .def("clear_trs", &CondorcetDomain::clear_trs, py::arg("trs"))
            .def("shuffle_trs", &CondorcetDomain::shuffle_trs, py::arg("trs"), py::arg("seed")=0)
            .def("transfer_trs", &CondorcetDomain::transfer_trs, py::arg("from"), py::arg("to"))

            .def("assign_id", &CondorcetDomain::assign_id, py::arg("trs"), py::arg("triple"), py::arg("rule_id"))
            .def("assign_rule", &CondorcetDomain::assign_rule, py::arg("trs"), py::arg("triple"), py::arg("rule"))
            .def("assign_id_by_index", &CondorcetDomain::assign_id_by_index, py::arg("trs"), py::arg("index"), py::arg("rule_id"))
            .def("assign_rule_by_index", &CondorcetDomain::assign_rule_by_index, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("unassigned_triples", &CondorcetDomain::unassigned_triples, py::arg("trs"))
            .def("assigned_triples", &CondorcetDomain::assigned_triples, py::arg("trs"))
            .def("evaluate_rules_on_triple", &CondorcetDomain::evaluate_rules_on_triple, py::arg("trs"), py::arg("triple"))
            .def("dynamic_triple_ordering", &CondorcetDomain::dynamic_triple_ordering, py::arg("trs"))
            .def("uplift_trs", &CondorcetDomain::uplift_trs, py::arg("large"), py::arg("small"), py::arg("subset"))
            .def("trs_to_state", &CondorcetDomain::trs_to_state, py::arg("trs"))
            .def("state_to_trs", &CondorcetDomain::state_to_trs, py::arg("state"))

            .def("domain", &CondorcetDomain::domain, py::arg("trs"))  // manipulating CDs
            .def("size", &CondorcetDomain::size, py::arg("trs"))

            .def("init_subset", &CondorcetDomain::init_subset, py::arg("sub_n")=5)
            .def("subset_weights", &CondorcetDomain::subset_weights)
            .def("subset_trs_list", &CondorcetDomain::subset_trs_list, py::arg("trs"))
            .def("subset_states", &CondorcetDomain::subset_states, py::arg("trs"))
            .def("subset_states_any_ordering", &CondorcetDomain::subset_states_any_ordering, py::arg("trs"))
            .def("subset_cd_sizes", &CondorcetDomain::subset_cd_sizes, py::arg("trs"))

            .def("inverse_cd", &CondorcetDomain::inverse_cd, py::arg("cd"), py::arg("permutation"))
            .def("domain_brothers", &CondorcetDomain::domain_brothers, py::arg("cd"))
            .def("isomorphic_hash", &CondorcetDomain::isomorphic_hash, py::arg("cd"))
            .def("non_isomorphic_domains", &CondorcetDomain::non_isomorphic_domains, py::arg("cds"))
            .def("domain_to_trs", &CondorcetDomain::domain_to_trs, py::arg("cd"))
            .def(py::pickle(
                    [](const CondorcetDomain& cd)
                    {
                        TripleTupleIndex tti{};
                        for (auto const& [key, val] : cd.m_triple_index)
                        {
                            tti[std::make_tuple(key[0], key[1], key[2])] = val;
                        }
                        return py::make_tuple(cd.n,
                                              cd.m_rules,
                                              cd.m_rule_id,
                                              cd.m_num_triples,
                                              cd.m_triple_elems,
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

                        CondorcetDomain cd(t[0].cast<Int8>());
                        cd.m_rules = t[1].cast<std::array<std::string, 6>>();
                        cd.m_rule_id = t[2].cast<std::map<std::string, Int8>>();
                        cd.m_num_triples = t[3].cast<Int32>();
                        cd.m_triple_elems = t[4].cast<std::vector<Int8>>();
                        TripleIndex ti{};
                        for (auto const& [key, val] : t[5].cast<TripleTupleIndex>())
                        {
                            Triple triple = {std::get<0>(key), std::get<1>(key), std::get<2>(key)};
                            ti[triple] = val;
                        }
                        cd.m_triple_index = ti;
                        cd.m_sub_n = t[6].cast<Int8>();
                        cd.m_subset_size = t[7].cast<Int32>();
                        cd.m_subsets = t[8].cast<std::vector<std::vector<Int8>>>();
                        cd.m_subset_dicts = t[9].cast<std::vector<std::map<Int8, Int8>>>();

                        return cd;
                    }
            ));

    py::class_<TripleLaws>(m, "TripleLaws")
        .def(py::init<>())
        .def_readwrite("triple", &TripleLaws::triple)
        .def_readwrite("laws", &TripleLaws::laws)
        .def(py::pickle(
                [](const TripleLaws& tl)
                {
                    return py::make_tuple(tl.triple, tl.laws);
                },
                [](py::tuple t)
                {
                    if (t.size() != 2)
                        throw std::runtime_error("Invalid state for tripleLaws object!");

                    TripleLaws tl{};
                    tl.triple = t[0].cast<Triple>();
                    tl.laws = t[1].cast<std::vector<std::string>>();

                    return tl;
                }
        ));

    py::class_<ForbiddenPermutation>(m, "ForbiddenPermutation")
        .def(py::init<Int8>(), py::arg("n"))
        .def_readonly("n", &ForbiddenPermutation::n)
        .def_readonly("triple_index", &ForbiddenPermutation::m_triple_index)
        .def_readonly("laws", &ForbiddenPermutation::m_laws)

        .def("init_tls", &ForbiddenPermutation::init_tls)
        .def("init_tls_by_scheme", &ForbiddenPermutation::init_tls_by_scheme)
        .def("assign_laws", &ForbiddenPermutation::assign_laws, py::arg("tls"), py::arg("triple"), py::arg("laws"))
        .def("assign_laws_by_index", &ForbiddenPermutation::assign_laws_by_index, py::arg("tls"), py::arg("index"), py::arg("laws"))
        .def("domain", &ForbiddenPermutation::domain, py::arg("tls"))
        .def("size", &ForbiddenPermutation::size, py::arg("tls"))

        .def(py::pickle(
                [](const ForbiddenPermutation& fp)
                {
                    TripleTupleIndex tti{};
                    for (auto const& [key, val] : fp.m_triple_index)
                    {
                        tti[std::make_tuple(key[0], key[1], key[2])] = val;
                    }
                    return py::make_tuple(fp.n, tti, fp.m_laws);
                },
                [](py::tuple t)
                {
                    if (t.size() != 3)
                        throw std::runtime_error("Invalid state for ForbiddenPermutation object!");

                    ForbiddenPermutation fp(t[0].cast<Int8>());

                    TripleIndex ti{};
                    for (auto const& [key, val] : t[1].cast<TripleTupleIndex>())
                    {
                        Triple triple = {std::get<0>(key), std::get<1>(key), std::get<2>(key)};
                        ti[triple] = val;
                    }
                    fp.m_triple_index = ti;

                    fp.m_laws = t[2].cast<std::array<std::string, 5>>();
                    return fp;
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

    m.def("Fishburn_scheme", &Fishburn_scheme, py::arg("triple"));

}