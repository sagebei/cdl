#include "../headers/utils.h"
#include <iostream>


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





