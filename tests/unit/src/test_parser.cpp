#include <cppunit/extensions/HelperMacros.h>

#include <cmath>

#include "parser.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class ParserTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ParserTest);

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
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(ParserTest);

// =============================================================================

void ParserTest::TestBool()
{
    CPPUNIT_ASSERT_EQUAL(true, ParseFrom("true").AsBool());
    CPPUNIT_ASSERT_EQUAL(false, ParseFrom("false").AsBool());
    CPPUNIT_ASSERT_EQUAL(true, ParseFrom(" \r \t \n true \t \n \r  ").AsBool());
    CPPUNIT_ASSERT_EQUAL(false, ParseFrom(" \t\n \r  false  \r \t\n  ").AsBool());
    CPPUNIT_ASSERT_THROW(ParseFrom("trues"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("falses"), JsonException);
}

// =============================================================================

void ParserTest::TestNull()
{
    CPPUNIT_ASSERT_EQUAL(nullptr, ParseFrom("null").AsNull());
    CPPUNIT_ASSERT_EQUAL(nullptr, ParseFrom("  \r \t \n null \t\r\n").AsNull());
    CPPUNIT_ASSERT_THROW(ParseFrom("nulls"), JsonException);
}

// =============================================================================

void ParserTest::TestNumber()
{
    CPPUNIT_ASSERT_EQUAL(123, ParseFrom("123").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, ParseFrom("-123").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(123, ParseFrom("  \r\n\t 123 \t \r\n ").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, ParseFrom("\r  \n  \t -123 \r \t\n  ").AsNumber().To<int>());
    CPPUNIT_ASSERT(AlmostEqual(3.1415, ParseFrom("3.1415").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-3.1415, ParseFrom("-3.1415").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e-4, ParseFrom("2.3e-4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e-4, ParseFrom("-2.3e-4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e4, ParseFrom("2.3e4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e4, ParseFrom("-2.3e4").AsNumber().To<double>()));

    CPPUNIT_ASSERT_EQUAL(true, std::isinf(ParseFrom("Infinity").AsNumber().To<double>()));
    CPPUNIT_ASSERT_EQUAL(true, std::isinf(ParseFrom("-Infinity").AsNumber().To<double>()));
    CPPUNIT_ASSERT_EQUAL(true, std::isnan(ParseFrom("NaN").AsNumber().To<double>()));
}

// =============================================================================

void ParserTest::TestString()
{
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), ParseFrom("\"hello json\"").AsString());
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), ParseFrom("  \t \r\n\"hello json\"\r\n\t").AsString());
    CPPUNIT_ASSERT_THROW(ParseFrom("\"hello"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("\"he\"he\"he\"llo\""), JsonException);
}

// =============================================================================

void ParserTest::TestEmptyArray()
{
    JsonNode node = ParseFrom("[]");
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), array.Size());
    CPPUNIT_ASSERT_THROW(ParseFrom("\r[ \n  , \n] \t"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("\r[ \n  ,, \n] \t"), JsonException);
}

// =============================================================================

void ParserTest::TestPlainArray()
{
    JsonNode node = ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4, NaN, -Infinity, Infinity ] ");
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(9), array.Size());
    CPPUNIT_ASSERT_EQUAL(1, array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, array[1].AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array[2].AsString());
    CPPUNIT_ASSERT_EQUAL(true, array[3].AsBool());
    CPPUNIT_ASSERT_EQUAL(false, array[4].AsBool());
    CPPUNIT_ASSERT(AlmostEqual(3.4, array[5].AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, std::isnan(array[6].AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, std::isinf(array[7].AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, std::isinf(array[8].AsNumber()));

    CPPUNIT_ASSERT_THROW(ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4, ] "),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,,2]"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[,1,,2]"), JsonException);
}

// =============================================================================

void ParserTest::TestComplexArray()
{
    JsonNode node = ParseFrom(" \r \n[ 1 , null\n ,[\"hello\", false, NaN] ,\"hello\" \r , {\"k1\": true}, false  \r,3.4 ] ");
    const JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(7), array.Size());
    CPPUNIT_ASSERT_EQUAL(1, array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, array[1].AsNull());

    const JsonArray& child_array = array[2].AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), child_array[0].AsString());
    CPPUNIT_ASSERT_EQUAL(false, child_array[1].AsBool());
    CPPUNIT_ASSERT_EQUAL(true, std::isnan(child_array[2].AsNumber()));

    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array[3].AsString());

    const JsonObject& child_object = array[4].AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(true, child_object.Get("k1").AsBool());

    CPPUNIT_ASSERT_EQUAL(false, array[5].AsBool());
    CPPUNIT_ASSERT(AlmostEqual(3.4, array[6].AsNumber()));

    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,3],]"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,,3]]"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("[1,[2,[4,5,6,],3]]"), JsonException);
}

// =============================================================================

void ParserTest::TestEmptyObject()
{
    JsonNode node = ParseFrom("{}");
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), object.Size());
    CPPUNIT_ASSERT_THROW(ParseFrom("\r{} \n  , \n} \t"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,}"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,,}"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{:}"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{:,}"), JsonException);
}

// =============================================================================

void ParserTest::TestPlainObject()
{
    JsonNode node = ParseFrom("{\"k1\" : true, \"k2\": 42, \"k3\": \"string\", \"k4\": Infinity, \"k5\": NaN, \"k6\": -Infinity}");
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());
    CPPUNIT_ASSERT_EQUAL(42, object.Get("k2").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("string"), object.Get("k3").AsString());
    CPPUNIT_ASSERT_EQUAL(true, std::isinf(object.Get("k4").AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, std::isnan(object.Get("k5").AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, std::isinf(object.Get("k6").AsNumber()));

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": 42, }"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{,\"k1\" : true, \"k2\": 42 }"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": 42 : }"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" :: true, \"k2\": 42 }"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" , true, \"k2\": 42}"), JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true \"k2\": 42}"), JsonException);
}

// =============================================================================

void ParserTest::TestComplexObject()
{
    JsonNode node = ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6, \"k6\": NaN}, \"k3\": [1,null]}");
    const JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());

    const JsonObject& child_object = object.Get("k2").AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(false, child_object.Get("k4").AsBool());
    CPPUNIT_ASSERT_EQUAL(6, child_object.Get("k5").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(true, std::isnan(child_object.Get("k6").AsNumber()));

    const JsonArray& child_array = object.Get("k3").AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(1, child_array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array[1].AsNull());

    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\":, false, \"k5\": 6}, \"k3\": [1,null]}"),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6},, \"k3\": [1,null]}"),
                         JsonException);
    CPPUNIT_ASSERT_THROW(ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6}: \"k3\": [1,null]}"),
                         JsonException);
}

// =============================================================================

} // namespace mj::test

// =============================================================================
