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

TRS initialize(int, bool);
void sort_triplet_rules(TRS&);
void print_tr(TRS&);
void print_cd(CD& cd);
unsigned long condorcet_domain(TRS& trs, int n);
void expand_cd(CD& cd, int value);
void filter_cd(const TripletRule& tr, CD& cd);
void change_rule(TRS& triplet_rules, int index, int label);
TRS fetch_triplet_rules(TRS& trs, int i);
int get_index(std::list<int>& elem, int& value);


