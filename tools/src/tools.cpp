#include "tools.h"


int VectorHasher::operator()(const std::vector<int>& vec) const
{
    int hash = vec.size();
    for(auto &i : vec) {
        hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

DATABASE build_full_trs_cd(int n)
{
    DATABASE database{};
    CondorcetDomain cd(n);
    TRS trs = cd.init_empty(true);
    const auto& rules = cd.rules;
    for (const std::string& r0: rules)
    {
        trs = cd.assign_by_index(trs, 0, r0);
        for (const std::string& r1: rules)
        {
            trs = cd.assign_by_index(trs, 1, r1);
            for (const std::string& r2: rules)
            {
                trs = cd.assign_by_index(trs, 2, r2);
                for (const std::string& r3: rules)
                {
                    trs = cd.assign_by_index(trs, 3, r3);
                    for (const std::string& r4: rules)
                    {
                        trs = cd.assign_by_index(trs, 4, r4);
                        for (const std::string& r5: rules)
                        {
                            trs = cd.assign_by_index(trs, 5, r5);
                            for (const std::string& r6: rules)
                            {
                                trs = cd.assign_by_index(trs, 6, r6);
                                for (const std::string& r7: rules)
                                {
                                    trs = cd.assign_by_index(trs, 7, r7);
                                    for (const std::string& r8: rules)
                                    {
                                        trs = cd.assign_by_index(trs, 8, r8);
                                        for (const std::string& r9: rules) {
                                            trs = cd.assign_by_index(trs, 9, r9);
                                            int size = cd.condorcet_domain(trs).size();
                                            std::vector<int> state = cd.trs_to_state(trs);

                                            database[state] = size;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return database;
}

DATABASE load_database(std::string filename)
{
    DATABASE database{};
    std::ifstream is(filename);
    boost::archive::text_iarchive ia(is);
    ia & database;
    return database;

}

void save_database(DATABASE database, std::string filename)
{
    std::ofstream os(filename);
    boost::archive::text_oarchive oa(os);
    oa & database;
}

void print_database(DATABASE database)
{
    for(const auto& item: database)
    {
        if (item.second == 20)
        {
            for (const int& i: item.first)
                std::cout << i;
            std::cout << " " << item.second << std::endl;
        }

    }
}

int fetch_value(DATABASE& database, const std::vector<int>& state)
{
    return database[state];
}
