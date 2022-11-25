#pragma once

#include <list>
#include <vector>

bool compare_list(const std::list<int>& a, const std::list<int>& b);
int get_index(const std::list<int>& elem, const int& value);
int factorial(int n);
void get_subset(std::vector<std::vector<int>>& subsets, int n, int sub_n, int index, std::vector<int>& set, std::vector<int>& current_subset);
std::vector<std::vector<int>> combinations(std::vector<int>& set, int sub_n);





