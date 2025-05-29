#include "project/version.hpp"
#include "project/lib.hpp"

#include <iostream>
#include <vector>
#include <tuple>

int main() {
    std::cout << "Project Version: " << PROJECT_VERSION << std::endl;

    std::vector<double> input = {1.2, 2.3, 3.4, 4.5};

    auto [sum, avg] = accumulate_vector(input);

    std::cout << "Sum: " << sum << ", Average: " << avg << std::endl;

    return 0;
}
