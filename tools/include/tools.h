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

DATABASE build_full_trs_cd(int n);
void print_database(DATABASE database);

class DataStorage
{
private:
    DATABASE database;
public:
    DataStorage();
    int fetch_value(const std::vector<int>& state);
    void save_database(std::string filename);
    void load_database(std::string filename);

};