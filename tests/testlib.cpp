
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <project/lib.hpp>

TEST_CASE( "Quick check", "[main]" ) {
    std::vector<double> values {1, 2., 3.};
    auto [sum, avg] = accumulate_vector(values);
    REQUIRE( sum == 6.0 );
    REQUIRE( avg == Approx(2.0) );
}
