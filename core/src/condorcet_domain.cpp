#include "condorcet_domain.h"

CondorcetDomain::CondorcetDomain(Int8 n)
{
    this->n = n;

    for (Int8 i = 1; i <= n; i ++)
        m_triple_elems.push_back(i);

    for (Int8 i = 1; i <= n-2; i++)
    {
        for (Int8 j = i + 1; j <= n - 1; j++)
        {
            for (Int8 k = j + 1; k <= n; k++)
                m_num_triples += 1;
        }
    }
}

void CondorcetDomain::filter_cd(const TripleRule& tr, CD& cd)
{
    const Int8& first = tr.triple[0], second = tr.triple[1], third = tr.triple[2];
    const Int8 rule_id = tr.rule_id;

    cd.remove_if([&](const IntList& elem)
    {
        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule_id == 1 && first_index > second_index && first_index > third_index)        ||  // 1N3
            (rule_id == 2 && third_index < first_index && third_index < second_index)        ||  // 3N1
            (rule_id == 3 && second_index > first_index && second_index > third_index)       ||  // 2N3
            (rule_id == 4 && second_index < first_index && second_index < third_index)       ||  // 2N1
            (rule_id == 5 && ((first_index > second_index && first_index < third_index)
                              || (first_index > third_index && first_index < second_index))) ||  // 1N2
            (rule_id == 6 && ((third_index > first_index && third_index < second_index)
                              || (third_index > second_index && third_index < first_index))))    // 3N2
            return true;
        else
            return false;
    });

}

void CondorcetDomain::filter_trs_list(std::list<TripleRule>& trs, const IntList& elem)
{

    trs.remove_if([&](const TripleRule& tr){
        const Int8& first = tr.triple[0], second = tr.triple[1], third = tr.triple[2];
        const Int8 rule_id = tr.rule_id;

        int first_index = get_index(elem, first);
        int second_index = get_index(elem, second);
        int third_index = get_index(elem, third);

        if ((rule_id == 1 && first_index > second_index && first_index > third_index)        ||  // 1N3
            (rule_id == 2 && third_index < first_index && third_index < second_index)        ||  // 3N1
            (rule_id == 3 && second_index > first_index && second_index > third_index)       ||  // 2N3
            (rule_id == 4 && second_index < first_index && second_index < third_index)       ||  // 2N1
            (rule_id == 5 && ((first_index > second_index && first_index < third_index)
                              || (first_index > third_index && first_index < second_index))) ||  // 1N2s
            (rule_id == 6 && ((third_index > first_index && third_index < second_index)
                              || (third_index > second_index && third_index < first_index))))    // 3N2
            return true;
        else
            return false;
    });
}

void CondorcetDomain::expand_cd(CD& cd, Int8& value)
{
    CD updated_cd;
    for (auto& elem: cd)
    {
        auto iter = elem.begin();
        for (Int8 i = 0; i <= elem.size(); i ++)
        {
            iter = elem.insert(iter, value);
            updated_cd.push_back(elem);
            iter = elem.erase(iter);
            iter ++;
        }
    }
    cd = updated_cd;
}

bool CondorcetDomain::check_permutation(const IntList& permutation, const TRS& trs)
{
    for (const TripleRule& tr : trs)
    {
        const Int8& first = tr.triple[0], second = tr.triple[1], third = tr.triple[2];
        const Int8& rule_id = tr.rule_id;

        if (rule_id != 0)
        {
            int first_index = get_index(permutation, first);
            if (first_index == -1)
                continue;

            int second_index = get_index(permutation, second);
            if (second_index == -1)
                continue;

            int third_index = get_index(permutation, third);
            if (third_index == -1)
                continue;

            if ((rule_id == 1 && first_index > second_index && first_index > third_index) ||  // 1N3
                (rule_id == 2 && third_index < first_index && third_index < second_index) ||  // 3N1
                (rule_id == 3 && second_index > first_index && second_index > third_index) ||  // 2N3
                (rule_id == 4 && second_index < first_index && second_index < third_index) ||  // 2N1
                (rule_id == 5 && ((first_index > second_index && first_index < third_index)
                                  || (first_index > third_index && first_index < second_index))) ||  // 1N2
                (rule_id == 6 && ((third_index > first_index && third_index < second_index)
                                  || (third_index > second_index && third_index < first_index))))    // 3N2
                return false;
        }
    }
    return true;
}

void CondorcetDomain::expand_permutation(IntList& permutation, const TRS& trs, Int8 alternative, std::size_t& cd_size, std::size_t& threshold, bool& reach_threshold)
{
    auto iter = permutation.begin();
    for (Int8 i = 0; i <= permutation.size(); i ++)
    {
        if (reach_threshold) // ensures the recursion stops here and won't go deeper.
            return;

        iter = permutation.insert(iter, alternative);
        if (check_permutation(permutation, trs))
        {
            if (alternative == n)
            {
                cd_size++;
                if (cd_size >= threshold)
                {
                    reach_threshold = true;
                    return;
                }

            }
            else
                expand_permutation(permutation, trs, alternative+1, cd_size, threshold, reach_threshold);
        }
        iter = permutation.erase(iter);
        iter ++;
    }
}

TRS CondorcetDomain::fetch_trs(const TRS& trs, Int8 i)
{
    TRS fetched_trs;
    for (const auto& tr: trs)
    {
        if (tr.triple[2] == i)
            fetched_trs.push_back(tr);
    }
    return fetched_trs;
}

void CondorcetDomain::build_triple_index(const TRS& trs)
{
    for (Int32 i = 0; i < trs.size(); i ++)
        m_triple_index[trs[i].triple] = i;
}

TRS CondorcetDomain::sort_trs(TRS trs)
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

    build_triple_index(trs);
    return trs;
}

TRS CondorcetDomain::init_trs_random(bool is_sorted)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 6);

    TRS trs;
    for (Int8 i = 1; i < n+1; i ++)
    {
        for (Int8 k = 1; k < n+1; k ++)
        {
            for (Int8 j = 1; j < n+1; j ++)
            {
                if (i < j && j < k)
                {
                    TripleRule tr;
                    tr.triple = {i, j, k};
                    tr.rule_id = distrib(gen);
                    trs.push_back(tr);
                }
            }
        }
    }

    if (is_sorted)
        trs = sort_trs(trs);

    return trs;
}

TRS CondorcetDomain::init_trs()
{
    TRS trs;
    for (Int8 i = 1; i < n+1; i ++)
    {
        for (Int8 k = 1; k < n+1; k ++)
        {
            for (Int8 j = 1; j < n+1; j ++)
            {
                if (i < j && j < k)
                {
                    TripleRule tr{};
                    tr.triple = {i, j, k};
                    tr.rule_id = 0;
                    trs.push_back(tr);
                }
            }
        }
    }

    build_triple_index(trs);
    return trs;
}

TRS CondorcetDomain::trs_to_rz_ordering(const TRS& trs)
{
    TRS trs_rz = init_trs();
    for (const TripleRule& tr : trs)
    {
        trs_rz = assign_id(trs_rz, tr.triple, tr.rule_id);
    }

    return trs_rz;
}

TRS CondorcetDomain::init_trs_lex()
{
    TRS trs;
    for (Int8 i = 1; i <= n-2; i ++)
    {
        for (Int8 j = i+1; j <= n-1; j ++)
        {
            for (Int8 k = j+1; k <= n; k ++)
            {
                TripleRule tr{};
                tr.triple = {i, j, k};
                tr.rule_id = 0;
                trs.push_back(tr);
            }
        }
    }

    build_triple_index(trs);
    return trs;
}

TRS CondorcetDomain::init_trs_colex()
{
    TRS trs;
    for (Int8 k = 3; k <= n; k ++)
    {
        for (Int8 j = 2; j < k; j ++)
        {
            for (Int8 i = 1; i < j; i++)
            {
                TripleRule tr{};
                tr.triple = {i, j, k};
                tr.rule_id = 0;
                trs.push_back(tr);
            }
        }
    }

    build_triple_index(trs);
    return trs;
}

TRS CondorcetDomain::init_trs_by_scheme(TRS trs, const std::function<std::string(Triple)>& scheme_fun)
{
    for (TripleRule& tr : trs)
        tr.rule_id = m_rule_id.at(scheme_fun(tr.triple));

    build_triple_index(trs);
    return trs;
}

TRS CondorcetDomain::clear_trs(TRS trs)
{
    for (TripleRule& tr : trs)
        tr.rule_id = 0;

    return trs;
}

TRS CondorcetDomain::shuffle_trs(TRS trs, int seed)
{
    TRS empty_trs = init_trs();
    empty_trs = transfer_trs(trs, empty_trs);

    std::mt19937_64 urbg(seed);
    std::shuffle(empty_trs.begin(), empty_trs.end(), urbg);

    build_triple_index(empty_trs);

    return empty_trs;
}

TRS CondorcetDomain::transfer_trs(const TRS& from_trs, TRS to_trs)
{
    for (TripleRule& to_tr : to_trs)
    {
        for (const TripleRule& from_tr : from_trs)
        {
            if (to_tr.triple == from_tr.triple)
                to_tr.rule_id = from_tr.rule_id;
        }
    }
    return to_trs;
}

TRS CondorcetDomain::assign_id(TRS trs, const Triple& triple, const Int8 rule_id)
{
    Int32 index = m_triple_index[triple];
    trs[index].rule_id = rule_id;

    return trs;
}

TRS CondorcetDomain::assign_id_any_ordering(TRS trs, const Triple& triple, const Int8 rule_id)
{
    for (TripleRule& tr : trs)
    {
        if (tr.triple == triple)
        {
            tr.rule_id = rule_id;
            break;
        }
    }
    return trs;
}

TRS CondorcetDomain::assign_rule(TRS trs, const Triple& triple, const std::string& rule)
{
    Int32 index = m_triple_index[triple];
    trs[index].rule_id = m_rule_id.at(rule);

    return trs;
}

TRS CondorcetDomain::assign_rule_any_ordering(TRS trs, const Triple& triple, const std::string& rule)
{
    for (TripleRule& tr : trs)
    {
        if (tr.triple == triple)
        {
            tr.rule_id = m_rule_id[rule];
            break;
        }
    }
    return trs;
}

TRS CondorcetDomain::assign_id_by_index(TRS trs, Int32 index, const Int8 rule_id)
{
    trs[index].rule_id = rule_id;
    return trs;
}

TRS CondorcetDomain::assign_rule_by_index(TRS trs, Int32 index, const std::string& rule)
{
    trs[index].rule_id = m_rule_id.at(rule);
    return trs;
}

Triple CondorcetDomain::next_unassigned_triple(const TRS& trs)
{
    for (const TripleRule& tr: trs)
    {
        if (tr.rule_id == 0)
            return tr.triple;
    }

    return {0, 0, 0};
}

std::vector<Triple> CondorcetDomain::unassigned_triples(const TRS& trs)
{
    std::vector<Triple> unassigned{};
    for (const TripleRule& tr: trs)
    {
        if (tr.rule_id == 0)
            unassigned.push_back(tr.triple);
    }
    return unassigned;
}

std::vector<Triple> CondorcetDomain::assigned_triples(const TRS& trs)
{
    std::vector<Triple> assigned{};
    for (const TripleRule& tr: trs)
    {
        if (tr.rule_id != 0)
            assigned.push_back(tr.triple);
    }
    return assigned;
}

std::vector<std::size_t> CondorcetDomain::evaluate_rules_on_triple(const TRS& trs, const Triple& triple)
{
    std::vector<std::size_t> sizes = {};
    for (const std::string& rule: m_rules)
    {
        TRS new_trs = assign_rule(trs, triple, rule);
        std::size_t s = size(new_trs);
        sizes.push_back(s);
    }
    return sizes;
}

Triple CondorcetDomain::dynamic_triple_ordering(const TRS& trs)
{
    std::map<std::size_t, Triple> size_triple;

    std::vector<Triple> unassigned = unassigned_triples(trs);
    if (unassigned.empty())
        return Triple{0, 0, 0};

    for (const Triple& unassigned_triple : unassigned)
    {
        auto sizes = evaluate_rules_on_triple(trs, unassigned_triple);
        auto max_size = *std::max_element(sizes.begin(), sizes.end());
        size_triple[max_size] = unassigned_triple;
    }
    const auto max_size_triple_ptr = std::min_element(std::begin(size_triple), std::end(size_triple),
                                                       [](const std::pair<std::size_t, Triple>& p1, const std::pair<std::size_t, Triple>& p2)
                                                       {
                                                           return p1.first < p2.first;
                                                       });

    return max_size_triple_ptr->second;

}

// [1, 2, 3, 5, 7] subset must be ordered
TRS CondorcetDomain::uplift_trs(const TRS& large, const TRS& small, const std::vector<Int8>& subset)
{
    TRS uplifted_trs = large;
    std::map<Int8, Int8> dict;
    for (Int8 i = 0; i < subset.size(); i++)
        dict[i + 1] = subset[i];

    for (const TripleRule& s: small)
    {
        const auto& [triple, rule_id] = s;
        Triple mapped_triple = {dict[triple[0]], dict[triple[1]], dict[triple[2]]};
        uplifted_trs = assign_id(uplifted_trs, mapped_triple, rule_id);
    }
    return uplifted_trs;
}

std::vector<Int8> CondorcetDomain::trs_to_state(const TRS& trs)
{
    std::vector<Int8> state;
    for (const TripleRule& tr: trs)
        state.push_back(tr.rule_id);

    return state;
}

TRS CondorcetDomain::state_to_trs(TRS trs, const std::vector<Int8>& state)
{
    Int32 i = 0;
    for (TripleRule& tr : trs)
    {
        tr.rule_id = state[i];
        i ++;
    }
    return trs;
}

CD CondorcetDomain::domain(const TRS& trs)
{
    CD cd = {{1, 2}, {2, 1}};
    for (Int8 i = 3; i <= n; i++)
    {
        expand_cd(cd, i);
        TRS fetched_trs = fetch_trs(trs, i);
        for (const TripleRule& tr: fetched_trs)
            filter_cd(tr, cd);
    }

    return cd;
}

std::size_t CondorcetDomain::size(const TRS& trs, std::size_t threshold)
{
    CD init_permutations = {{1, 2}, {2, 1}};
    std::size_t cd_size{};

    bool reach_threshold = false;
    for (IntList& permutation : init_permutations)
    {
        if (!reach_threshold)
            expand_permutation(permutation, trs, 3, cd_size, threshold, reach_threshold);
    }

    return cd_size;
}

CD CondorcetDomain::filter_domain(const CD& domain, const TripleRule& tr)
{
    CD resulting_domain = domain;
    filter_cd(tr, resulting_domain);
    return resulting_domain;
}

void CondorcetDomain::init_subset(Int8 sub_n)
{
    m_sub_n = sub_n;
    m_subsets = combinations(m_triple_elems, m_sub_n);
    m_subset_dicts.clear();

    for (const std::vector<Int8>& subset: m_subsets)
    {
        std::map<Int8, Int8> dict{};
        for (Int8 i = 0; i < sub_n; i++)  // construct the dictionary
        {
            const Int8& value = subset[i];
            dict[value] = i + 1;
        }
        m_subset_dicts.push_back(dict);
    }
}

std::vector<int> CondorcetDomain::subset_weights()
{
    std::vector<int> weights{};
    for (const std::vector<Int8>& subset : m_subsets)
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

    for (Int32 subset_idx = 0; subset_idx < m_subsets.size(); subset_idx++)  // for each subset
    {
        const std::vector<Int8>& subset = m_subsets[subset_idx];
        std::map<Int8, Int8> subset_dict = m_subset_dicts[subset_idx];

        TRS sub_trs{};
        for (const TripleRule& tr: trs)  // find the triple that is contained in the subset
        {
            const auto& [triple, rule_id] = tr;
            if (std::includes(subset.begin(), subset.end(), triple.begin(), triple.end()))
            {
                TripleRule sub_triple_rule;
                for (Int8 i = 0; i < 3; i++)
                    sub_triple_rule.triple[i] = subset_dict[triple[i]];
                sub_triple_rule.rule_id = rule_id;
                sub_trs.push_back(sub_triple_rule);
            }
        }
        trs_list.push_back(sub_trs);
    }

    return trs_list;
}

CDS CondorcetDomain::subset_domain_list(const CD& cd)
{
    CDS subset_cds{};

    for (Int32 subset_idx = 0; subset_idx < m_subsets.size(); subset_idx ++)
    {
        const std::vector<Int8>& subset = m_subsets[subset_idx];
        std::map<Int8, Int8> subset_dict = m_subset_dicts[subset_idx];

        std::set<IntList> subset_cd_set{};
        for (const IntList& permutation : cd)
        {
            IntList sub_permutation{};
            for (const Int8& p : permutation)
            {
                Int8 element = subset_dict[p];
                if (element != 0)
                    sub_permutation.push_back(element);
            }

            subset_cd_set.insert(sub_permutation);
        }

        CD subset_cd(subset_cd_set.begin(), subset_cd_set.end());
        subset_cds.push_back(subset_cd);
    }

    return subset_cds;
}

CD CondorcetDomain::domain_on_alternatives(const CD& cd, const std::vector<Int8>& alternatives)
{
    std::set<IntList> subset_domain{};

    for (const IntList& permutation : cd)
    {
        IntList sub_permutation{};
        for (const Int8& p : permutation)
        {
            if (std::find(alternatives.begin(), alternatives.end(), p) != alternatives.end())
                sub_permutation.push_back(p);
        }
        subset_domain.insert(sub_permutation);
    }
    CD subset_cd(subset_domain.begin(), subset_domain.end());

    return subset_cd;
}

CD CondorcetDomain::domain_on_triple(const CD& domain, const Triple& triple)
{
    std::vector<Int8> new_triple(triple.begin(), triple.end());
    return domain_on_alternatives(domain, new_triple);
}

std::vector<std::vector<Int8>> CondorcetDomain::subset_states(const TRS& trs)
{
    std::vector<std::vector<Int8>> sub_states{};
    std::vector<TRS> trs_list = subset_trs_list(trs);
    for (const TRS& sub_trs : trs_list)
    {
        std::vector<Int8> sub_state = trs_to_state(sub_trs);
        sub_states.push_back(sub_state);
    }

    return sub_states;
}

std::vector<std::vector<Int8>> CondorcetDomain::subset_states_any_ordering(const TRS& trs)
{
    std::vector<std::vector<Int8>> sub_states{};
    CondorcetDomain sub_cd(m_sub_n);

    std::vector<TRS> trs_list = subset_trs_list(trs);
    for (const TRS& sub_trs : trs_list)
    {
        TRS sub_trs_init = sub_cd.init_trs();
        for (const TripleRule& tr : sub_trs)
        {
            for (const TripleRule& tr_init : sub_trs_init)
            {
                if (tr.triple == tr_init.triple)
                    sub_trs_init = sub_cd.assign_id(sub_trs_init, tr.triple, tr.rule_id);
            }
        }

        std::vector<Int8> sub_state = trs_to_state(sub_trs_init);
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
        std::size_t sub_size = sub_cd.size(sub_trs);

        sizes.push_back(sub_size);
        all_trs.push_back(sub_trs);
    }

    return std::make_tuple(all_trs, sizes);
}

CD CondorcetDomain::inverse_domain(const CD& cd, const IntList& permutation)
{
    std::map<Int8, Int8> dict;
    for (Int8 i = 0; i < permutation.size(); i ++)
        dict[*std::next(permutation.begin(), i)] = (i + 1);

    CD new_cd{};
    for (const IntList& perm: cd)
    {
        IntList new_perm;
        for (const Int8& p: perm)
        {
            new_perm.push_back(dict[p]);
        }
        new_cd.push_back(new_perm);
    }

    return new_cd;
}

CDS CondorcetDomain::isomorphic_domains(const CD& cd)
{
    CDS cds;

    for (const IntList& permutation : cd)
    {
        CD new_cd = inverse_domain(cd, permutation);
        new_cd.sort(compare_permutations);

        cds.push_back(new_cd);
    }
    return cds;  // Each cd inside cds is sorted; but cds is not sorted.
}

CD CondorcetDomain::isomorphic_hash(const CD& cd)
{
    CDS cds = isomorphic_domains(cd);
    cds.sort(compare_cds);
    return cds.front();   // This cd is sorted.
}

CDS CondorcetDomain::non_isomorphic_domains(CDS cds)
{
    CDS sorted_cds{};
    for (CD& cd : cds)
        cd.sort(compare_permutations);

    CDS iso_list{};
    while (cds.size() > 0)
    {
        CD cd = cds.front();
        cds.pop_front();
        iso_list.push_back(cd);
        for (const IntList& permutation : cd)
        {
            CD inversed_cd = inverse_domain(cd, permutation);
            inversed_cd.sort(compare_permutations);
            cds.remove(inversed_cd);
        }
    }

    return iso_list;
}

TRS CondorcetDomain::inverse_trs(const TRS& trs, const IntList& permutation, const std::vector<std::string>& rules)
{
    TRS new_trs = clear_trs(trs);

    std::map<Int8, Int8> perm_dict{};
    Int8 i = 1;
    for (const Int8& p : permutation)
    {
        perm_dict[p] = i;
        i ++;
    }

    for (const TripleRule& tr : trs)
    {
        Triple new_triple{perm_dict[tr.triple[0]], perm_dict[tr.triple[1]], perm_dict[tr.triple[2]]};
        std::sort(new_triple.begin(), new_triple.end());

        if (tr.rule_id != 0)
        {
            std::string rule = m_rules[tr.rule_id - 1];
            auto index_pointer = std::find(new_triple.begin(), new_triple.end(), perm_dict[tr.triple[int(rule[0] - '0')-1]]);
            Int8 index = std::distance(new_triple.begin(), index_pointer) + 1;
            std::string new_rule = std::to_string(index) + rule[1] + rule[2];

            if (std::find(rules.begin(), rules.end(), new_rule) == rules.end())
                return TRS();

            new_trs = assign_rule_any_ordering(new_trs, new_triple, new_rule);
        }
    }

    return new_trs;
}

bool CondorcetDomain::is_trs_isomorphic(const TRS& trs, const std::vector<std::string>& rules)
{
    TRS empty_trs = clear_trs(trs);

    Int8 largest_alternative{};
    for (auto iter=trs.begin(); iter != trs.end(); ++iter)
    {
        if (iter->rule_id != 0)
            if (iter->triple[2] > largest_alternative)
                largest_alternative = iter->triple[2];
    }

    std::vector<Int8> alternatives(largest_alternative);
    std::iota(alternatives.begin(), alternatives.end(), 1);

    do {
        bool go_to_next = false;
        TRS new_trs(empty_trs);

        std::map<Int8, Int8> perm_dict{};
        for (Int8 i = 1; i <= n; i ++)
            perm_dict[i] = i;
        for (Int8 i = 0; i < alternatives.size(); i ++)
            perm_dict[alternatives[i]] = i + 1;

        for (const TripleRule& tr : trs)
        {
            Triple new_triple{perm_dict[tr.triple[0]], perm_dict[tr.triple[1]], perm_dict[tr.triple[2]]};
            std::sort(new_triple.begin(), new_triple.end());

            if (tr.rule_id != 0)
            {
                std::string rule = m_rules[tr.rule_id - 1];
                auto index_pointer = std::find(new_triple.begin(), new_triple.end(), perm_dict[tr.triple[int(rule[0] - '0')-1]]);
                Int8 index = std::distance(new_triple.begin(), index_pointer) + 1;
                std::string new_rule = std::to_string(index) + rule[1] + rule[2];

                if (std::find(rules.begin(), rules.end(), new_rule) == rules.end())
                {
                    go_to_next = true;
                    break;
                }

                new_trs = assign_rule_any_ordering(new_trs, new_triple, new_rule);
            }
            else
            {
                for (const std::string& rule : rules)
                {
                    auto index_pointer = std::find(new_triple.begin(), new_triple.end(), perm_dict[tr.triple[int(rule[0] - '0')-1]]);
                    Int8 index = std::distance(new_triple.begin(), index_pointer) + 1;
                    std::string new_rule = std::to_string(index) + rule[1] + rule[2];

                    if (std::find(rules.begin(), rules.end(), new_rule) == rules.end())
                    {
                        go_to_next = true;
                        break;
                    }
                }

            }
        }

        if (go_to_next == false)
        {
            if (trs_to_state(trs) < trs_to_state(new_trs))
                return true;
        }

    }
    while(std::next_permutation(alternatives.begin(), alternatives.end()));

    return false;
}

bool CondorcetDomain::is_trs_isomorphic_1n33n1(const TRS& trs)
{
    const std::vector<std::string> rules{"1N3", "3N1"};
    TRS empty_trs = clear_trs(trs);

    Int8 largest_alternative{};
    for (auto iter=trs.begin(); iter != trs.end(); ++iter)
    {
        if (iter->rule_id != 0)
            if (iter->triple[2] > largest_alternative)
                largest_alternative = iter->triple[2];
    }

    std::vector<Int8> alternatives(largest_alternative);
    std::iota(alternatives.begin(), alternatives.end(), 1);

    do {
        bool go_to_next = false;
        TRS new_trs(empty_trs);

        std::map<Int8, Int8> perm_dict{};
        for (Int8 i = 1; i <= n; i ++)
            perm_dict[i] = i;
        for (Int8 i = 0; i < alternatives.size(); i ++)
            perm_dict[alternatives[i]] = i + 1;

        for (const TripleRule& tr : trs)
        {
            Triple new_triple{perm_dict[tr.triple[0]], perm_dict[tr.triple[1]], perm_dict[tr.triple[2]]};
            std::sort(new_triple.begin(), new_triple.end());

            if (tr.rule_id == 1 || tr.rule_id == 2)
            {
                std::string rule = m_rules[tr.rule_id - 1];
                auto index_pointer = std::find(new_triple.begin(), new_triple.end(), perm_dict[tr.triple[int(rule[0] - '0')-1]]);
                Int8 index = std::distance(new_triple.begin(), index_pointer) + 1;
                std::string new_rule = std::to_string(index) + rule[1] + rule[2];

                if (std::find(rules.begin(), rules.end(), new_rule) == rules.end())
                {
                    go_to_next = true;
                    break;
                }

                new_trs = assign_rule_any_ordering(new_trs, new_triple, new_rule);
            }
            else if (tr.rule_id == 0)
            {
                for (const std::string& rule : rules)
                {
                    auto index_pointer = std::find(new_triple.begin(), new_triple.end(), perm_dict[tr.triple[int(rule[0] - '0')-1]]);
                    Int8 index = std::distance(new_triple.begin(), index_pointer) + 1;
                    std::string new_rule = std::to_string(index) + rule[1] + rule[2];

                    if (std::find(rules.begin(), rules.end(), new_rule) == rules.end())
                    {
                        go_to_next = true;
                        break;
                    }
                }

            }
        }

        if (go_to_next == false)
        {
            if (trs_to_state(trs) < trs_to_state(new_trs))
                return true;
        }

    }
    while(std::next_permutation(alternatives.begin(), alternatives.end()));

    return false;
}


std::vector<TRS> CondorcetDomain::isomorphic_trs_list(const TRS& trs, const std::vector<std::string>& rules)
{
    std::vector<TRS> trs_list{};

    for (const IntList& permutation : domain(trs))
    {
        TRS new_trs = inverse_trs(trs, permutation, rules);
        if (new_trs.size() != 0)
            trs_list.push_back(new_trs);
    }

    return trs_list;
}

TRS CondorcetDomain::domain_to_trs(const CD &cd)
{
    std::list<TripleRule> all_trs;

    TRS trs = init_trs();
    for (TripleRule tr: trs)
    {
        for (const std::string& rule: m_rules)
        {
            tr.rule_id = m_rule_id.at(rule);
            all_trs.push_back(tr);
        }
    }
    for (const IntList& elem : cd)
    {
        filter_trs_list(all_trs, elem);
    }

    TRS result_trs(all_trs.begin(), all_trs.end());
    return result_trs;
}

bool CondorcetDomain::is_domain_copious(const TRS& trs, const CD& domain)
{
    for (const TripleRule& tr : trs)
    {
        if (domain_on_triple(domain, tr.triple).size() < 4)
            return false;
    }
    return true;
}

bool CondorcetDomain::is_domain_ample(const CD& domain)
{
    std::vector<std::vector<Int8>> triples = combinations(m_triple_elems, 2);
    for (const std::vector<Int8>& triple : triples)
    {
        if (domain_on_alternatives(domain, triple).size() != 2)
            return false;
    }
    return true;
}

bool CondorcetDomain::domain_include(const CD& domain, const IntList& permutation)
{
    if (std::find(domain.begin(), domain.end(), permutation) == domain.end())
        return false;
    else
        return true;
}

bool CondorcetDomain::is_domain_unitary(const CD& domain)
{
    std::list<Int8> unitary_permutation{};
    for (Int8 i = 1; i <= n; i ++)
        unitary_permutation.push_back(i);

    return domain_include(domain, unitary_permutation);
}

bool CondorcetDomain::is_domain_maximal_width(const CD& domain)
{
    std::list<Int8> unitary_permutation{};
    for (Int8 i = 1; i <= n; i ++)
        unitary_permutation.push_back(i);

    if (!domain_include(domain, unitary_permutation))
        return false;

    unitary_permutation.reverse();
    if (!domain_include(domain, unitary_permutation))
        return false;

    return true;
}

CD CondorcetDomain::dual_domain(CD domain)
{
    for (IntList& permutation : domain) // calculate the dual domain by definition
        permutation.reverse();

    return inverse_domain(domain, domain.front());  // return its unitary form
}

float CondorcetDomain::trs_core(const TRS& trs, const CD& domain)
{
    std::set<std::vector<Int8>> unique_iso_trs_set{};

    for (const IntList& permutation : domain)
    {
        TRS iso_trs = inverse_trs(trs, permutation, m_rules);
        unique_iso_trs_set.insert(trs_to_state(iso_trs));
    }

    return domain.size() / unique_iso_trs_set.size();

}

void print_trs(const TRS& trs)
{
    const std::map<Int8, std::string> m_id_rule{{0, ""},
                                                {1, "1N3"},
                                                {2, "3N1"},
                                                {3, "2N3"},
                                                {4, "2N1"},
                                                {5, "1N2"},
                                                {6, "3N2"}};
    for (auto const& tr: trs)
    {
        std::cout<<tr.triple[0]<< tr.triple[1] << tr.triple[2] << " : " << m_id_rule.at(tr.rule_id) << std::endl;
    }
    std::cout << std::endl;
}

void print_cd(const CD& cd)
{
    for (const IntList& elem: cd)
    {
        for (const Int8& e: elem)
            std::cout << e;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}





