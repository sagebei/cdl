#pragma once
#include "utils.h"

struct TripleLaws
{
    Triple triple;
    std::vector<std::string> laws;
};

typedef std::vector<TripleLaws> TLS;

class ForbiddenPermutation
{
private:
    void filter_domain(const TripleLaws& tl, CD& domain);
    void expand_domain(CD& domain, Int8& alternative);
    TLS fetch_tls(const TLS& tls, Int8 i);

public:
    Int8 n{};
    std::array<std::string, 5> m_laws{{"132", "213", "231", "312", "321"}};

    Int32 m_num_triples{};
    TripleIndex m_triple_index{};

    ForbiddenPermutation(Int8 n);
    void build_triple_index(const TLS& tls);
    TLS init_tls();
    TLS init_tls_by_scheme(const std::function<std::vector<std::string>(Triple)>& scheme_fun);
    TLS assign_laws(TLS tls, const Triple& triple, const std::vector<std::string> laws);
    TLS assign_laws_by_index(TLS tls, Int32 index, const std::vector<std::string> laws);

    bool check_permutation(const IntList& permutation, const TLS& tls);
    void expand_permutation(IntList& permutation, const TLS& tls, Int8 alternative, std::size_t& domain_size);

    CD domain(const TLS& tls);  // Width-First
    std::size_t size(const TLS& tls);     // Depth-First
};