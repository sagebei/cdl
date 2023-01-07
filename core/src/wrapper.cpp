#include "wrapper.h"


TRSWrapper::TRSWrapper(CondorcetDomain cd)
{
    this->cd = cd;
    TRS empty_trs = cd.init_empty();
    for (const TripletRule& tr: empty_trs)
    {
        allowed_rules[tr.triplet] = {"1N3", "3N1"};
    }
}

void TRSWrapper::change_allowed_rules(const std::vector<Triplet>& triplets, const std::vector<std::string>& rules)
{
    for (const Triplet& triplet : triplets)
    {
        allowed_rules[triplet] = rules;
    }
}

TRS TRSWrapper::remove_rules(TRS trs, const std::vector<int>& contains)
{
    for (TripletRule& tr : trs)
    {
        Triplet& triplet = tr.triplet;
        for (int& c : contains)
        {
            if (triplet[0] == c || triplet[1] == c || triplet[2] == c)
                trs = cd.assign(trs, triplet, "");
        }
    }
}

Triplet TRSWrapper::next_unassigned_triplet(const TRS& trs)
{
    return cd.unassigned_triplets(trs)[0];
}

std::vector<std::size_t> TRSWrapper::evaluate_rules_on_triplet(const TRS& trs, Triplet triplet)
{
    std::vector<std::size_t> sizes = {};
    std::vector<std::string>& rules = allowed_rules[triplet];

    for (std::string& rule: rules)
    {
        TRS new_trs = assign(trs, triplet, rule);
        std::size_t s = condorcet_domain(new_trs).size();
        sizes.push_back(s);
    }
    return sizes;
}

Triplet TRSWrapper::dynamic_triplet_ordering(const TRS& trs)
{
    std::map<std::size_t, Triplet> size_triplet;

    std::vector<Triplet> unassigned = unassigned_triplets(trs);
    if (unassigned.empty())
        return Triplet{0, 0, 0};

    // use a single for loop
    // int max_size = -1;
    // Triplet max_triplet{};

    for (const Triplet& unassigned_triplet : unassigned)
    {
        auto sizes = evaluate_rules_on_triplet(trs, unassigned_triplet);
        auto max_size = *std::max_element(sizes.begin(), sizes.end());
        size_triplet[max_size] = unassigned_triplet;
    }
    const auto max_size_triplet_ptr = std::min_element(std::begin(size_triplet), std::end(size_triplet),
                                                       [](const std::pair<std::size_t, Triplet>& p1, const std::pair<std::size_t, Triplet>& p2)
                                                       {
                                                           return p1.first < p2.first;
                                                       });

    return max_size_triplet_ptr->second;

}


int TRSWrapper::largest_full_size(const TRS& trs)
{
    return 100000;
}