#pragma once

#include <list>
#include <vector>
#include <array>
#include <cinttypes>

typedef std::uint8_t Int8;
typedef std::uint32_t Int32;
typedef std::list<Int8> IntList;
typedef std::array<Int8, 3> Triplet;

struct TripletHasher
{
    std::size_t operator()(const Triplet& arr) const;
};

bool compare_list(const IntList& a, const IntList& b);
int get_index(const IntList& elem, const Int8& value);
void get_subset(std::vector<std::vector<Int8>>& subsets, Int8 n, Int8 sub_n, Int32 index, std::vector<Int8>& set, std::vector<Int8>& current_subset);
std::vector<std::vector<Int8>> combinations(std::vector<Int8>& set, Int8 sub_n);

int benchmark_size(int n);





