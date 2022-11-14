#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;

struct TripletRule
{
    std::array<int, 3> triplet;
    std::string rule;
};

std::vector<TripletRule> sort_triplet_rules(std::vector<TripletRule> triplet_rules)
{
    auto start = triplet_rules.begin();
    auto end = triplet_rules.end() - 1;

    std::vector<TripletRule> sorted_triplet_rules;
    while(true)
    {
        sorted_triplet_rules.push_back(*start);
        start++;
        if(start == end) {break;}
        sorted_triplet_rules.push_back(*end);
        end--;
        if(start == end) {break;}
    }
    return sorted_triplet_rules;

}

std::vector<TripletRule> initialize(int n=5, bool sort=true)
{
    std::vector<TripletRule> triplet_rules;
    std::array<std::string, 4> rules = {"1N3", "3N1", "2N3", "2N1"};

    for (int i = 1; i <= n-2; i++)
    {
        for (int j = i+1; j <= n-1; j++)
        {
            for (int k = j+1; k <= n; k++)
            {
                TripletRule tr;
                tr.triplet = {{i, j, k}};
                tr.rule = rules[1];
                triplet_rules.push_back(tr);

            }
        }
    }
    if(sort)
    {
        triplet_rules = sort_triplet_rules(triplet_rules);
    }
    return triplet_rules;
}

void print_tr(std::vector<TripletRule>  triplet_rules)
{
    for (auto const& tr: triplet_rules)
    {
        std::cout<<tr.triplet[0]<< tr.triplet[1] << tr.triplet[2] << " : " << tr.rule << std::endl;
    }

}

PYBIND11_MAKE_OPAQUE(std::vector<TripletRule>)
PYBIND11_MODULE(condorcet, m)
{
    m.doc() = "core functionality for calculating the condorcet domain";

//    py::class_<TripletRule>(m, "TripletRule")
//            .def(py::init<>())
//            .def_readwrite("triplet", &TripletRule::triplet)
//            .def_readwrite("rule", &TripletRule::rule);
    py::bind_vector<std::vector<TripletRule>>(m, "TripletRules");
    m.def("initialize", &initialize);
}