#include "condorcet_domain.h"

CondorcetDomain::CondorcetDomain(int num=5)
{
    n = num;
    rules = {"1N3", "3N1", "2N3", "2N1"};
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

int CondorcetDomain::get_index(const std::list<int>& elem, const int& value)
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



TRS CondorcetDomain::initialize(bool sort=true)
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
                tr.rule = rules[1];
                triplet_rules.push_back(tr);
            }
        }
    }
    if(sort)
        sort_triplet_rules(triplet_rules);

    return triplet_rules;
}

TRS CondorcetDomain::initialize_by_scheme(RuleScheme& scheme) const
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

int CondorcetDomain::hash_cd(const CD& cd)
{
    int seed = 0;
    for (const auto& elem: cd)
    {
        for (int i = 0; i < elem.size(); i++)
        {
            seed += (*std::next(elem.begin(), i) * (i + n + 1));
        }
    }
    return seed;
}

CDS CondorcetDomain::cd_brothers(const CD& cd)
{
    CDS cds;
    std::vector<std::size_t> seeds;

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
    TRS trs = initialize();


    return trs;

}

void CondorcetDomain::change_rule(TRS& triplet_rules, int index, int label)
{
    std::array<std::string, 4> rules = {"1N3", "3N1", "2N3", "2N1"};
    triplet_rules[index].rule = rules[label];
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









