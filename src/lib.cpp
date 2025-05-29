#include <project/lib.hpp>

std::tuple<double, double> accumulate_vector(const std::vector<double>& values)
{
    double sum{0};
    for(auto d : values)
    {
        sum += d;
    }

    double avg = sum / values.size();

    return {sum, avg};
}
