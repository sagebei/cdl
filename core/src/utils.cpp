#include "utils.h"
#include <stdexcept>


std::size_t TripleHasher::operator()(const Triple& arr) const
{
    std::size_t h = 0;

    for (const Int8& a : arr) {
        h ^= std::hash<Int8>{}(a)  + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

bool compare_permutations(const IntList& first, const IntList& second)
{
    auto iter_first = first.begin();
    auto iter_second = second.begin();

    for (; iter_first != first.end(); ++iter_first, ++iter_second)
    {
        Int8 f = *iter_first;
        Int8 s = *iter_second;

        if (f < s)
            return true;
        else if (s < f)
            return false;
    }

    return false;
}

bool compare_cds(CD& first_cd, CD& second_cd)
{
    auto iter_first_cd = first_cd.begin();
    auto iter_second_cd = second_cd.begin();

    for (; iter_first_cd != first_cd.end(); ++iter_first_cd, ++iter_second_cd)
    {
        const IntList& first_permutation = *iter_first_cd;
        const IntList& second_permutation = *iter_second_cd;

        auto iter_first_elem = first_permutation.begin();
        auto iter_second_elem = second_permutation.begin();

        for (; iter_first_elem != first_permutation.end(); ++iter_first_elem, ++iter_second_elem)
        {
            Int8 f = *iter_first_elem;
            Int8 s = *iter_second_elem;

            if (f < s)
                return true;
            else if (s < f)
                return false;
        }

    }
    return false;

}

// get the index of a value in a list
int get_index(const IntList& permutation, const Int8& value)
{
    const auto value_index_iter = std::find(permutation.begin(), permutation.end(), value);
    if (value_index_iter != permutation.end())
        return std::distance(permutation.begin(), value_index_iter);
    else
        return -1;

}

void get_subset(std::vector<std::vector<Int8>>& subsets, Int8 n, Int8 sub_n, Int32 index, std::vector<Int8>& set, std::vector<Int8>& current_subset)
{
    if (n < sub_n) return;
    if (sub_n == 0)
    {
        subsets.push_back(current_subset);
        return;
    }
    get_subset(subsets, n-1, sub_n, index+1, set, current_subset);
    current_subset.push_back(set[index]);
    get_subset (subsets, n-1, sub_n-1, index+1, set, current_subset);
    current_subset.pop_back();
}


// return all distinct subsets of given size of a set
std::vector<std::vector<Int8>> combinations(std::vector<Int8>& set, Int8 sub_n)
{
    std::vector<std::vector<Int8>> subsets;
    std::vector<Int8> current_subset;
    get_subset(subsets, set.size(), sub_n, 0, set, current_subset);
    return subsets;
}


int benchmark_size(int n)
{
    switch (n) {
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 9;
        case 5:
            return 20;
        case 6:
            return 45;
        case 7:
            return 100;
        case 8:
            return 222;
        case 9:
            return 488;
        case 10:
            return 1069;
        case 11:
            return 2324;
        case 12:
            return 5034;
        default:
            throw std::runtime_error{"n is larger than 12"};
    }
}

std::string Fishburn_scheme(const Triple& triple)
{
    if ((triple[1] % 2) == 0)
        return "2N1";
    else
        return "2N3";
}
