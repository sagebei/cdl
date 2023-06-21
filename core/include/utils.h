#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <array>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <functional>
#include <numeric>
#include <utility>


typedef std::uint_fast16_t Int8;
typedef std::uint32_t Int32;
typedef std::list<Int8> IntList;
typedef std::array<Int8, 3> Triplet;

struct TripletRule
{
    Triplet triplet;
    Int8 rule_id;
};

typedef std::vector<TripletRule> TRS;
typedef std::map<Triplet, Int32> TripletIndex;

typedef std::list<IntList> CD;
typedef std::list<CD> CDS;

struct TripletHasher
{
    std::size_t operator()(const Triplet& arr) const;
};

bool compare_permutations(const IntList& first, const IntList& second);
bool compare_cds(CD& first_cd, CD& second_cd);
int get_index(const IntList& elem, const Int8& value);
void get_subset(std::vector<std::vector<Int8>>& subsets, Int8 n, Int8 sub_n, Int32 index, std::vector<Int8>& set, std::vector<Int8>& current_subset);
std::vector<std::vector<Int8>> combinations(std::vector<Int8>& set, Int8 sub_n);

int benchmark_size(int n);
std::string Fishburn_scheme(const Triplet& triplet);




