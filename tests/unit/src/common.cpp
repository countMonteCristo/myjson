#include <cmath>

// =============================================================================

namespace mj::test
{

// =============================================================================

bool AlmostEqual(double expected, double actual)
{
    static constexpr double EPSILON = 1e-9;
    return std::fabs(expected - actual) < EPSILON;
}

// =============================================================================

} // namespace mj::test
