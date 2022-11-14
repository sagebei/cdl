#include "condorcet_domain.h"

void sort_triplet_rules(TRS& triplet_rules)
{
    auto start = triplet_rules.begin();
    auto end = triplet_rules.end();

    TRS sorted_triplet_rules;
    while(true)
    {
        sorted_triplet_rules.push_back(*start);
        start++;
        if(start == end) break;

        end--;
        sorted_triplet_rules.push_back(*end);
        if(start == end) break;
    }
    triplet_rules = sorted_triplet_rules;
}

TRS initialize(int n=5, bool sort=true)
{
    TRS triplet_rules;
    std::array<std::string, 4> rules = {"1N3", "3N1", "2N3", "2N1"};

    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                tr.rule = rules[1];
                triplet_rules.push_back(tr);
            }
        }
    }
    if(sort)
        sort_triplet_rules(triplet_rules);

    return triplet_rules;
}

void change_rule(TRS& triplet_rules, int index, int label)
{
    std::array<std::string, 4> rules = {"1N3", "3N1", "2N3", "2N1"};
    triplet_rules[index].rule = rules[label];
}

void print_tr(TRS& trs)
{
    for (auto const& tr: trs)
    {
        std::cout<<tr.triplet[0]<< tr.triplet[1] << tr.triplet[2] << " : " << tr.rule << std::endl;
    }
}

void print_cd(CD& cd)
{
    for (auto& elem: cd)
    {
        for (int& e: elem)
            std::cout << e;
        std::cout << std::endl;
    }
}

/*  n=5
 * (1, 2, 3) 3N1
 * (1, 2, 4) 1n3
 *  ...
 * (3, 4, 5) 3N1
 * */
unsigned long condorcet_domain(TRS& trs, int n)
{
    CD cd = {{1, 2}, {2, 1}};
    for (int i = 3; i <= n; i++) {
        expand_cd(cd, i);
//        print_cd(cd);
        TRS fetched_trs = fetch_triplet_rules(trs, i);
        for (const TripletRule& tr: fetched_trs) {
            filter_cd(tr, cd);
        }
    }

    return cd.size();
}

void expand_cd(CD& cd, const int value)
{
    CD updated_cd;
    auto iter_cd = updated_cd.begin();

    for (auto& elem: cd)
    {
        auto iter_elem = elem.begin();
        for (int i = 0; i <= elem.size(); i ++)
        {
            iter_elem = elem.insert(iter_elem, value);
            std::list<int> updated_elem(elem);
            iter_cd = updated_cd.insert(iter_cd, updated_elem);
            iter_elem = elem.erase(iter_elem);
            iter_elem ++;
        }
    }
    cd = updated_cd;
}

/*
 * n=4
 * (1, 2, 3)  "1N2"    (4, 1, 2, 3)
 *                     (1, 4, 2, 3)
 *                     (1, 2, 4, 3)
 *                     (1, 2, 3, 4)
 *
 *                     (1, 3, 2)
 *                     (2, 3, 1)
 *                     (3, 2, 1)
 * */

TRS fetch_triplet_rules(TRS& trs, int i)
{
    TRS fetched_trs;
    for (const auto& tr: trs)
    {
        if (tr.triplet[2] == i)
            fetched_trs.push_back(tr);
    }
    return fetched_trs;
}

int get_index(const std::list<int>& elem, const int& value)
{
    int index = -1;
    int i = 0;

    auto iter = elem.begin();
    while (iter != elem.end())
    {
        if (*iter == value)
        {
            index = i;
            break;
        }

        i ++;
        iter = std::next(iter, 1);
    }
    return index;
}

void filter_cd(const TripletRule& tr, CD& cd)
{
    const auto& [first, second, third] = tr.triplet;
    const std::string& rule = tr.rule;

    auto iter = cd.begin();
    while(iter != cd.end())
    {
        //{"1N3", "3N1", "2N3", "2N1"}
        const std::list<int>& elem = *iter;

        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule == "1N3" && first_index > second_index && first_index > third_index)   ||
            (rule == "3N1" && third_index < first_index && third_index < second_index)   ||
            (rule == "2N3" && second_index > first_index && second_index > third_index)  ||
            (rule == "2N1" && second_index < first_index && second_index < third_index))

        {
            iter = cd.erase(iter);
        }
        else
            iter ++;
    }

}









