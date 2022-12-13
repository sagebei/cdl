#include "condorcet_domain.h"

#include <utility>
#include "utils.h"

CondorcetDomain::CondorcetDomain(int n)
{
    this->n = n;
    rules = {"1N3", "3N1", "2N3", "2N1"};
    num_triplets = factorial(n) / (factorial(n - 3) * 6);
    for (int i = 1; i <=n; i ++)
        triplet_elems.push_back(i);
}

void CondorcetDomain::sort_trs(TRS& trs)
{
    auto start = trs.begin();
    auto end = trs.end();

    TRS sorted_trs;
    while(true)
    {
        sorted_trs.push_back(*start);
        start++;
        if(start == end) break;

        end--;
        sorted_trs.push_back(*end);
        if(start == end) break;
    }
    trs = sorted_trs;
}

void CondorcetDomain::filter_cd(const TripletRule& tr, CD& cd)
{
    const int& first = tr.triplet[0], second = tr.triplet[1], third = tr.triplet[2];
    const std::string& rule = tr.rule;

    cd.remove_if([&](const std::list<int>& elem) {
        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule == "1N3" && first_index > second_index && first_index > third_index)   ||
            (rule == "3N1" && third_index < first_index && third_index < second_index)   ||
            (rule == "2N3" && second_index > first_index && second_index > third_index)  ||
            (rule == "2N1" && second_index < first_index && second_index < third_index))
            return true;
        else
            return false;
    });

}

void CondorcetDomain::filter_trs(TRS& trs, const std::list<int>& elem)
{
    trs.remove_if([&](const TripletRule& tr){
        const int& first = tr.triplet[0], second = tr.triplet[1], third = tr.triplet[2];
        const std::string& rule = tr.rule;

        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule == "1N3" && first_index > second_index && first_index > third_index)   ||
            (rule == "3N1" && third_index < first_index && third_index < second_index)   ||
            (rule == "2N3" && second_index > first_index && second_index > third_index)  ||
            (rule == "2N1" && second_index < first_index && second_index < third_index))
            return true;
        else
            return false;
    });
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

TRS CondorcetDomain::fetch_trs(const TRS& trs, int i)
{
    TRS fetched_trs;
    for (const auto& tr: trs)
    {
        if (tr.triplet[2] == i)
            fetched_trs.push_back(tr);
    }
    return fetched_trs;
}

TRS CondorcetDomain::init_empty(bool is_sorted)
{
    TRS trs;
    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                tr.rule = "";
                trs.push_back(tr);
            }
        }
    }

    if (is_sorted)
        sort_trs(trs);

    return trs;
}

TRS CondorcetDomain::init_by_scheme(const RuleScheme& scheme, bool is_sorted)
{
    std::size_t size = scheme.numbers.size();
    TRS trs;

    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                for (int s = 0; s < size; s++)
                {
                    const std::vector<int>& number = scheme.numbers[s];
                    if (std::find(number.begin(), number.end(), j) != number.end())
                    {
                        tr.rule = scheme.rules[s];
                        break;
                    }

                }
                trs.push_back(tr);
            }
        }
    }

    if (is_sorted)
        sort_trs(trs);

    return trs;
}

TRS CondorcetDomain::assign(TRS trs, const Triplet& triplet, const std::string& rule)
{
    for (TripletRule& tr: trs)
    {
        if (tr.triplet == triplet)
            tr.rule = rule;
    }
    return trs;
}

TRS CondorcetDomain::assign_by_index(TRS trs, int index, const std::string& rule)
{
    auto tr = std::next(trs.begin(), index);
    tr->rule = rule;
    return trs;
}

std::vector<Triplet> CondorcetDomain::unassigned_triplets(const TRS& trs)
{
    std::vector<Triplet> unassigned{};
    for (const TripletRule& tr: trs)
    {
        if (tr.rule.empty())
            unassigned.push_back(tr.triplet);
    }
    return unassigned;
}

std::vector<std::size_t> CondorcetDomain::evaluate_rules_on_triplet(const TRS& trs, Triplet triplet)
{
    std::vector<std::size_t> sizes = {};
    for (std::string& rule: rules)
    {
        TRS new_trs = assign(trs, triplet, rule);
        std::size_t s = condorcet_domain(new_trs).size();
        sizes.push_back(s);
    }
    return sizes;
}

Triplet CondorcetDomain::dynamic_triplet_ordering(const TRS& trs)
{
    std::map<std::size_t, Triplet> size_triplet;

    std::vector<Triplet> unassigned = unassigned_triplets(trs);
    if (unassigned.empty())
        return Triplet{0, 0, 0};

    for (const Triplet& unassigned_triplet : unassigned)
    {
        auto sizes = evaluate_rules_on_triplet(trs, unassigned_triplet);
        auto max_size = *std::max_element(sizes.begin(), sizes.end());
        size_triplet[max_size] = unassigned_triplet;
    }
    const auto max_size_triplet_ptr = std::min_element(std::begin(size_triplet), std::end(size_triplet),
                                                           [](const std::pair<std::size_t, Triplet>& p1, const std::pair<std::size_t, Triplet>& p2)
                                                                 {
                                                                     return p1.first < p2.first;
                                                                 });

    return max_size_triplet_ptr->second;

}

// [1, 2, 3, 5, 7] subset must be ordered
TRS CondorcetDomain::transfer_trs(const TRS& large, const TRS& small, const std::vector<int>& subset)
{
    TRS transferred_trs = large;
    std::map<int, int> dict;
    for (int i = 0; i < subset.size(); i++)
        dict[i + 1] = subset[i];

    for (const TripletRule& s: small)
    {
        Triplet mapped_triplet = {dict[s.triplet[0]], dict[s.triplet[1]], dict[s.triplet[2]]};
        transferred_trs = assign(transferred_trs, mapped_triplet, s.rule);
    }
    return transferred_trs;
}

std::vector<int> CondorcetDomain::trs_to_state(const TRS& trs)
{
    std::vector<int> state;
    for (const TripletRule& tr: trs)
    {
        const std::string& rule = tr.rule;
        if (rule.empty())
            state.push_back(0);
        else if (rule == rules[0])
            state.push_back(1);
        else if (rule == rules[1])
            state.push_back(2);
        else if (rule == rules[2])
            state.push_back(3);
        else if (rule == rules[3])
            state.push_back(4);
    }
    return state;
}

CD CondorcetDomain::condorcet_domain(const TRS& trs)
{
    CD cd = {{1, 2}, {2, 1}};
    for (int i = 3; i <= n; i++) {
        expand_cd(cd, i);
        TRS fetched_trs = fetch_trs(trs, i);
        for (const TripletRule& tr: fetched_trs) {
            filter_cd(tr, cd);
        }
    }

    return cd;
}

std::tuple<std::vector<std::vector<int>>, std::vector<std::size_t>>  CondorcetDomain::subset_cd_sizes(const TRS& trs, int sub_n)
{
    CondorcetDomain cd = CondorcetDomain(sub_n);
    std::vector<std::vector<int>> subsets = combinations(triplet_elems, sub_n);

    std::vector<std::size_t> sizes;
    for (const std::vector<int>& subset: subsets)  // for each subset
    {
        TRS sub_trs;
        std::map<int, int> dict;
        for (auto& tr: trs)  // find the triplet that is contained in the subset
        {
            int counter = 0;
            for (int i = 0; i < subset.size(); i++)
            {
                int value = subset[i];
                dict[value] = i + 1;
                Triplet triplet = tr.triplet;
                if (std::find(std::begin(triplet), std::end(triplet), value) != std::end(triplet))
                    counter += 1;
            }
            if (counter == 3)  // if all the element in the triplet is present in the subset
            {
                TripletRule sub_tr;
                sub_tr.rule = tr.rule;
                for (int i = 0; i < tr.triplet.size(); i++)
                    sub_tr.triplet[i] = dict[tr.triplet[i]];
                sub_trs.push_back(sub_tr);
            }
        }

        std::size_t sub_size = cd.condorcet_domain(sub_trs).size();
        sizes.push_back(sub_size);
    }

    return std::make_tuple(subsets, sizes);
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

CDS CondorcetDomain::domain_brothers(const CD& cd)
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

TRS CondorcetDomain::domain_to_trs(const CD &cd, bool is_sorted)
{
    TRS all_trs;
    TRS trs = init_empty(is_sorted);
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

void print_trs(const TRS& trs)
{
    for (auto const& tr: trs)
    {
        std::cout<<tr.triplet[0]<< tr.triplet[1] << tr.triplet[2] << " : " << tr.rule << std::endl;
    }
    std::cout << std::endl;
}

void print_cd(const CD& cd)
{
    for (auto& elem: cd)
    {
        for (const int& e: elem)
            std::cout << e;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}









