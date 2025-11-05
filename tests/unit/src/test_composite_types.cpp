#include <assert.h>
#include <cmath>

#include <cppunit/extensions/HelperMacros.h>

#include "exceptions.hpp"
#include "json.hpp"
#include "visitor.hpp"

#include "common.hpp"

// =============================================================================

namespace mj::test
{

// =============================================================================

class CompositeTypesTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CompositeTypesTest);

    CPPUNIT_TEST(TestArray);
    CPPUNIT_TEST(TestObject);

    CPPUNIT_TEST(TestConstructArray);
    CPPUNIT_TEST(TestConstructObject);

    CPPUNIT_TEST_SUITE_END();

protected:
    void TestArray();
    void TestObject();

    void TestConstructArray();
    void TestConstructObject();
};

// =============================================================================

CPPUNIT_TEST_SUITE_REGISTRATION(CompositeTypesTest);

// =============================================================================

void CompositeTypesTest::TestArray()
{
    mj::JsonArray array{
        1,
        "23",
        nullptr,
        true,
        JsonArray{false, nullptr, "json"},
        JsonObject{
            std::make_pair("k0", 4),
            std::make_pair("k1", false),
            std::make_pair("k2", "world"),
        }
    };
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), array.Size());

    size_t idx = 0;
    CPPUNIT_ASSERT_EQUAL(1, array.At(idx++).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("23"), array.At(idx++).AsString());
    CPPUNIT_ASSERT_EQUAL(nullptr, array.At(idx++).AsNull());
    CPPUNIT_ASSERT_EQUAL(true, array.At(idx++).AsBool());

    const mj::JsonArray& child_array = array.At(idx++).AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(false, child_array.At(0).AsBool());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array.At(1).AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("json"), child_array.At(2).AsString());

    const mj::JsonObject& child_object = array.At(idx++).AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(4, child_object.Get("k0").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(false, child_object.Get("k1").AsBool());
    CPPUNIT_ASSERT_EQUAL(std::string("world"), child_object.Get("k2").AsString());

    size_t illegal_index = 9000;
    CPPUNIT_ASSERT_THROW(array.At(illegal_index), JsonException);
    CPPUNIT_ASSERT_NO_THROW(array[illegal_index]);
}

// =============================================================================

void CompositeTypesTest::TestObject()
{
    mj::JsonObject object{
        std::make_pair("k01", 1),
        std::make_pair("k02", false),
        std::make_pair("k03", nullptr),
        std::make_pair("k04", mj::JsonArray{1, nullptr, "23", true}),
        std::make_pair("k05", mj::JsonObject{
            std::make_pair("k06", true),
            std::make_pair("k07", 123),
            std::make_pair("k08", "456"),
            std::make_pair("k09", nullptr),
        }),
    };

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(5), object.Size());
    CPPUNIT_ASSERT_EQUAL(1, object.Get("k01").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(false, object.Get("k02").AsBool());
    CPPUNIT_ASSERT_EQUAL(nullptr, object.Get("k03").AsNull());

    const JsonArray& child_array = object.Get("k04").AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(1, child_array.At(0).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array.At(1).AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("23"), child_array.At(2).AsString());
    CPPUNIT_ASSERT_EQUAL(true, child_array.At(3).AsBool());

    const JsonObject& child_object = object.Get("k05").AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(true, child_object.Get("k06").AsBool());
    CPPUNIT_ASSERT_EQUAL(123, child_object.Get("k07").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("456"), child_object.Get("k08").AsString());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_object.Get("k09").AsNull());
}

// =============================================================================

void CompositeTypesTest::TestConstructArray()
{
    mj::JsonArray array;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), array.Size());

    const size_t count = 6;
    array.Reserve(count);
    array.PushBack(2.71828);
    array.PushBack(true);
    array.PushBack(nullptr);
    array.PushBack(mj::JsonArray{1, 2, 3.14});
    array.PushBack("hello");
    array.PushBack(mj::JsonObject{
        std::make_pair("k06", true),
        std::make_pair("k07", 123),
        std::make_pair("k08", "456"),
        std::make_pair("k09", nullptr),
    });

    CPPUNIT_ASSERT_EQUAL(count, array.Size());
    CPPUNIT_ASSERT(AlmostEqual(2.71828, array.At(0).AsNumber()));
    CPPUNIT_ASSERT_EQUAL(true, array.At(1).AsBool());
    CPPUNIT_ASSERT_EQUAL(nullptr, array.At(2).AsNull());

    const mj::JsonArray& child_array = array.At(3).AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(1, child_array.At(0).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(2, child_array.At(1).AsNumber().To<int>());
    CPPUNIT_ASSERT(AlmostEqual(3.14, child_array.At(2).AsNumber()));

    CPPUNIT_ASSERT_EQUAL(std::string("hello"), array.At(4).AsString());

    const JsonObject& child_object = array.At(5).AsObject();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(4), child_object.Size());
    CPPUNIT_ASSERT_EQUAL(true, child_object.Get("k06").AsBool());
    CPPUNIT_ASSERT_EQUAL(123, child_object.Get("k07").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("456"), child_object.Get("k08").AsString());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_object.Get("k09").AsNull());

    size_t illegal_index = count * 1000;
    CPPUNIT_ASSERT_THROW(array.At(illegal_index), JsonException);
    CPPUNIT_ASSERT_NO_THROW(array[illegal_index]);
}

// =============================================================================

void CompositeTypesTest::TestConstructObject()
{
    mj::JsonObject object;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), object.Size());

    object.AddField("k00", true);
    object.AddField("k01", nullptr);
    object.AddField("k02", "json");
    object.AddField("k03", 3);
    object.AddField("k04", JsonArray{"first", 2, false, 5.7, nullptr});
    object.AddField("k05", JsonObject{
        std::make_pair("k06", 123),
        std::make_pair("k07", "string value"),
        std::make_pair("k08", nullptr),
    });

    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), object.Size());

    CPPUNIT_ASSERT_EQUAL(true, object.Get("k00").AsBool());
    CPPUNIT_ASSERT_EQUAL(nullptr, object.Get("k01").AsNull());
    CPPUNIT_ASSERT_EQUAL(std::string("json"), object.Get("k02").AsString());
    CPPUNIT_ASSERT_EQUAL(3, object.Get("k03").AsNumber().To<int>());

    const JsonArray& child_array = object.Get("k04").AsArray();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(5), child_array.Size());
    CPPUNIT_ASSERT_EQUAL(std::string("first"), child_array.At(0).AsString());
    CPPUNIT_ASSERT_EQUAL(2, child_array.At(1).AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(false, child_array.At(2).AsBool());
    CPPUNIT_ASSERT(AlmostEqual(5.7, child_array.At(3).AsNumber()));
    CPPUNIT_ASSERT_EQUAL(nullptr, child_array.At(4).AsNull());

    const mj::JsonObject& child_object = object.Get("k05").AsObject();
    CPPUNIT_ASSERT_EQUAL(123, child_object.Get("k06").AsNumber().To<int>());
    CPPUNIT_ASSERT_EQUAL(std::string("string value"), child_object.Get("k07").AsString());
    CPPUNIT_ASSERT_EQUAL(nullptr, child_object.Get("k08").AsNull());

    CPPUNIT_ASSERT_THROW(object.Get("unused_key"), mj::JsonException);
}

// =============================================================================

} // namespace mj::test

// =============================================================================
