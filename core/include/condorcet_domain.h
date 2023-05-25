#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <utility>
#include "utils.h"

typedef std::array<int, 3> Triplet;

struct TripletRule
{
    Triplet triplet;
    int rule_id;
};

typedef std::vector<TripletRule> TRS;
//typedef std::unordered_map<Triplet, int, TripletHasher> TripletIndex;
typedef std::map<Triplet, int> TripletIndex;

typedef std::list<std::list<int>> CD;
typedef std::vector<CD> CDS;

class CondorcetDomain
{
private:

    void sort_trs(TRS& trs);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    void filter_trs_list(std::list<TripletRule>& trs, const std::list<int>& elem);
    TRS fetch_trs(const TRS& trs, int i);

public:
    int n{};
    std::array<std::string, 6> m_rules{{"1N3", "3N1", "2N3", "2N1", "1N2", "3N2"}};
    std::map<std::string, int> m_rule_id{{"", 0},
                                         {m_rules[0], 1},
                                         {m_rules[1], 2},
                                         {m_rules[2], 3},
                                         {m_rules[3], 4},
                                         {m_rules[4], 5},
                                         {m_rules[5], 6}};

    int m_num_triplets{};
    std::vector<int> m_triplet_elems{};
    TripletIndex m_triplet_index{};

    // member variable for build subsets
    int m_sub_n{};
    int m_subset_size{};
    std::vector<std::vector<int>> m_subsets{};
    std::vector<std::map<int, int>> m_subset_dicts{};

    CondorcetDomain(int n=8);

    // creating and manipulating TRS
    void build_triplet_index(const TRS& trs);
    TRS init_random(bool is_sorted=false);
    TRS init_trs(const std::string& rule="");
    TRS init_by_scheme(const std::function<std::string(Triplet)>& scheme_fun);

    TRS clear_trs(TRS trs);
    TRS shuffle_trs(TRS trs, int seed=0);
    TRS transfer_trs(const TRS& from, TRS to);

    TRS assign_id(TRS trs, const Triplet& triplet, const int rule_id);
    TRS assign_rule(TRS trs, const Triplet& triplet, const std::string& rule);
    TRS assign_id_by_index(TRS trs, int index, const int rule_id);
    TRS assign_rule_by_index(TRS trs, int index, const std::string& rule);
    std::vector<Triplet> unassigned_triplets(const TRS& trs);
    std::vector<Triplet> assigned_triplets(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triplet(const TRS& trs, const Triplet& triplet);
    Triplet dynamic_triplet_ordering(const TRS& trs);
    std::vector<int> trs_to_state(const TRS& trs);
    TRS state_to_trs(const std::vector<int>& state);
    TRS uplift_trs(const TRS& large, const TRS& small, const std::vector<int>& subset);

    // manipulating CDs
    CD condorcet_domain(const TRS& trs);
    std::size_t size(const TRS& trs);

    // subset functions
    void init_subset(int sub_n);
    std::vector<int> subset_weights();
    std::vector<TRS> subset_trs_list(const TRS& trs);
    std::vector<std::vector<int>> subset_states(const TRS& trs);
    std::vector<std::vector<int>> subset_states_any_ordering(const TRS& trs);
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


