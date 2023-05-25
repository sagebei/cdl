#pragma once

#include <iostream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <utility>
#include "utils.h"

struct TripletRule
{
    Triplet triplet;
    Int8 rule_id;
};

typedef std::vector<TripletRule> TRS;
typedef std::map<Triplet, Int32> TripletIndex;

typedef std::list<IntList> CD;
typedef std::vector<CD> CDS;

class CondorcetDomain
{
private:
    void sort_trs(TRS& trs);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, Int8& value);
    void filter_trs_list(std::list<TripletRule>& trs, const IntList& elem);
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

    Int32 m_num_triplets{};
    std::vector<Int8> m_triplet_elems{};
    TripletIndex m_triplet_index{};

    // member variable for constructing subsets
    Int8 m_sub_n{};
    Int32 m_subset_size{};
    std::vector<std::vector<Int8>> m_subsets{};
    std::vector<std::map<Int8, Int8>> m_subset_dicts{};

    CondorcetDomain(Int8 n=8);

    // creating and manipulating TRS
    void build_triplet_index(const TRS& trs);
    TRS init_random(bool is_sorted=false);
    TRS init_trs(const std::string& rule="");
    TRS init_by_scheme(const std::function<std::string(Triplet)>& scheme_fun);

    TRS clear_trs(TRS trs);
    TRS shuffle_trs(TRS trs, int seed=0);
    TRS transfer_trs(const TRS& from, TRS to);

    TRS assign_id(TRS trs, const Triplet& triplet, const Int8 rule_id);
    TRS assign_rule(TRS trs, const Triplet& triplet, const std::string& rule);
    TRS assign_id_by_index(TRS trs, Int32 index, const Int8 rule_id);
    TRS assign_rule_by_index(TRS trs, Int32 index, const std::string& rule);
    std::vector<Triplet> unassigned_triplets(const TRS& trs);
    std::vector<Triplet> assigned_triplets(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triplet(const TRS& trs, const Triplet& triplet);
    Triplet dynamic_triplet_ordering(const TRS& trs);
    std::vector<Int8> trs_to_state(const TRS& trs);
    TRS state_to_trs(const std::vector<Int8>& state);
    TRS uplift_trs(const TRS& large, const TRS& small, const std::vector<Int8>& subset);

    // manipulating CDs
    CD condorcet_domain(const TRS& trs);
    std::size_t size(const TRS& trs);

    // subset functions
    void init_subset(Int8 sub_n);
    std::vector<int> subset_weights();
    std::vector<TRS> subset_trs_list(const TRS& trs);
    std::vector<std::vector<Int8>> subset_states(const TRS& trs);
    std::vector<std::vector<Int8>> subset_states_any_ordering(const TRS& trs);
    std::tuple<std::vector<TRS>, std::vector<std::size_t>> subset_cd_sizes(const TRS& trs);

    // cd brothers
    std::size_t hash_cd(CD& cd);
    std::size_t hash_cd_brothers(CDS& cds);
    void sort_cd(CD& cd);
    CDS domain_brothers(const CD& cd);
    TRS domain_to_trs(const CD& cd, bool is_sorted);

    // enable default copy construction and copy assignment
    CondorcetDomain(const CondorcetDomain& cd) = default;
    CondorcetDomain& operator=(const CondorcetDomain& cd) = default;
};

// printing functions
void print_trs(const TRS& trs);
void print_cd(const CD& cd);


