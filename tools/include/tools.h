#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <boost/version.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include "condorcet_domain.h"

struct VectorHasher
{
    int operator()(const std::vector<int>& vec) const;
};

typedef std::unordered_map<std::vector<int>, int, VectorHasher> DATABASE;


DATABASE load_database(std::string filename);
void save_database(DATABASE database, std::string filename);

DATABASE build_full_trs_cd(int n);
void print_database(DATABASE database);

int fetch_value(const DATABASE& database, const std::vector<int>& state);