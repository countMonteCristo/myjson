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
    // std::stringstream ss;
    // mj::JsonBool b{true};
    mj::JsonNode node{true};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
    CPPUNIT_ASSERT_EQUAL(true, node.AsBool());
}

// =============================================================================

void SimpleTypesTest::TestBoolTrue()
{
    // std::stringstream ss;
    // mj::JsonBool b{false};
    mj::JsonNode node{false};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
    CPPUNIT_ASSERT_EQUAL(false, node.AsBool());
}

// =============================================================================

void SimpleTypesTest::TestNull()
{
    // std::stringstream ss;
    // mj::JsonNull n{nullptr};
    mj::JsonNode node{nullptr};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
    CPPUNIT_ASSERT_EQUAL(nullptr, node.AsNull());
}

// =============================================================================

void SimpleTypesTest::TestNumberInteger()
{
    // std::stringstream ss;
    // mj::JsonNumber n{123};
    mj::JsonNode node{123};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("123"), ss.str());
    CPPUNIT_ASSERT_EQUAL(123, node.AsNumber().To<int>());
}

// =============================================================================

void SimpleTypesTest::TestNumberDouble()
{
    // std::stringstream ss;
    // mj::JsonNumber n{123.456};
    mj::JsonNode node{123.456};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("123.456"), ss.str());
    CPPUNIT_ASSERT(AlmostEqual(123.456, node.AsNumber()));
}

// =============================================================================

void SimpleTypesTest::TestString()
{
    // std::stringstream ss;
    // mj::JsonString n{"hello, json"};
    mj::JsonNode node{"hello, json"};
    // node.PrintToStream(ss);
    // CPPUNIT_ASSERT_EQUAL(std::string("\"hello, json\""), ss.str());
    CPPUNIT_ASSERT_EQUAL(std::string("hello, json"), node.AsString());
}

// =============================================================================

} // namespace mj::test

// =============================================================================
