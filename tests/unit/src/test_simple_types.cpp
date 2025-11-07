#include <cppunit/extensions/HelperMacros.h>

#include "json.hpp"
#include "visitor.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class SimpleTypesTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SimpleTypesTest);

    CPPUNIT_TEST(TestBoolFalse);
    CPPUNIT_TEST(TestBoolTrue);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNumberInteger);
    CPPUNIT_TEST(TestNumberDouble);
    CPPUNIT_TEST(TestString);

    CPPUNIT_TEST_SUITE_END();

protected:
    void TestBoolFalse();
    void TestBoolTrue();
    void TestNull();
    void TestNumberInteger();
    void TestNumberDouble();
    void TestString();
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleTypesTest);

// =============================================================================

void SimpleTypesTest::TestBoolFalse()
{
    mj::JsonNode node{true};
    CPPUNIT_ASSERT_EQUAL(true, node.AsBool());
}

// =============================================================================

void SimpleTypesTest::TestBoolTrue()
{
    mj::JsonNode node{false};
    CPPUNIT_ASSERT_EQUAL(false, node.AsBool());
}

// =============================================================================

void SimpleTypesTest::TestNull()
{
    mj::JsonNode node{nullptr};
    CPPUNIT_ASSERT_EQUAL(nullptr, node.AsNull());
}

// =============================================================================

void SimpleTypesTest::TestNumberInteger()
{
    mj::JsonNode node{123};
    CPPUNIT_ASSERT_EQUAL(123, node.AsNumber().To<int>());
}

// =============================================================================

void SimpleTypesTest::TestNumberDouble()
{
    mj::JsonNode node{123.456};
    CPPUNIT_ASSERT(AlmostEqual(123.456, node.AsNumber()));
}

// =============================================================================

void SimpleTypesTest::TestString()
{
    mj::JsonNode node{"hello, json"};
    CPPUNIT_ASSERT_EQUAL(std::string("hello, json"), node.AsString());
}

// =============================================================================

} // namespace mj::test

// =============================================================================
