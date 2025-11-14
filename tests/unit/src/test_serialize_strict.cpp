#include <cppunit/extensions/HelperMacros.h>

#include "json.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class SerializeStrictTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SerializeStrictTest);

    CPPUNIT_TEST(TestBoolFalse);
    CPPUNIT_TEST(TestBoolTrue);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNumberInteger);
    CPPUNIT_TEST(TestNumberDouble);
    CPPUNIT_TEST(TestNumberInfinite);
    CPPUNIT_TEST(TestString);
    CPPUNIT_TEST(TestArray);
    CPPUNIT_TEST(TestSortedObject);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override { options.strict = true; }

protected:
    void TestBoolFalse();
    void TestBoolTrue();
    void TestNull();
    void TestNumberInteger();
    void TestNumberDouble();
    void TestNumberInfinite();
    void TestString();
    void TestArray();
    void TestSortedObject();

private:
    JsonSerializeOptions options;
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(SerializeStrictTest);

// =============================================================================

void SerializeStrictTest::TestBoolFalse()
{
    std::stringstream ss;
    JsonNode node{true};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("true"), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestBoolTrue()
{
    std::stringstream ss;
    JsonNode node{false};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("false"), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestNull()
{
    std::stringstream ss;
    JsonNode node{nullptr};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("null"), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestNumberInteger()
{
    std::stringstream ss;
    JsonNode node{123};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("123"), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestNumberDouble()
{
    std::stringstream ss;
    JsonNode node{123.456};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("123.456"), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestNumberInfinite()
{
    std::stringstream ss;
    JsonNode node1{std::numeric_limits<double>::infinity()};
    CPPUNIT_ASSERT_THROW(node1.SerializeToStream(ss, options), JsonException);

    JsonNode node2{-std::numeric_limits<double>::infinity()};
    CPPUNIT_ASSERT_THROW(node2.SerializeToStream(ss, options), JsonException);

    JsonNode node3{std::numeric_limits<double>::quiet_NaN()};
    CPPUNIT_ASSERT_THROW(node3.SerializeToStream(ss, options), JsonException);
}

// =============================================================================

void SerializeStrictTest::TestString()
{
    std::stringstream ss;
    JsonNode node{"hello, json"};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("\"hello, json\""), ss.str());
}

// =============================================================================

void SerializeStrictTest::TestArray()
{
    std::stringstream ss;
    JsonNode node{JsonArray{
        1, "2345", 3.14, true, false, nullptr,
        JsonArray{"hello", "world"},
    }};
    node.SerializeToStream(ss, options);
    CPPUNIT_ASSERT_EQUAL(std::string("[1,\"2345\",3.14,true,false,null,[\"hello\",\"world\"]]"), ss.str());

    JsonNode bad_node{JsonArray{
        1, "2345", 3.14, true, false, nullptr,
        std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        JsonArray{"hello", "world"},
    }};
    CPPUNIT_ASSERT_THROW(bad_node.SerializeToStream(ss, options), JsonException);
}

// =============================================================================

void SerializeStrictTest::TestSortedObject()
{
    std::stringstream ss;
    JsonNode node{JsonObject{
        std::make_pair("cherry", 3),
        std::make_pair("apple", 1),
        std::make_pair("banana", 2),
    }};
    node.SerializeToStream(ss, JsonSerializeOptions{.sort_keys=true});
    CPPUNIT_ASSERT_EQUAL(std::string("{\"apple\":1,\"banana\":2,\"cherry\":3}"), ss.str());

    JsonNode bad_node{JsonObject{
        std::make_pair("cherry", 3),
        std::make_pair("apple", std::numeric_limits<double>::infinity()),
        std::make_pair("banana", 2),
    }};
    CPPUNIT_ASSERT_THROW(bad_node.SerializeToStream(ss, options), JsonException);
}

// =============================================================================


} // namespace mj::test

// =============================================================================
