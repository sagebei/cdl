#ifndef INSPIRATION_CONDORCET_DOMAIN_H
#define INSPIRATION_CONDORCET_DOMAIN_H

#endif

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

    void add(std::vector<int> number, std::string rule)
    {
        numbers.push_back(number);
        rules.push_back(rule);
    }
};

typedef std::vector<TripletRule> TRS;
typedef std::list<std::list<int>> CD;
typedef std::vector<CD> CDS;

class CondorcetDomain
{
private:
    int n;
    std::array<std::string, 4> rules;

    void sort_triplet_rules(TRS& triplet_rules);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    TRS fetch_triplet_rules(TRS& trs, int i);
    int get_index(const std::list<int>& elem, const int& value);

public:
    CondorcetDomain(int num);

    TRS initialize(bool sort);
    TRS initialize_by_scheme(RuleScheme& scheme) const;
    CD condorcet_domain(TRS& trs);
    int hash_cd(const CD& cd);
    CDS cd_brothers(const CD& cd);
    TRS cd_to_trs(const CD& cd);
    void change_rule(TRS& triplet_rules, int index, int label);
    void print_trs(const TRS& trs);
    void print_cd(const CD& cd);

};


