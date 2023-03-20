#include "condorcet_domain.h"


CondorcetDomain::CondorcetDomain(int n)
{
    this->n = n;
    m_num_triplets = factorial(n) / (factorial(n - 3) * 6);
    for (int i = 1; i <=n; i ++)
        m_triplet_elems.push_back(i);
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

void CondorcetDomain::build_triplet_index(const TRS& trs)
{
    for (int i = 0; i < trs.size(); i ++)
        m_triplet_index[trs[i].triplet] = i;
}

void CondorcetDomain::filter_cd(const TripletRule& tr, CD& cd)
{
    const int& first = tr.triplet[0], second = tr.triplet[1], third = tr.triplet[2];
    const int rule_id = tr.rule_id;

    cd.remove_if([&](const std::list<int>& elem) {
        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule_id == 1 && first_index > second_index && first_index > third_index)   ||
            (rule_id == 2 && third_index < first_index && third_index < second_index)   ||
            (rule_id == 3 && second_index > first_index && second_index > third_index)  ||
            (rule_id == 4 && second_index < first_index && second_index < third_index))
            return true;
        else
            return false;
    });

}

void CondorcetDomain::filter_trs_list(std::list<TripletRule>& trs, const std::list<int>& elem)
{

    trs.remove_if([&](const TripletRule& tr){
        const int& first = tr.triplet[0], second = tr.triplet[1], third = tr.triplet[2];
        const int rule_id = tr.rule_id;

        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule_id == 1 && first_index > second_index && first_index > third_index)   ||
            (rule_id == 2 && third_index < first_index && third_index < second_index)   ||
            (rule_id == 3 && second_index > first_index && second_index > third_index)  ||
            (rule_id == 4 && second_index < first_index && second_index < third_index))
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

TRS CondorcetDomain::init_random(bool is_sorted)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 4);

    TRS trs;
    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {i, j, k};
                tr.rule_id = distrib(gen);
                trs.push_back(tr);
            }
        }
    }

    if (is_sorted)
        sort_trs(trs);

    build_triplet_index(trs);

    return trs;
}

TRS CondorcetDomain::init_trs(const int rule_id)
{
    TRS trs;
    for (int i = 1; i < n+1; i ++)
    {
        for (int k = 1; k < n+1; k ++)
        {
            for (int j = 1; j < n+1; j ++)
            {

                if (i < j && j < k)
                {
                    TripletRule tr;
                    tr.triplet = {i, j, k};
                    tr.rule_id = rule_id;
                    trs.push_back(tr);
                }
            }
        }
    }

    build_triplet_index(trs);
    return trs;
}

TRS CondorcetDomain::init_by_scheme(const std::function<std::string(Triplet)>& scheme_fun)
{
    TRS trs;
    for (int i = 1; i < n+1; i ++)
    {
        for (int k = 1; k < n+1; k ++)
        {
            for (int j = 1; j < n+1; j ++)
            {

                if (i < j && j < k)
                {
                    TripletRule tr;
                    tr.triplet = {i, j, k};
                    tr.rule_id = m_rule_id.at(scheme_fun(tr.triplet));
                    trs.push_back(tr);
                }
            }
        }
    }

    build_triplet_index(trs);
    return trs;
}

TRS CondorcetDomain::clear_trs(TRS trs)
{
    for (TripletRule& tr : trs)
        tr.rule_id = 0;

    return trs;
}

TRS CondorcetDomain::shuffle_trs(TRS trs, int seed)
{
    TRS empty_trs = init_trs();
    empty_trs = transfer_trs(trs, empty_trs);

    std::mt19937_64 urbg(seed);
    std::shuffle(empty_trs.begin(), empty_trs.end(), urbg);

    build_triplet_index(empty_trs);

    return empty_trs;
}

TRS CondorcetDomain::transfer_trs(const TRS& from_trs, TRS to_trs)
{
    for (TripletRule& to_tr : to_trs)
    {
        for (const TripletRule& from_tr : from_trs)
        {
            if (to_tr.triplet == from_tr.triplet)
                to_tr.rule_id = from_tr.rule_id;
        }
    }
    return to_trs;
}

TRS CondorcetDomain::assign_id(TRS trs, const Triplet& triplet, const int rule_id)
{
    int index = m_triplet_index[triplet];
    trs[index].rule_id = rule_id;

    return trs;
}

TRS CondorcetDomain::assign_rule(TRS trs, const Triplet& triplet, const std::string& rule)
{
    int index = m_triplet_index[triplet];
    trs[index].rule_id = m_rule_id.at(rule);

    return trs;
}

TRS CondorcetDomain::assign_id_by_index(TRS trs, int index, const int rule_id)
{
    trs[index].rule_id = rule_id;
    return trs;
}

TRS CondorcetDomain::assign_rule_by_index(TRS trs, int index, const std::string& rule)
{
    trs[index].rule_id = m_rule_id.at(rule);
    return trs;
}

std::vector<Triplet> CondorcetDomain::unassigned_triplets(const TRS& trs)
{
    std::vector<Triplet> unassigned{};
    for (const TripletRule& tr: trs)
    {
        if (tr.rule_id == 0)
            unassigned.push_back(tr.triplet);
    }
    return unassigned;
}

std::vector<Triplet> CondorcetDomain::assigned_triplets(const TRS& trs)
{
    std::vector<Triplet> assigned{};
    for (const TripletRule& tr: trs)
    {
        if (tr.rule_id != 0)
            assigned.push_back(tr.triplet);
    }
    return assigned;
}

std::vector<std::size_t> CondorcetDomain::evaluate_rules_on_triplet(const TRS& trs, const Triplet& triplet)
{
    std::vector<std::size_t> sizes = {};
    for (const std::string& rule: m_rules)
    {
        TRS new_trs = assign_rule(trs, triplet, rule);
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
TRS CondorcetDomain::uplift_trs(const TRS& large, const TRS& small, const std::vector<int>& subset)
{
    TRS uplifted_trs = large;
    std::map<int, int> dict;
    for (int i = 0; i < subset.size(); i++)
        dict[i + 1] = subset[i];

    for (const TripletRule& s: small)
    {
        const auto& [triplet, rule_id] = s;
        Triplet mapped_triplet = {dict[triplet[0]], dict[triplet[1]], dict[triplet[2]]};
        uplifted_trs = assign_id(uplifted_trs, mapped_triplet, rule_id);
    }
    return uplifted_trs;
}

std::vector<int> CondorcetDomain::trs_to_state(const TRS& trs)
{
    std::vector<int> state;
    for (const TripletRule& tr: trs)
        state.push_back(tr.rule_id);

    return state;
}

TRS CondorcetDomain::state_to_trs(const std::vector<int>& state)
{
    TRS trs = init_trs();
    int i = 0;
    for (TripletRule& tr : trs)
    {
        tr.rule_id = state[i];
        i ++;
    }
    return trs;
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

std::size_t CondorcetDomain::size(const TRS& trs)
{
    return condorcet_domain(trs).size();
}

void CondorcetDomain::init_subset(int sub_n)
{
    m_sub_n = sub_n;
    m_subsets = combinations(m_triplet_elems, m_sub_n);
    m_subset_size = m_subsets.size();
    m_subset_dicts.clear();

    for (const std::vector<int>& subset: m_subsets)
    {
        std::map<int, int> dict{};
        for (int i = 0; i < sub_n; i++)  // construct the dictionary
        {
            const int& value = subset[i];
            dict[value] = i + 1;
        }
        m_subset_dicts.push_back(dict);
    }
}

std::vector<int> CondorcetDomain::subset_weights()
{
    std::vector<int> weights{};
    for (const std::vector<int>& subset : m_subsets)
    {
        int weight = 0;
        for (int i = 0; i < subset.size() - 1; i ++)
        {
            weight += subset[i+1] - subset[i];
        }
        weights.push_back(weight);
    }
    return weights;
}

std::vector<TRS> CondorcetDomain::subset_trs_list(const TRS& trs)
{
    std::vector<TRS> trs_list{};

    for (int subset_idx = 0; subset_idx < m_subset_size; subset_idx++)  // for each subset
    {
        TRS sub_trs{};
        for (const TripletRule& tr: trs)  // find the triplet that is contained in the subset
        {
            const auto& [triplet, rule_id] = tr;
            const std::vector<int>& subset = m_subsets[subset_idx];
            if (std::includes(subset.begin(), subset.end(), triplet.begin(), triplet.end()))
            {
                TripletRule sub_triplet_rule;
                for (int i = 0; i < 3; i++)
                    sub_triplet_rule.triplet[i] = m_subset_dicts[subset_idx][triplet[i]];
                sub_triplet_rule.rule_id = rule_id;
                sub_trs.push_back(sub_triplet_rule);
            }
        }
        trs_list.push_back(sub_trs);
    }

    return trs_list;
}

std::vector<std::vector<int>> CondorcetDomain::subset_states(const TRS& trs)
{
    std::vector<std::vector<int>> sub_states{};
    std::vector<TRS> trs_list = subset_trs_list(trs);
    for (const TRS& sub_trs : trs_list)
    {
        std::vector<int> sub_state = trs_to_state(sub_trs);
        sub_states.push_back(sub_state);
    }

    return sub_states;
}

std::vector<std::vector<int>> CondorcetDomain::subset_states_any_ordering(const TRS& trs)
{
    std::vector<std::vector<int>> sub_states{};
    CondorcetDomain sub_cd(m_sub_n);

    std::vector<TRS> trs_list = subset_trs_list(trs);
    for (const TRS& sub_trs : trs_list)
    {
        TRS sub_trs_init = sub_cd.init_trs();
        for (const TripletRule& tr : sub_trs)
        {
            for (const TripletRule& tr_init : sub_trs_init)
            {
                if (tr.triplet == tr_init.triplet)
                    sub_trs_init = sub_cd.assign_id(sub_trs_init, tr.triplet, tr.rule_id);
            }
        }

        std::vector<int> sub_state = trs_to_state(sub_trs_init);
        sub_states.push_back(sub_state);
    }

    return sub_states;
}

std::tuple<std::vector<TRS>, std::vector<std::size_t>> CondorcetDomain::subset_cd_sizes(const TRS& trs)
{
    std::vector<std::size_t> sizes;
    std::vector<TRS> all_trs;

    CondorcetDomain sub_cd(m_sub_n);

    std::vector<TRS> trs_list = subset_trs_list(trs);
    for (const TRS& sub_trs : trs_list)
    {
        std::size_t sub_size = sub_cd.condorcet_domain(sub_trs).size();

        sizes.push_back(sub_size);
        all_trs.push_back(sub_trs);
    }

    return std::make_tuple(all_trs, sizes);
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
    std::list<TripletRule> all_trs;

    TRS trs = init_trs();
    for (TripletRule tr: trs)
    {
        for (const std::string& rule: m_rules)
        {
            tr.rule_id = m_rule_id.at(rule);
            all_trs.push_back(tr);
        }
    }
    for (const std::list<int>& elem : cd)
    {
        filter_trs_list(all_trs, elem);
    }

    TRS result_trs(all_trs.begin(), all_trs.end());
    return result_trs;
}

void print_trs(const TRS& trs)
{
    const std::map<int, std::string> m_id_rule{{0, ""},
                                               {1, "1N3"},
                                               {2, "3N1"},
                                               {3, "2N3"},
                                               {4, "2N1"}};
    for (auto const& tr: trs)
    {
        std::cout<<tr.triplet[0]<< tr.triplet[1] << tr.triplet[2] << " : " << m_id_rule.at(tr.rule_id) << std::endl;
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









