#include "utils.h"
#include <iostream>


 bool compare_list(const std::list<int>& first, const std::list<int>& second)
{
    bool result;

    for (int i = 0; i < first.size(); i ++)
    {
        int f = *std::next(first.begin(), i);
        int s = *std::next(second.begin(), i);

        if (f < s)
        {
            result = true;
            break;
        }
        else if (s < f)
        {
            result = false;
            break;
        }
    }
    return result;
}


int get_index(const std::list<int>& elem, const int& value)
{
    auto const& iter = elem.begin();
    for (int i = 0; i < elem.size(); i ++)
    {
        if (value == *std::next(iter, i))
        {
            return i;
        }
    }
    return -1;
}





