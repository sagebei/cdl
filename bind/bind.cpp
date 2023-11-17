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
    m.attr("__version__") = "2.2.5";

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
            .def_readonly("subsets", &CondorcetDomain::m_subsets)
            .def_readonly("subset_dicts", &CondorcetDomain::m_subset_dicts)

            // creating and manipulating TRS
            .def("build_triple_index", &CondorcetDomain::build_triple_index, py::arg("trs"))
            .def("sort_trs", &CondorcetDomain::sort_trs, py::arg("trs"))
            .def("init_trs_random", &CondorcetDomain::init_trs_random, py::arg("is_sorted")=false)
            .def("init_trs", &CondorcetDomain::init_trs)
            .def("trs_to_rz_ordering", &CondorcetDomain::trs_to_rz_ordering, py::arg("trs"))
            .def("init_trs_lex", &CondorcetDomain::init_trs_lex)
            .def("init_trs_colex", &CondorcetDomain::init_trs_colex)
            .def("init_trs_by_scheme", &CondorcetDomain::init_trs_by_scheme, py::arg("scheme_fun"))

            .def("clear_trs", &CondorcetDomain::clear_trs, py::arg("trs"))
            .def("shuffle_trs", &CondorcetDomain::shuffle_trs, py::arg("trs"), py::arg("seed")=0)
            .def("transfer_trs", &CondorcetDomain::transfer_trs, py::arg("from"), py::arg("to"))

            .def("assign_id", &CondorcetDomain::assign_id, py::arg("trs"), py::arg("triple"), py::arg("rule_id"))
            .def("assign_id_any_ordering", &CondorcetDomain::assign_id_any_ordering, py::arg("trs"), py::arg("triple"), py::arg("rule_id"))
            .def("assign_rule", &CondorcetDomain::assign_rule, py::arg("trs"), py::arg("triple"), py::arg("rule"))
            .def("assign_rule_any_ordering", &CondorcetDomain::assign_rule_any_ordering, py::arg("trs"), py::arg("triple"), py::arg("rule"))
            .def("assign_id_by_index", &CondorcetDomain::assign_id_by_index, py::arg("trs"), py::arg("index"), py::arg("rule_id"))
            .def("assign_rule_by_index", &CondorcetDomain::assign_rule_by_index, py::arg("trs"), py::arg("index"), py::arg("rule"))
            .def("next_unassigned_triple", &CondorcetDomain::next_unassigned_triple, py::arg("trs"))
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
            .def("subset_domain_list", &CondorcetDomain::subset_domain_list, py::arg("domain"))
            .def("domain_on_triple", &CondorcetDomain::domain_on_triple, py::arg("cd"), py::arg("triple"))
            .def("subset_states", &CondorcetDomain::subset_states, py::arg("trs"))
            .def("subset_states_any_ordering", &CondorcetDomain::subset_states_any_ordering, py::arg("trs"))
            .def("subset_cd_sizes", &CondorcetDomain::subset_cd_sizes, py::arg("trs"))

            .def("inverse_domain", &CondorcetDomain::inverse_domain, py::arg("cd"), py::arg("permutation"))
            .def("isomorphic_domains", &CondorcetDomain::isomorphic_domains, py::arg("cd"))
            .def("isomorphic_hash", &CondorcetDomain::isomorphic_hash, py::arg("cd"))
            .def("non_isomorphic_domains", &CondorcetDomain::non_isomorphic_domains, py::arg("cds"))
            .def("is_trs_isomorphic", &CondorcetDomain::is_trs_isomorphic, py::arg("trs_rz"), py::arg("rules"))
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
                                              cd.m_subsets,
                                              cd.m_subset_dicts);
                    },
                    [](py::tuple t)
                    {
                        if (t.size() != 9)
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
                        cd.m_subsets = t[7].cast<std::vector<std::vector<Int8>>>();
                        cd.m_subset_dicts = t[8].cast<std::vector<std::map<Int8, Int8>>>();

                        return cd;
                    }
            ));

    m.def("print_trs", [](const TRS& trs) {
        py::scoped_ostream_redirect stream(
                std::cout,
                py::module_::import("sys").attr("stdout")
        );
        print_trs(trs);
    }, py::arg("trs"));


    // Forbidden Permutations
    py::class_<TupleLaws>(m, "TupleLaws")
        .def(py::init<>())
        .def_readwrite("tuple", &TupleLaws::tuple)
        .def_readwrite("laws", &TupleLaws::laws)
        .def(py::pickle(
                [](const TupleLaws& tl)
                {
                    return py::make_tuple(tl.tuple, tl.laws);
                },
                [](py::tuple t)
                {
                    if (t.size() != 2)
                        throw std::runtime_error("Invalid state for tripleLaws object!");

                    TupleLaws tl{};
                    tl.tuple = t[0].cast<Tuple>();
                    tl.laws = t[1].cast<Laws>();

                    return tl;
                }
        ));

    py::class_<ForbiddenPermutation>(m, "ForbiddenPermutation")
        .def(py::init<Int8, Int8>(), py::arg("n"), py::arg("k"))
        .def_readonly("n", &ForbiddenPermutation::n)
        .def_readonly("k", &ForbiddenPermutation::k)
        .def_readonly("num_tuples", &ForbiddenPermutation::m_num_tuples)
        .def_readonly("alternatives", &ForbiddenPermutation::m_alternatives)
        .def_readonly("tuple_index", &ForbiddenPermutation::m_tuple_index)

        .def("init_tls", &ForbiddenPermutation::init_tls)
        .def("init_tls_by_scheme", &ForbiddenPermutation::init_tls_by_scheme)
        .def("assign_laws", &ForbiddenPermutation::assign_laws, py::arg("tls"), py::arg("tuple"), py::arg("laws"))
        .def("assign_laws_by_index", &ForbiddenPermutation::assign_laws_by_index, py::arg("tls"), py::arg("index"), py::arg("laws"))
        .def("domain", &ForbiddenPermutation::domain, py::arg("tls"))
        .def("size", &ForbiddenPermutation::size, py::arg("tls"))

        .def(py::pickle(
                [](const ForbiddenPermutation& fp)
                {
                    return py::make_tuple(fp.n, fp.k, fp.m_tuple_index, fp.m_alternatives, fp.m_num_tuples);
                },
                [](py::tuple t)
                {
                    if (t.size() != 5)
                        throw std::runtime_error("Invalid state for ForbiddenPermutation object!");

                    ForbiddenPermutation fp(t[0].cast<Int8>(), t[1].cast<Int8>());
                    fp.m_tuple_index = t[2].cast<std::map<Tuple, Int32>>();
                    fp.m_alternatives = t[3].cast<std::vector<Int8>>();
                    fp.m_num_tuples = t[3].cast<Int8>();
                    return fp;
                }
        ));

    // print function
    m.def("print_tls", [](const TLS& tls) {
        py::scoped_ostream_redirect stream(
                std::cout,
                py::module_::import("sys").attr("stdout")
        );
        print_tls(tls);
        }, py::arg("tls"));

    m.def("benchmark_size", &benchmark_size, py::arg("n"));
    m.def("Fishburn_scheme", &Fishburn_scheme, py::arg("triple"));

}