#include <iostream>
#include "condorcet_domain.h"
#include "tools.h"


int main()
{
//    CondorcetDomain cd(5);
//    RuleScheme rs;
//    rs.add({2, 3, 4}, "3N1");

    DataStorage ds;

//    DATABASE database = build_full_trs_cd(5);
//    ds.save_database(database, "full_trs_size.map");
    ds.load_database("/Users/Bei/CLionProjects/cdl/tools/full_trs_size.map");

//    DATABASE db = load_database("../bind/full_trs_size.map");
//    std::vector<int> state = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::cout << ds.fetch_value({1, 1, 1, 1, 1, 1, 1, 1, 1, 1}) << std::endl;
//    for (int i = 0; i < cd.num_triplets; i++)
//    {
//        TRS trs = cd.init_by_scheme(rs);
//        trs = cd.assign_by_index(trs, i, "");
//        print_trs(trs);
//
//        for (const std::string& r0: rules)
//        {
//            trs = cd.assign_by_index(trs, 0, r0);
//            for (const std::string& r1: rules)
//            {
//                trs = cd.assign_by_index(trs, 1, r1);
//                for (const std::string& r2: rules)
//                {
//                    trs = cd.assign_by_index(trs, 2, r2);
//                    for (const std::string& r3: rules)
//                    {
//                        trs = cd.assign_by_index(trs, 3, r3);
//                        for (const std::string& r4: rules)
//                        {
//                            trs = cd.assign_by_index(trs, 4, r4);
//                            for (const std::string& r5: rules)
//                            {
//                                trs = cd.assign_by_index(trs, 5, r5);
//                                for (const std::string& r6: rules)
//                                {
//                                    trs = cd.assign_by_index(trs, 6, r6);
//                                    for (const std::string& r7: rules)
//                                    {
//                                        trs = cd.assign_by_index(trs, 7, r7);
//                                        for (const std::string& r8: rules)
//                                        {
//                                            trs = cd.assign_by_index(trs, 8, r8);
//                                            for (const std::string& r9: rules) {
//                                                trs = cd.assign_by_index(trs, 9, r9);
//                                                int size = cd.condorcet_domain(trs).size();
//                                                std::vector<int> state = cd.trs_to_state(trs);
//
//                                                database[state] = size;
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

//    save_database(database, "missing_1_trs_size.map");
//
//
//    std::cout << db.size() << std::endl;

    return 0;
}