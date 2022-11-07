#include <utils/RandomUtils.h>

std::string RandomUtils::getRandomNumberStr(int n)
{
    time_t t = (time_t)rand();
    srand((unsigned)time(&t));

    std::vector<int> vec;

    for (int i = 0; i < n; i++)
    {
        vec.push_back(rand() % 10);
    }

    std::ostringstream s;

    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(s, ""));
    return s.str();
}