#include "condorcet_domain.h"
#include "utils.h"

CondorcetDomain::CondorcetDomain(int n)
{
    this->n = n;
    rules = {"1N3", "3N1", "2N3", "2N1"};
    num_triplets = factorial(n) / (factorial(n - 3) * 6);
    for (int i = 1; i <=n; i ++)
        triplet_elems.push_back(i);
}

void CondorcetDomain::sort_triplet_rules(TRS& triplet_rules)
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

void CondorcetDomain::filter_cd(const TripletRule& tr, CD& cd)
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

void CondorcetDomain::filter_trs(TRS& trs, const std::list<int>& elem)
{
    auto iter = trs.begin();
    while(iter != trs.end())
    {
        TripletRule& tr = *iter;
        const auto& [first, second, third] = tr.triplet;
        const std::string& rule = tr.rule;

        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule == "1N3" && first_index > second_index && first_index > third_index)   ||
            (rule == "3N1" && third_index < first_index && third_index < second_index)   ||
            (rule == "2N3" && second_index > first_index && second_index > third_index)  ||
            (rule == "2N1" && second_index < first_index && second_index < third_index))

        {
            iter = trs.erase(iter);
        }
        else
            iter ++;
    }
}


void CondorcetDomain::expand_cd(CD& cd, int& value)
{
    CD updated_cd;
    for (auto& elem: cd)
    {
        auto iter_elem = elem.begin();
        for (int i = 0; i <= elem.size(); i ++)
        {
            iter_elem = elem.insert(iter_elem, value);
            std::list<int> updated_elem(elem);
            updated_cd.push_back(updated_elem);
            iter_elem = elem.erase(iter_elem);
            iter_elem ++;
        }
    }
    cd = updated_cd;
}

TRS CondorcetDomain::fetch_triplet_rules(TRS& trs, int i)
{
    TRS fetched_trs;
    for (const auto& tr: trs)
    {
        if (tr.triplet[2] == i)
            fetched_trs.push_back(tr);
    }
    return fetched_trs;
}

TRS CondorcetDomain::init(bool sort)
{
    TRS triplet_rules;

    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                tr.rule = "";
                triplet_rules.push_back(tr);
            }
        }
    }
    if(sort)
        sort_triplet_rules(triplet_rules);

    return triplet_rules;
}

TRS CondorcetDomain::init_by_scheme(RuleScheme& scheme) const
{
    unsigned long size = scheme.numbers.size();
    TRS triplet_rules;

    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                for (int s = 0; s < size; s ++)
                {
                    std::vector<int>& number = scheme.numbers[s];
                    if (std::find(number.begin(), number.end(), j) != number.end())
                    {
                        tr.rule = scheme.rules[s];
                        break;
                    }

                }
                triplet_rules.push_back(tr);
            }
        }
    }
    return triplet_rules;
}

CD CondorcetDomain::condorcet_domain(TRS& trs)
{
    CD cd = {{1, 2}, {2, 1}};
    for (int i = 3; i <= n; i++) {
        expand_cd(cd, i);
        TRS fetched_trs = fetch_triplet_rules(trs, i);
        for (const TripletRule& tr: fetched_trs) {
            filter_cd(tr, cd);
        }
    }

    return cd;
}

std::vector<std::size_t> CondorcetDomain::subset_cd_sizes(TRS& trs, int sub_n)
{
    CondorcetDomain cd = CondorcetDomain(sub_n);
    std::vector<std::vector<int>> subsets = combinations(triplet_elems, sub_n);

    std::vector<std::size_t> sizes;
    for (std::vector<int>& subset: subsets)  // for each subset
    {
        TRS sub_trs;
        std::map<int, int> dict;
        for (auto& tr: trs)  // find the triplet that is contained in the subset
        {
            int counter = 0;
            for (int i = 0; i < subset.size(); i ++)
            {
                int value = subset[i];
                dict[value] = i + 1;
                std::array<int, 3> triplet = tr.triplet;
                if (std::find(std::begin(triplet), std::end(triplet), value) != std::end(triplet))
                    counter += 1;
            }
            if (counter == 3)  // if all the element in the triplet is present in the subset
            {
                TripletRule sub_tr;
                sub_tr.rule = tr.rule;
                for (int i = 0; i < tr.triplet.size(); i ++)
                    sub_tr.triplet[i] = dict[tr.triplet[i]];
                sub_trs.push_back(sub_tr);
            }
        }

        for (auto it = dict.begin(); it != dict.end(); it++)
            std::cout << it->first << ":" << it->second << " ";
        std::cout << std::endl;
        print_trs(sub_trs);
        std::size_t sub_size = cd.condorcet_domain(sub_trs).size();
        sizes.push_back(sub_size);
    }

    return sizes;
}

std::size_t CondorcetDomain::hash_cd(CD& cd)
{
    std::size_t seed = 0;
    sort_cd(cd);
    auto iter = cd.begin();

    for (int i =0; i < cd.size(); i ++)
    {
        std::list<int>& elem = *iter;
        for (int j = 0; j < elem.size(); j++)
        {
            seed += (*std::next(elem.begin(), j) * (i + 123) * (j + 456));
        }
        iter = std::next(iter, 1);
    }

    return seed;
}

std::size_t CondorcetDomain::hash_cd_brothers(CDS &cds)
{
    std::size_t seed = 0;
    for (auto& cd: cds)
    {
        for (int i =0; i < cd.size(); i ++)
        {
            std::list<int>& elem = *std::next(cd.begin(), i);
            for (int j = 0; j < elem.size(); j++)
            {
                seed += (*std::next(elem.begin(), j) * (i + 123) * (j + 678));
            }
        }
    }

    return seed;
}

void CondorcetDomain::sort_cd(CD& cd)
{
    cd.sort(compare_list);
}

CDS CondorcetDomain::cd_brothers(const CD& cd)
{
    CDS cds;
    std::vector<int> seeds;

    for (const auto& elem: cd)
    {
        std::map<int, int> dict;
        CD new_cd;
        for (int i = 0; i < elem.size(); i ++)
            dict[*std::next(elem.begin(), i)] = (i + 1);

        for (const auto& e: cd)
        {
            std::list<int> entry;
            for (const int& v: e)
            {
                entry.push_back(dict[v]);
            }
            new_cd.push_back(entry);
        }

        int seed = hash_cd(new_cd);
        if (std::find(seeds.begin(), seeds.end(), seed) == seeds.end())
        {
            seeds.push_back(seed);
            cds.push_back(new_cd);
        }

    }

    return cds;
}

TRS CondorcetDomain::cd_to_trs(const CD &cd)
{
    TRS all_trs;
    TRS trs = init();
    for (auto tr: trs)
    {
        for (std::string rule: rules)
        {
            tr.rule = rule;
            all_trs.push_back(tr);
        }
    }
    for (const std::list<int>& elem : cd)
    {
        filter_trs(all_trs, elem);
    }

    return all_trs;

}

TRS& CondorcetDomain::assign_rule(TRS& trs, int index, std::string rule)
{
    TripletRule& tr = *std::next(trs.begin(), index);
    tr.rule = rule;
    return trs;
}

void CondorcetDomain::print_trs(const TRS& trs)
{
    for (auto const& tr: trs)
    {
        std::cout<<tr.triplet[0]<< tr.triplet[1] << tr.triplet[2] << " : " << tr.rule << std::endl;
    }
    std::cout << std::endl;
}

void CondorcetDomain::print_cd(const CD& cd)
{
    for (auto& elem: cd)
    {
        for (const int& e: elem)
            std::cout << e;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}









