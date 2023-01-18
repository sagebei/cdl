#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <random>

typedef std::array<int, 3> Triplet;

struct TripletRule
{
    Triplet triplet;
    std::string rule;
};

struct RuleScheme
{
    std::vector<std::vector<int>> numbers;
    std::vector<std::string> rules;

    void add(const std::vector<int>& number, const std::string& rule)
    {
        numbers.push_back(number);
        rules.push_back(rule);
    }
};

typedef std::list<TripletRule> TRS;
typedef std::list<std::list<int>> CD;
typedef std::vector<CD> CDS;

class CondorcetDomain
{
private:

    void sort_trs(TRS& trs);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    void filter_trs(TRS& trs, const std::list<int>& elem);
    TRS fetch_trs(const TRS& trs, int i);

public:
    int n;
    std::array<std::string, 4> rules;
    int num_triplets;
    std::vector<int> triplet_elems;

    CondorcetDomain(int n=8);

    // creating and manipulating TRS
    TRS init_empty(bool is_sorted=false);
    TRS init_random(bool is_sorted=false);
    TRS init_by_scheme(const RuleScheme& scheme, bool is_sorted=false);
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

    std::vector<int> subset_weights(int sub_n);
    std::vector<TRS> subset_trss(const TRS& trs, int sub_n);
    std::vector<std::vector<int>> subset_states(const TRS& trs, int sub_n);
    std::tuple<std::vector<TRS>, std::vector<std::size_t>> subset_cd_sizes(const TRS& trs, int sub_n);
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


