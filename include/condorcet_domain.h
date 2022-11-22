#ifndef INSPIRATION_CONDORCET_DOMAIN_H
#define INSPIRATION_CONDORCET_DOMAIN_H

#include <array>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <map>
#include <algorithm>

struct TripletRule
{
    std::array<int, 3> triplet;
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
    void sort_triplet_rules(TRS& triplet_rules);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    void filter_trs(TRS& trs, const std::list<int>& elem);
    TRS fetch_triplet_rules(TRS& trs, int i);

public:
    std::array<std::string, 4> rules;
    int num_triplets;

    CondorcetDomain(int num=5);
    TRS init(bool sort=true);
    TRS init_by_scheme(RuleScheme& scheme) const;
    TRS& assign_rule(TRS& trs, int index, std::string rule);
    CD condorcet_domain(TRS& trs);
    std::size_t hash_cd(CD& cd);
    std::size_t hash_cd_brothers(CDS& cds);
    void sort_cd(CD& cd);
    CDS cd_brothers(const CD& cd);
    TRS cd_to_trs(const CD& cd);

    void print_trs(const TRS& trs);
    void print_cd(const CD& cd);

};

#endif

