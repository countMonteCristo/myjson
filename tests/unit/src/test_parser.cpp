#include <cppunit/extensions/HelperMacros.h>

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
    CPPUNIT_ASSERT_EQUAL(true, mj::ParseFrom("true").AsBool());
    CPPUNIT_ASSERT_EQUAL(false, mj::ParseFrom("false").AsBool());
    CPPUNIT_ASSERT_EQUAL(true, mj::ParseFrom(" \r \t \n true \t \n \r  ").AsBool());
    CPPUNIT_ASSERT_EQUAL(false, mj::ParseFrom(" \t\n \r  false  \r \t\n  ").AsBool());
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("trues"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("falses"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestNull()
{
    CPPUNIT_ASSERT_EQUAL(nullptr, mj::ParseFrom("null").AsNull());
    CPPUNIT_ASSERT_EQUAL(nullptr, mj::ParseFrom("  \r \t \n null \t\r\n").AsNull());
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("nulls"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestNumber()
{
    CPPUNIT_ASSERT_EQUAL(123, mj::ParseFrom("123").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, mj::ParseFrom("-123").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(123, mj::ParseFrom("  \r\n\t 123 \t \r\n ").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(-123, mj::ParseFrom("\r  \n  \t -123 \r \t\n  ").AsNumber().To<int>());
    CPPUNIT_ASSERT(AlmostEqual(3.1415, mj::ParseFrom("3.1415").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-3.1415, mj::ParseFrom("-3.1415").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e-4, mj::ParseFrom("2.3e-4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e-4, mj::ParseFrom("-2.3e-4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(2.3e4, mj::ParseFrom("2.3e4").AsNumber().To<double>()));
    CPPUNIT_ASSERT(AlmostEqual(-2.3e4, mj::ParseFrom("-2.3e4").AsNumber().To<double>()));
}

// =============================================================================

void ParserTest::TestString()
{
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), mj::ParseFrom("\"hello json\"").AsString());
    CPPUNIT_ASSERT_EQUAL(std::string("hello json"), mj::ParseFrom("  \t \r\n\"hello json\"\r\n\t").AsString());
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("\"hello"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("\"he\"he\"he\"llo\""), mj::JsonException);
}

// =============================================================================

void ParserTest::TestEmptyArray()
{
    mj::JsonNode node = mj::ParseFrom("[]");
    const mj::JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), array.Size());
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("\r[ \n  , \n] \t"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("\r[ \n  ,, \n] \t"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestPlainArray()
{
    mj::JsonNode node = mj::ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4 ] ");
    const mj::JsonArray& array = node.AsArray();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), array.Size());
    CPPUNIT_ASSERT_EQUAL(1, array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, array[1].AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array[2].AsString());
    CPPUNIT_ASSERT_EQUAL(true, array[3].AsBool());
    CPPUNIT_ASSERT_EQUAL(false, array[4].AsBool());
    CPPUNIT_ASSERT(AlmostEqual(3.4, array[5].AsNumber()));

    CPPUNIT_ASSERT_THROW(mj::ParseFrom(" \r \n[ 1 , null\n ,\"hello\" \r , true,false  \r,3.4, ] "),
                         mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("[1,,2]"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("[,1,,2]"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestComplexArray()
{
    mj::JsonNode node = mj::ParseFrom(" \r \n[ 1 , null\n ,[\"hello\", false] ,\"hello\" \r , {\"k1\": true}, false  \r,3.4 ] ");
    const mj::JsonArray& array = node.AsArray();

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

    CPPUNIT_ASSERT_THROW(mj::ParseFrom("[1,[2,3],]"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("[1,[2,,3]]"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("[1,[2,[4,5,6,],3]]"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestEmptyObject()
{
    mj::JsonNode node = mj::ParseFrom("{}");
    const mj::JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), object.Size());
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("\r{} \n  , \n} \t"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{,}"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{,,}"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{:}"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{:,}"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestPlainObject()
{
    mj::JsonNode node = mj::ParseFrom("{\"k1\" : true, \"k2\": 42, \"k3\": \"string\"}");
    const mj::JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());
    CPPUNIT_ASSERT_EQUAL(42, object.Get("k2").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("string"), object.Get("k3").AsString());

    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true, \"k2\": 42, }"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{,\"k1\" : true, \"k2\": 42 }"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true, \"k2\": 42 : }"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" :: true, \"k2\": 42 }"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" , true, \"k2\": 42}"), mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true \"k2\": 42}"), mj::JsonException);
}

// =============================================================================

void ParserTest::TestComplexObject()
{
    mj::JsonNode node = mj::ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6}, \"k3\": [1,null]}");
    const mj::JsonObject& object = node.AsObject();

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), object.Size());
    CPPUNIT_ASSERT_EQUAL(true, object.Get("k1").AsBool());

    const mj::JsonObject& child_object = object.Get("k2").AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(false, child_object.Get("k4").AsBool());
    CPPUNIT_ASSERT_EQUAL(6, child_object.Get("k5").AsNumber().To<int>());

    const mj::JsonArray& child_array = object.Get("k3").AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(1, child_array[0].AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array[1].AsNull());

    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true, \"k2\": {\"k4\":, false, \"k5\": 6}, \"k3\": [1,null]}"),
                         mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6},, \"k3\": [1,null]}"),
                         mj::JsonException);
    CPPUNIT_ASSERT_THROW(mj::ParseFrom("{\"k1\" : true, \"k2\": {\"k4\": false, \"k5\": 6}: \"k3\": [1,null]}"),
                         mj::JsonException);
}

// =============================================================================

} // namespace mj::test

// =============================================================================
