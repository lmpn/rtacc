#include <catch2/catch_test_macros.hpp>

int factorial(int input) noexcept
{
  int result = 1;

  while (input > 0) {
    result *= input;
    --input;
  }

  return result;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
  REQUIRE(factorial(0) == 1);
  REQUIRE(factorial(1) == 1);
  REQUIRE(factorial(2) == 2);
  REQUIRE(factorial(3) == 6);
  REQUIRE(factorial(10) == 3628800);
}
