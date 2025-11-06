#include <cppunit/extensions/HelperMacros.h>

#include "json.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class SerializeTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SerializeTest);

    CPPUNIT_TEST(TestBoolFalse);
    CPPUNIT_TEST(TestBoolTrue);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNumberInteger);
    CPPUNIT_TEST(TestNumberDouble);
    CPPUNIT_TEST(TestString);
    CPPUNIT_TEST(TestArray);
    CPPUNIT_TEST(TestSortedObject);

    CPPUNIT_TEST_SUITE_END();

protected:
    void TestBoolFalse();
    void TestBoolTrue();
    void TestNull();
    void TestNumberInteger();
    void TestNumberDouble();
    void TestString();
    void TestArray();
    void TestSortedObject();
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(SerializeTest);

// =============================================================================

void SerializeTest::TestBoolFalse()
{
    std::stringstream ss;
    mj::JsonNode node{true};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
}

// =============================================================================

void SerializeTest::TestBoolTrue()
{
    std::stringstream ss;
    mj::JsonNode node{false};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
}

// =============================================================================

void SerializeTest::TestNull()
{
    std::stringstream ss;
    mj::JsonNode node{nullptr};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
}

// =============================================================================

void SerializeTest::TestNumberInteger()
{
    std::stringstream ss;
    mj::JsonNode node{123};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("123"), ss.str());
}

// =============================================================================

void SerializeTest::TestNumberDouble()
{
    std::stringstream ss;
    mj::JsonNode node{123.456};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("123.456"), ss.str());
}

// =============================================================================

void SerializeTest::TestString()
{
    std::stringstream ss;
    mj::JsonNode node{"hello, json"};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("\"hello, json\""), ss.str());
}

// =============================================================================

void SerializeTest::TestArray()
{
    std::stringstream ss;
    mj::JsonNode node{JsonArray{
        1, "2345", 3.14, true, false, nullptr,
        JsonArray{"hello", "world"},
    }};
    node.PrintToStream(ss, SerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("[1,\"2345\",3.14,true,false,null,[\"hello\",\"world\"]]"), ss.str());
}

// =============================================================================

void SerializeTest::TestSortedObject()
{
    std::stringstream ss;
    mj::JsonNode node{JsonObject{
        std::make_pair("cherry", 3),
        std::make_pair("apple", 1),
        std::make_pair("banana", 2),
    }};
    node.PrintToStream(ss, SerializeOptions{.sort_keys=true});
    CPPUNIT_ASSERT_EQUAL(std::string("{\"apple\":1,\"banana\":2,\"cherry\":3}"), ss.str());
}

// =============================================================================


} // namespace mj::test

// =============================================================================
