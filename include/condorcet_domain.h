#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <map>
#include <algorithm>

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
    int n;
    std::vector<int> triplet_elems;

    void sort_trs(TRS& trs);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    void filter_trs(TRS& trs, const std::list<int>& elem);
    TRS fetch_trs(const TRS& trs, int i);

public:
    std::array<std::string, 4> rules;
    int num_triplets;

    CondorcetDomain(int num=5);

    // creating and manipulating TRS
    TRS init(bool sort=true);
    TRS init_by_scheme(const RuleScheme& scheme) const;
    TRS& assign(TRS& trs, Triplet triplet, std::string rule);
    TRS& assign_by_index(TRS& trs, int index, std::string rule);
    std::vector<std::array<int, 3>> unassigned_triplets(const TRS& trs);
    std::vector<std::size_t> evaluate_rules_on_triplet(TRS trs, Triplet triplet);

    // manipulating CDs
    CD condorcet_domain(const TRS& trs);
    std::vector<std::size_t> subset_cd_sizes(const TRS& trs, int sub_n);
    std::size_t hash_cd(CD& cd);
    std::size_t hash_cd_brothers(CDS& cds);
    void sort_cd(CD& cd);
    CDS domain_brothers(const CD& cd);
    TRS domain_to_trs(const CD& cd);

};

// printing functions
void print_trs(const TRS& trs);
void print_cd(const CD& cd);


