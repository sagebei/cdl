#include "utils.h"
#include <iostream>
#include <vector>
#include <stdexcept>


std::size_t TripletHasher::operator()(const std::array<int, 3>& arr) const
{
    std::size_t h = 0;

    for (const int& a : arr) {
        h ^= std::hash<int>{}(a)  + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}

// used to sort a list of list
bool compare_list(const std::list<int>& first, const std::list<int>& second)
{
    auto iter_first = first.begin();
    auto iter_second = second.begin();

    for (int i = 0; i < first.size(); i ++)
    {
        int f = *iter_first;
        int s = *iter_second;

        if (f < s)
        {
            return true;
        }
        else if (s < f)
        {
            return false;
        }
        iter_first = std::next(iter_first, 1);
        iter_second = std::next(iter_second, 1);
    }

    return false;
}

// get the index of a value in a list
int get_index(const std::list<int>& elem, const int& value)
{
    auto iter = elem.begin();
    for (int i = 0; i < elem.size(); i ++)
    {
        if (value == *iter)
        {
            return i;
        }
        iter = std::next(iter, 1);
    }
    return -1;
}

void get_subset(std::vector<std::vector<int>>& subsets, int n, int sub_n, int index, std::vector<int>& set, std::vector<int>& current_subset)
{
    if (n < sub_n) return;
    if (sub_n == 0) {
        subsets.push_back(current_subset);
        return;
    }
    get_subset(subsets, n-1, sub_n, index+1, set, current_subset);
    current_subset.push_back(set[index]);
    get_subset (subsets, n-1, sub_n-1, index+1, set, current_subset);
    current_subset.pop_back();
}


// return all distinct subsets of given size of a set
std::vector<std::vector<int>> combinations(std::vector<int>& set, int sub_n)
{
    std::vector<std::vector<int>> subsets;
    std::vector<int> current_subset;
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

