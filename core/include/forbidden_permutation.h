#pragma once
#include "utils.h"

struct TripletLaws
{
    Triplet triplet;
    std::vector<std::string> laws;
};

typedef std::vector<TripletLaws> TLS;

class ForbiddenPermutation
{
//private:


public:
    Int8 n{};
    const std::array<std::string, 5> m_laws{{"132", "213", "231", "312", "321"}};

    Int32 m_num_triplets{};
    TripletIndex m_triplet_index{};

    ForbiddenPermutation(Int8 n);
    void build_triplet_index(const TLS& tls);
    TLS init_tls();
};