#include <cppunit/extensions/HelperMacros.h>

#include "parser.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class CustomTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CustomTest);

    CPPUNIT_TEST(TestCheckBigNumbers);

    CPPUNIT_TEST_SUITE_END();

protected:
    void TestCheckBigNumbers();
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(CustomTest);

// =============================================================================

void CustomTest::TestCheckBigNumbers()
{
    std::string s = "{\"x\":199059818}";
    auto n = mj::ParseFrom(s);

    std::stringstream ss;
    n.SerializeToStream(ss);
    CPPUNIT_ASSERT_EQUAL(s, ss.str());
}
// =============================================================================

} // namespace mj::test

// =============================================================================
