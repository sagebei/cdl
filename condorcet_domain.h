#ifndef INSPIRATION_CONDORCET_DOMAIN_H
#define INSPIRATION_CONDORCET_DOMAIN_H

#endif

#include <array>
#include <iostream>
#include <vector>
#include <forward_list>
#include <list>
#include <algorithm>

struct TripletRule
{
    std::array<int, 3> triplet;
    std::string rule;
};

typedef std::vector<TripletRule> TRS;
typedef std::list<std::list<int>> CD;

class CondorcetDomain
{
private:
    int n = 5;
    void sort_triplet_rules(TRS& triplet_rules);
    void filter_cd(const TripletRule& tr, CD& cd);
    void expand_cd(CD& cd, int& value);
    TRS fetch_triplet_rules(TRS& trs, int i);
    int get_index(const std::list<int>& elem, const int& value);


public:
    CondorcetDomain(int n);

    TRS initialize(bool sort);

    TRS initialize_by_scheme();

    unsigned long condorcet_domain(TRS& trs);

    void change_rule(TRS& triplet_rules, int index, int label);

    void print_tr(TRS& trs);

    void print_cd(CD& cd);

};


