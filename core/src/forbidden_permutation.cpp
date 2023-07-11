#include "forbidden_permutation.h"

ForbiddenPermutation::ForbiddenPermutation(Int8 n, Int8 k)
{
    this->n = n;
    this->k = k;

    std::vector<Int8> alternatives(n);
    std::iota(alternatives.begin(), alternatives.end(), 1);
    m_alternatives = alternatives;

    m_num_tuples = combinations(m_alternatives, k).size();
}

void ForbiddenPermutation::filter_domain(const TupleLaws& tl, CD& domain)
{
    domain.remove_if([&](const IntList& permutation) {
        std::vector<Int8> indices{};
        for (const Int8& t : tl.tuple)
        {
            int index = get_index(permutation, t);
            indices.push_back(index);
        }

        for (const std::vector<Int8>& law: tl.laws)
        {
            std::vector<bool> is_obey{};
            for (int i = 0; i < law.size()-1; i ++)
                is_obey.push_back(indices[law[i]-1] < indices[law[i+1]-1]);

            if (std::accumulate(is_obey.begin(), is_obey.end(), 0) == is_obey.size())
                return true;
        }
        return false;
    });
}

void ForbiddenPermutation::expand_domain(CD& domain, Int8& alternative)
{
    CD updated_domain;
    for (auto& elem: domain)
    {
        auto iter = elem.begin();
        for (Int8 i = 0; i <= elem.size(); i ++)
        {
            iter = elem.insert(iter, alternative);
            updated_domain.push_back(elem);
            iter = elem.erase(iter);
            iter ++;
        }
    }
    domain = updated_domain;
}

TLS ForbiddenPermutation::fetch_tls(const TLS& tls, Int8 i)
{
    TLS fetched_tls;
    for (const auto& tl: tls)
    {
        if (tl.tuple[2] == i)
            fetched_tls.push_back(tl);
    }
    return fetched_tls;
}

void ForbiddenPermutation::build_tuple_index(const TLS& tls)
{
    for (Int32 i = 0; i < tls.size(); i ++)
    {
        Tuple tuple = tls[i].tuple;
        m_tuple_index[tuple] = i;
    }
}

TLS ForbiddenPermutation::init_tls()
{
    TLS tls;

    std::vector<std::vector<Int8>> subsets = combinations(m_alternatives, k);
    std::reverse(subsets.begin(), subsets.end());

    for (std::vector<Int8>& subset : subsets)
    {
        TupleLaws tl{};
        tl.tuple = subset;
        tl.laws = {};
        tls.push_back(tl);
    }

    build_tuple_index(tls);
    return tls;
}

TLS ForbiddenPermutation::init_tls_by_scheme(const std::function<Laws(Tuple)>& scheme_fun)
{
    TLS tls{};

    std::vector<Int8> alternatives(n);
    std::iota(alternatives.begin(), alternatives.end(), 1);
    std::vector<std::vector<Int8>> subsets = combinations(alternatives, k);
    std::reverse(subsets.begin(), subsets.end());

    for (std::vector<Int8>& subset : subsets)
    {
        TupleLaws tl{};
        tl.tuple = subset;
        tl.laws = scheme_fun(tl.tuple);
        tls.push_back(tl);
    }

    build_tuple_index(tls);
    return tls;
}

TLS ForbiddenPermutation::assign_laws(TLS tls, const Tuple& tuple, const Laws laws)
{
    Int32 index = m_tuple_index[tuple];
    tls[index].laws = laws;
    return tls;
}

TLS ForbiddenPermutation::assign_laws_by_index(TLS tls, Int32 index, const Laws laws)
{
    tls[index].laws = laws;
    return tls;
}

CD ForbiddenPermutation::domain(const TLS& tls)
{
    CD domain = {{1, 2}, {2, 1}};
    for (Int8 i = 3; i <= n; i++) {
        expand_domain(domain, i);
        TLS fetched_tls = fetch_tls(tls, i);
        for (const TupleLaws& tl: fetched_tls)
            filter_domain(tl, domain);
    }

    return domain;
}


bool ForbiddenPermutation::check_permutation(const IntList& permutation, const TLS& tls)
{
    for (const TupleLaws& tl : tls)
    {
        bool skip = false;
        std::vector<Int8> indices{};

        for (const Int8& t : tl.tuple)
        {
            int index = get_index(permutation, t);
            if (index == -1)
            {
                skip = true;
                break;
            }
            indices.push_back(index);
        }
        if (skip)
            continue;

        for (const std::vector<Int8>& law: tl.laws)
        {
            std::vector<bool> is_obey{};
            for (int i = 0; i < law.size()-1; i ++)
                is_obey.push_back(indices[law[i]-1] < indices[law[i+1]-1]);

            if (std::accumulate(is_obey.begin(), is_obey.end(), 0) == is_obey.size())
                return false;   // does not pass the check
        }

    }
    return true;
}

void ForbiddenPermutation::expand_permutation(IntList& permutation, const TLS& tls, Int8 alternative, std::size_t& domain_size)
{
    auto iter = permutation.begin();
    for (Int8 i = 0; i <= permutation.size(); i ++)
    {
        iter = permutation.insert(iter, alternative);
//        std::cout << check_permutation(permutation, tls) << std::endl;
        if (check_permutation(permutation, tls))
        {
            if (alternative == n)
                domain_size ++;
            else
                expand_permutation(permutation, tls, alternative+1, domain_size);
        }
        iter = permutation.erase(iter);
        iter ++;
    }
}

std::size_t ForbiddenPermutation::size(const TLS& tls)
{
    CD init_permutations = {{1, 2}, {2, 1}};
    std::size_t domain_size = 0;
    for (IntList& permutation : init_permutations)
    {
        expand_permutation(permutation, tls, 3, domain_size);
    }
    return domain_size;
}

void print_tls(const TLS& tls)
{
    for (const TupleLaws& tl : tls)
    {
        for (const Int8& t : tl.tuple)
            std::cout << t;
        std::cout << " : ";
        for (const std::vector<Int8>& law : tl.laws)
        {
            for (const Int8& l : law)
                std::cout << l;
            std::cout << ", ";
        }
        std::cout << std::endl;
    }
}




