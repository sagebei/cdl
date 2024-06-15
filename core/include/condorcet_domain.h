#pragma once
#include "utils.h"


class CondorcetDomain
{
private:
    void filter_cd(const TripleRule& tr, CD& cd);
    void expand_cd(CD& cd, Int8& value);
    bool check_permutation(const IntList& permutation, const TRS& trs);
    void expand_permutation(IntList& permutation, const TRS& trs, Int8 alternative, std::size_t& cd_size, std::size_t& threshold, bool& reach_threshold);
    void filter_trs_list(std::list<TripleRule>& trs, const IntList& elem);
    TRS fetch_trs(const TRS& trs, Int8 i);

public:
    Int8 n{};
    std::vector<std::string> m_rules{{"1N3", "3N1", "2N3", "2N1", "1N2", "3N2"}};
    std::map<std::string, Int8> m_rule_id{{"", 0},
                                         {m_rules[0], 1},
                                         {m_rules[1], 2},
                                         {m_rules[2], 3},
                                         {m_rules[3], 4},
                                         {m_rules[4], 5},
                                         {m_rules[5], 6}};

    Int32 m_num_triples{};
    std::vector<Int8> m_triple_elems{};
    TripleIndex m_triple_index{};

    // member variable for constructing subsets
    Int8 m_sub_n{};
    std::vector<std::vector<Int8>> m_subsets{};
    std::vector<std::map<Int8, Int8>> m_subset_dicts{};

    CondorcetDomain(Int8 n=8);

    // creating and manipulating TRS
    void build_triple_index(const TRS& trs);
    TRS sort_trs(TRS trs);
    TRS init_trs_random(bool is_sorted=false);
    TRS init_trs();
    TRS trs_to_rz_ordering(const TRS& trs);
    TRS init_trs_lex();
    TRS init_trs_colex();
    TRS init_trs_by_scheme(TRS trs, const std::function<std::string(Triple)>& scheme_fun);

    TRS clear_trs(TRS trs);
    TRS shuffle_trs(TRS trs, int seed=0);
    TRS transfer_trs(const TRS& from, TRS to);

    TRS assign_id(TRS trs, const Triple& triple, const Int8 rule_id);
    TRS assign_id_any_ordering(TRS trs, const Triple& triple, const Int8 rule_id);
    TRS assign_rule(TRS trs, const Triple& triple, const std::string& rule);
    TRS assign_rule_any_ordering(TRS trs, const Triple& triple, const std::string& rule);
    TRS assign_id_by_index(TRS trs, Int32 index, const Int8 rule_id);
    TRS assign_rule_by_index(TRS trs, Int32 index, const std::string& rule);
    Triple next_unassigned_triple(const TRS& trs);
    std::vector<Triple> unassigned_triples(const TRS& trs);
    std::vector<Triple> assigned_triples(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triple(const TRS& trs, const Triple& triple);
    Triple dynamic_triple_ordering(const TRS& trs);
    std::vector<Int8> trs_to_state(const TRS& trs);
    TRS state_to_trs(TRS trs, const std::vector<Int8>& state);
    TRS uplift_trs(const TRS& large, const TRS& small, const std::vector<Int8>& subset);

    // manipulating CDs
    CD domain(const TRS& trs);  // Width-First
    std::size_t size(const TRS& trs, std::size_t threshold=SIZE_MAX);     // Depth-First; SIZE_MAX=18446744073709551615
    CD filter_domain(const CD& domain, const TripleRule& tr);

    // subset functions
    void init_subset(Int8 sub_n);
    std::vector<int> subset_weights();
    std::vector<TRS> subset_trs_list(const TRS& trs);
    CDS subset_domain_list(const CD& cd);
    CD domain_on_alternatives(const CD& domain, const std::vector<Int8>& alternatives);
    CD domain_on_triple(const CD& cd, const Triple& triple);
    std::vector<std::vector<Int8>> subset_states(const TRS& trs);
    std::vector<std::vector<Int8>> subset_states_any_ordering(const TRS& trs);
    std::tuple<std::vector<TRS>, std::vector<std::size_t>> subset_cd_sizes(const TRS& trs);

    // Isomorphism
    CD inverse_domain(const CD& cd, const IntList& permutation);
    CDS isomorphic_domains(const CD& cd);
    CD isomorphic_hash(const CD& cd);
    CDS non_isomorphic_domains(CDS cds);
    TRS inverse_trs(const TRS& trs, const IntList& permutation, const std::vector<std::string>& rules);
    bool is_trs_isomorphic(const TRS& trs, const std::vector<std::string>& rules);
    bool is_trs_isomorphic_1n33n1(const TRS& trs);
    std::vector<TRS> isomorphic_trs_list(const TRS& trs, const std::vector<std::string>& rules);

    TRS domain_to_trs(const CD& cd);

    // cd types
    bool is_domain_copious(const TRS& trs, const CD& domain);
//    bool is_domain_rich(const CD& domain, int richness);
    bool is_domain_ample(const CD& domain);
    bool domain_include(const CD& domain, const IntList& permutation);
    bool is_domain_unitary(const CD& domain);
    bool is_domain_maximal_width(const CD& domain);
    CD dual_domain(const CD domain);
    float trs_core(const TRS& trs, const CD& domain);

    // enable default copy construction and copy assignment
    CondorcetDomain(const CondorcetDomain& cd) = default;
    CondorcetDomain& operator=(const CondorcetDomain& cd) = default;
};

// printing functions
void print_trs(const TRS& trs);
void print_cd(const CD& cd);


