#pragma once
#include "utils.h"


class CondorcetDomain
{
private:
    void filter_cd(const TripleRule& tr, CD& cd);
    void expand_cd(CD& cd, Int8& value);
    bool check_permutation(const IntList& permutation, const TRS& trs);
    void expand_permutation(IntList& permutation, const TRS& trs, Int8 alternative, std::size_t& cd_size);
    void filter_trs_list(std::list<TripleRule>& trs, const IntList& elem);
    TRS fetch_trs(const TRS& trs, Int8 i);

public:
    Int8 n{};
    std::array<std::string, 6> m_rules{{"1N3", "3N1", "2N3", "2N1", "1N2", "3N2"}};
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
    TRS init_trs_lex();
    TRS init_trs_colex();
    TRS init_trs_by_scheme(const std::function<std::string(Triple)>& scheme_fun);

    TRS clear_trs(TRS trs);
    TRS shuffle_trs(TRS trs, int seed=0);
    TRS transfer_trs(const TRS& from, TRS to);

    TRS assign_id(TRS trs, const Triple& triple, const Int8 rule_id);
    TRS assign_rule(TRS trs, const Triple& triple, const std::string& rule);
    TRS assign_id_by_index(TRS trs, Int32 index, const Int8 rule_id);
    TRS assign_rule_by_index(TRS trs, Int32 index, const std::string& rule);
    Triple next_unassigned_triple(const TRS& trs);
    std::vector<Triple> unassigned_triples(const TRS& trs);
    std::vector<Triple> assigned_triples(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triple(const TRS& trs, const Triple& triple);
    Triple dynamic_triple_ordering(const TRS& trs);
    std::vector<Int8> trs_to_state(const TRS& trs);
    TRS state_to_trs(const std::vector<Int8>& state);
    TRS uplift_trs(const TRS& large, const TRS& small, const std::vector<Int8>& subset);

    // manipulating CDs
    CD domain(const TRS& trs);  // Width-First
    std::size_t size(const TRS& trs);     // Depth-First

    // subset functions
    void init_subset(Int8 sub_n);
    std::vector<int> subset_weights();
    std::vector<TRS> subset_trs_list(const TRS& trs);
    CDS subset_domain_list(const CD& cd);
    CD domain_on_triple(const CD& cd, const Triple& triple);
    std::vector<std::vector<Int8>> subset_states(const TRS& trs);
    std::vector<std::vector<Int8>> subset_states_any_ordering(const TRS& trs);
    std::tuple<std::vector<TRS>, std::vector<std::size_t>> subset_cd_sizes(const TRS& trs);

    // cd brothers
    CD inverse_domain(const CD& cd, const IntList& permutation);
    CDS domain_brothers(const CD& cd);
    CDS isomorphic_domains(const CD& cd);
    CD isomorphic_hash(const CD& cd);
    CDS non_isomorphic_domains(CDS cds);
    bool is_trs_isomorphic(const TRS& trs, const Triple& triple, const std::vector<std::string>& rules);

    TRS domain_to_trs(const CD& cd);

    // enable default copy construction and copy assignment
    CondorcetDomain(const CondorcetDomain& cd) = default;
    CondorcetDomain& operator=(const CondorcetDomain& cd) = default;
};

// printing functions
void print_trs(const TRS& trs);
void print_cd(const CD& cd);


