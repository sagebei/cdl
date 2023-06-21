#include "forbidden_permutation.h"

ForbiddenPermutation::ForbiddenPermutation(Int8 n)
{
    this->n = n;

    for (Int8 i = 1; i <= n-2; i++)
    {
        for (Int8 j = i + 1; j <= n - 1; j++)
        {
            for (Int8 k = j + 1; k <= n; k++)
                m_num_triplets += 1;
        }
    }
}

void ForbiddenPermutation::build_triplet_index(const TLS& tls)
{
    for (Int32 i = 0; i < tls.size(); i ++)
    {
        Triplet triplet = tls[i].triplet;
        m_triplet_index[triplet] = i;
    }
}

TLS ForbiddenPermutation::init_tls()
{
    TLS tls;
    for (Int8 i = 1; i < n+1; i ++)
    {
        for (Int8 k = 1; k < n+1; k ++)
        {
            for (Int8 j = 1; j < n+1; j ++)
            {
                if (i < j && j < k)
                {
                    TripletLaws tl{};
                    tl.triplet = {i, j, k};
                    tl.laws = {};
                    tls.push_back(tl);
                }
            }
        }
    }

    build_triplet_index(tls);
    return tls;
}

TLS ForbiddenPermutation::assign_laws(TLS tls, const Triplet& triplet, const std::vector<std::string> laws)
{
    Int32 index = m_triplet_index[triplet];
    for (auto law : laws)
        tls[index].laws.push_back(law);
    return tls;
}

//CD ForbiddenPermutation::domain(const TLS& tls)
//{
//
//}


bool ForbiddenPermutation::check_permutation(const IntList& permutation, const TLS& tls)
{
    for (const TripletLaws& tl : tls)
    {
        const Int8 &first = tl.triplet[0], second = tl.triplet[1], third = tl.triplet[2];

        for (const std::string& law: tl.laws)
        {
            if (law != "")
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

                if ((law == "132" && first_index > third_index && third_index > second_index)  ||
                    (law == "213" && second_index > first_index && first_index > third_index)  ||
                    (law == "231" && second_index > third_index && third_index > first_index)  ||
                    (law == "312" && third_index > first_index && first_index > second_index)  ||
                    (law == "321" && third_index > second_index && second_index > first_index))
                    return false;
            }
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
    std::size_t domain_size{};
    for (IntList& permutation : init_permutations)
    {
        expand_permutation(permutation, tls, 3, domain_size);
    }
    return domain_size;
}

