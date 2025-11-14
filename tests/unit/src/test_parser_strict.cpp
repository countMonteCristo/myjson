#include <cppunit/extensions/HelperMacros.h>

#include "parser.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class ParserStrictTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ParserStrictTest);

    CPPUNIT_TEST(TestBool);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNumber);
    CPPUNIT_TEST(TestString);
    CPPUNIT_TEST(TestEmptyArray);
    CPPUNIT_TEST(TestPlainArray);
    CPPUNIT_TEST(TestComplexArray);
    CPPUNIT_TEST(TestEmptyObject);
    CPPUNIT_TEST(TestPlainObject);
    CPPUNIT_TEST(TestComplexObject);

    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override { options.strict = true; }

protected:
    void TestBool();
    void TestNull();
    void TestNumber();
    void TestString();
    void TestEmptyArray();
    void TestPlainArray();
    void TestComplexArray();
    void TestEmptyObject();
    void TestPlainObject();
    void TestComplexObject();

private:
    JsonDeserializeOptions options;
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(ParserStrictTest);

// =============================================================================

void ParserStrictTest::TestBool()
{
    CPPUNIT_ASSERT_EQUAL(true, ParseFrom("true", options).AsBool());
    CPPUNIT_ASSERT_EQUAL(false, ParseFrom("false", options).AsBool());
    CPPUNIT_ASSERT_EQUAL(true, ParseFrom(" \r \t \n true \t \n \r  ", options).AsBool());
    CPPUNIT_ASSERT_EQUAL(false, ParseFrom(" \t\n \r  false  \r \t\n  ", options).AsBool());
    CPPUNIT_ASSERT_THROW(ParseFrom("trues", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("falses", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestNull()
{
    CPPUNIT_ASSERT_EQUAL(nullptr, ParseFrom("null", options).AsNull());
    CPPUNIT_ASSERT_EQUAL(nullptr, ParseFrom("  \r \t \n null \t\r\n", options).AsNull());
    CPPUNIT_ASSERT_THROW(ParseFrom("nulls", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestNumber()
{
    CPPUNIT_ASSERT_EQUAL(123, ParseFrom("123", options).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, ParseFrom("-123", options).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(123, ParseFrom("  \r\n\t 123 \t \r\n ", options).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, ParseFrom("\r  \n  \t -123 \r \t\n  ", options).AsNumber().To<int>());
    CPPUNIT_ASSERT(AlmostEqual(3.1415, ParseFrom("3.1415", options).AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-3.1415, ParseFrom("-3.1415", options).AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e-4, ParseFrom("2.3e-4", options).AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e-4, ParseFrom("-2.3e-4", options).AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e4, ParseFrom("2.3e4", options).AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e4, ParseFrom("-2.3e4", options).AsNumber().To<double>()));

    CPPUNIT_ASSERT_THROW(ParseFrom("Infinity", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("-Infinity", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("NaN", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestString()
{
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), ParseFrom("\"hello json\"", options).AsString());
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), ParseFrom("  \t \r\n\"hello json\"\r\n\t", options).AsString());
    CPPUNIT_ASSERT_THROW(ParseFrom("\"hello", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("\"he\"he\"he\"llo\"", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestEmptyArray()
{
    JsonNode node = ParseFrom("[]", options);
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), array.Size());
    CPPUNIT_ASSERT_THROW(ParseFrom("\r[ \n  , \n] \t", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("\r[ \n  ,, \n] \t", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestPlainArray()
{
    JsonNode node = ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4 ] ", options);
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), array.Size());
    CPPUNIT_ASSERT_EQUAL(1, array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, array[1].AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array[2].AsString());
    CPPUNIT_ASSERT_EQUAL(true, array[3].AsBool());
    CPPUNIT_ASSERT_EQUAL(false, array[4].AsBool());
    CPPUNIT_ASSERT(AlmostEqual(3.4, array[5].AsNumber()));

    CPPUNIT_ASSERT_THROW(ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4, ] ", options),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,,2]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[,1,,2]", options), JsonException);

    CPPUNIT_ASSERT_THROW(ParseFrom("[1,NaN,2]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,2,Infinity]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,-Infinity, 3]", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestComplexArray()
{
    JsonNode node = ParseFrom(" \r \n[ 1 , null\n ,[\"hello\", false] ,\"hello\" \r , {\"k1\": true}, false  \r,3.4 ] ",
                                      options);
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), array.Size());
    CPPUNIT_ASSERT_EQUAL(1, array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, array[1].AsNull());

    const JsonArray& child_array = array[2].AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), child_array[0].AsString());
    CPPUNIT_ASSERT_EQUAL(false, child_array[1].AsBool());

    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array[3].AsString());

    const JsonObject& child_object = array[4].AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(true, child_object.Get("k1").AsBool());

    CPPUNIT_ASSERT_EQUAL(false, array[5].AsBool());
    CPPUNIT_ASSERT(AlmostEqual(3.4, array[6].AsNumber()));

    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,3],]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,,3]]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,[4,5,6,],3]]", options), JsonException);

    CPPUNIT_ASSERT_THROW(ParseFrom("[\"a\", [NaN]]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[\"a\", [Infinity]]", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[\"a\", [-Infinity]]", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestEmptyObject()
{
    JsonNode node = ParseFrom("{}", options);
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), object.Size());
    CPPUNIT_ASSERT_THROW(ParseFrom("\r{} \n  , \n} \t", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,}", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,,}", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{:}", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{:,}", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestPlainObject()
{
    JsonNode node = ParseFrom("{\"k1\" : true, \"k2\": 42, \"k3\": \"string\"}", options);
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());
    CPPUNIT_ASSERT_EQUAL(42, object.Get("k2").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("string"), object.Get("k3").AsString());

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": 42, }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,\"k1\" : true, \"k2\": 42 }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": 42 : }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" :: true, \"k2\": 42 }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" , true, \"k2\": 42}", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true \"k2\": 42}", options), JsonException);

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"a\": NaN }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"a\": Infinity }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"a\": -Infinity }", options), JsonException);
}

// =============================================================================

void ParserStrictTest::TestComplexObject()
{
    JsonNode node = ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6}, \"k3\": [1,null]}", options);
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());

    const JsonObject& child_object = object.Get("k2").AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(false, child_object.Get("k4").AsBool());
    CPPUNIT_ASSERT_EQUAL(6, child_object.Get("k5").AsNumber().To<int>());

    const JsonArray& child_array = object.Get("k3").AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(1, child_array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array[1].AsNull());

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\":, false, \"k5\": 6}, \"k3\": [1,null]}", options),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6},, \"k3\": [1,null]}", options),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6}: \"k3\": [1,null]}", options),
                         JsonException);

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"key\": {\"a\": NaN } }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"key\": {\"a\": Infinity } }", options), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"key\": {\"a\": -Infinity } }", options), JsonException);
}

// =============================================================================

} // namespace mj::test

// =============================================================================
