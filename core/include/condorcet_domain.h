#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <list>
#include <map>
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
    std::string rule;
};

typedef std::vector<TripletRule> TRS;
typedef std::map<Triplet, int> TripletIndex;

typedef std::list<std::list<int>> CD;
typedef std::vector<CD> CDS;

class CondorcetDomain
{
private:

    void sort_trs(TRS& trs);
    void build_triplet_index(const TRS& trs);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    void filter_trs_list(std::list<TripletRule>& trs, const std::list<int>& elem);
    TRS fetch_trs(const TRS& trs, int i);

public:
    int n;
    std::array<std::string, 4> rules{};
    int num_triplets;
    std::vector<int> triplet_elems{};
    TripletIndex triplet_index{};

    // member variable for build subsets
    int sub_n{};
    int subset_size{};
    std::vector<std::vector<int>> subsets{};
    std::vector<std::map<int, int>> subset_dicts{};

    CondorcetDomain(int n=8);

    // creating and manipulating TRS
    TRS init_random(bool is_sorted=false);
    TRS init_trs(std::string rule="");
    TRS init_by_scheme(const std::function<std::string(Triplet)>& scheme_fun);

    TRS clear_trs(TRS trs);
    TRS shuffle_trs(TRS trs, int seed=0);
    TRS transfer_trs(const TRS& from, TRS to);

    TRS assign(TRS trs, const Triplet& triplet, const std::string& rule);
    TRS assign_by_index(TRS trs, int index, const std::string& rule);
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


