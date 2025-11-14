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

// TODO: remove  in test functions
void SerializeTest::TestBoolFalse()
{
    std::stringstream ss;
    JsonNode node{true};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
}

// =============================================================================

void SerializeTest::TestBoolTrue()
{
    std::stringstream ss;
    JsonNode node{false};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
}

// =============================================================================

void SerializeTest::TestNull()
{
    std::stringstream ss;
    JsonNode node{nullptr};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
}

// =============================================================================

void SerializeTest::TestNumberInteger()
{
    std::stringstream ss;
    JsonNode node{123};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("123"), ss.str());
}

// =============================================================================

void SerializeTest::TestNumberDouble()
{
    std::stringstream ss;
    JsonNode node{123.456};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("123.456"), ss.str());
}

// =============================================================================

void SerializeTest::TestString()
{
    std::stringstream ss;
    JsonNode node{"hello, json"};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("\"hello, json\""), ss.str());
}

// =============================================================================

void SerializeTest::TestArray()
{
    std::stringstream ss;
    JsonNode node{JsonArray{
        1, "2345", 3.14, true, false, nullptr,
        JsonArray{"hello", "world"},
    }};
    node.SerializeToStream(ss, JsonSerializeOptions{});
    CPPUNIT_ASSERT_EQUAL(std::string("[1,\"2345\",3.14,true,false,null,[\"hello\",\"world\"]]"), ss.str());
}

// =============================================================================

void SerializeTest::TestSortedObject()
{
    std::stringstream ss;
    JsonNode node{JsonObject{
        std::make_pair("cherry", 3),
        std::make_pair("apple", 1),
        std::make_pair("banana", 2),
    }};
    node.SerializeToStream(ss, JsonSerializeOptions{.sort_keys=true});
    CPPUNIT_ASSERT_EQUAL(std::string("{\"apple\":1,\"banana\":2,\"cherry\":3}"), ss.str());
}

// =============================================================================


} // namespace mj::test

// =============================================================================
