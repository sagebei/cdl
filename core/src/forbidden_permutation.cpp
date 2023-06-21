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
        m_triplet_index[tls[i].triplet] = i;
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
                    tls.push_back(tl);
                }
            }
        }
    }

    build_triplet_index(tls);
    return tls;
}

