#pragma once
#include "utils.h"

typedef std::vector<Int8> Tuple;
typedef std::vector<std::vector<Int8>> Laws;
typedef std::map<Tuple, Int32> TupleIndex;

struct TupleLaws
{
    Tuple tuple;
    Laws laws;
};
typedef std::vector<TupleLaws> TLS;

class ForbiddenPermutation
{
private:
    void filter_domain(const TupleLaws& tl, CD& domain);
    void expand_domain(CD& domain, Int8& alternative);
    TLS fetch_tls(const TLS& tls, Int8 i);

public:
    Int8 n{};
    Int8 k{};

    std::vector<Int8> m_alternatives{};
    Int32 m_num_tuples{};
    TupleIndex m_tuple_index{};

    ForbiddenPermutation(Int8 n, Int8 k);
    void build_tuple_index(const TLS& tls);

    TLS init_tls();
    TLS init_tls_by_scheme(const std::function<Laws(Tuple)>& scheme_fun);
    TLS assign_laws(TLS tls, const Tuple& tuple, const Laws& laws);
    TLS assign_laws_by_index(TLS tls, Int32 index, const Laws& laws);

    bool check_permutation(const IntList& permutation, const TLS& tls);
    void expand_permutation(IntList& permutation, const TLS& tls, Int8 alternative, std::size_t& domain_size);

    CD domain(const TLS& tls);  // Width-First
    std::size_t size(const TLS& tls);     // Depth-First
};

// printing functions
void print_tls(const TLS& tls);