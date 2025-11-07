#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>
#include <vector>

#include "exceptions.hpp"
#include "serialize_options.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

class NumberDouble
{
public:
    template<typename T>
    NumberDouble(T x) :
        n_(x)
    {}

    operator double() const { return n_; }

    template<typename T>
    T To() const { return static_cast<T>(n_); }

private:
    double n_;
};

using Number = NumberDouble;

// =============================================================================

using JsonNumber = Number;
using JsonString = std::string;
using JsonBool = bool;
using JsonNull = std::nullptr_t;

class JsonObject;
class JsonArray;

using JsonObjectPtr = std::unique_ptr<JsonObject>;
using JsonArrayPtr = std::unique_ptr<JsonArray>;

using JsonValue = std::variant<
    JsonString,
    JsonNumber,
    JsonBool,
    JsonObjectPtr,
    JsonArrayPtr,
    JsonNull
>;

// =============================================================================

template<typename T>
concept JsonMovable = std::is_same_v<typename std::remove_reference<T>::type, JsonObject> ||
                      std::is_same_v<typename std::remove_reference<T>::type, JsonArray>;

// =============================================================================

class JsonNode
{
public:
    template<typename T>
    JsonNode(T&& t) :
        value_(std::forward<T>(t))
    {}

    template<JsonMovable T>
    JsonNode(T&& x) :
        value_(std::make_unique<std::remove_cvref_t<T>>(std::move(std::forward<T>(x))))
    {}

    JsonNode(const char* t) : value_(JsonString(t)) {}
    JsonNode() : value_(JsonNull()) {}

    JsonNode(JsonNode&& node) = default;
    JsonNode(const JsonNode&) = delete;

    void SerializeToStream(std::ostream& stream = std::cout, JsonSerializeOptions options = JsonSerializeOptions{}) const;

    bool IsString() const { return value_.index() == 0; }
    bool IsNumber() const { return value_.index() == 1; }
    bool IsBool() const { return value_.index() == 2; }
    bool IsObject() const { return value_.index() == 3; }
    bool IsArray() const { return value_.index() == 4; }
    bool IsNull() const { return value_.index() == 5; }

    JsonString& AsString() { return std::get<JsonString>(value_); }
    JsonNumber& AsNumber() { return std::get<JsonNumber>(value_); }
    JsonBool& AsBool() { return std::get<JsonBool>(value_); }
    JsonObject& AsObject() { return *std::get<JsonObjectPtr>(value_); }
    JsonArray& AsArray() { return *std::get<JsonArrayPtr>(value_); }
    JsonNull& AsNull() { return std::get<JsonNull>(value_); }

    const JsonString& AsString() const { return std::get<JsonString>(value_); }
    const JsonNumber& AsNumber() const { return std::get<JsonNumber>(value_); }
    const JsonBool& AsBool() const { return std::get<JsonBool>(value_); }
    const JsonObject& AsObject() const { return *std::get<JsonObjectPtr>(value_); }
    const JsonArray& AsArray() const { return *std::get<JsonArrayPtr>(value_); }
    const JsonNull& AsNull() const { return std::get<JsonNull>(value_); }

    JsonValue& Value() { return value_; }
    const JsonValue& Value() const { return value_; }

private:
    JsonValue value_;
};

// =============================================================================

class JsonArray
{
public:
    using Array = std::vector<JsonNode>;

    JsonArray() = default;
    JsonArray(JsonArray&&) = default;

    JsonArray(const JsonArray&) = delete;

    template<typename... Args>
    JsonArray(Args&&... args)
    {
        nodes_.reserve(sizeof...(Args));
        ((nodes_.emplace_back(std::forward<Args>(args))), ...);
    }

    template<typename T>
    void PushBack(T&& value)
    {
        nodes_.push_back(std::move(std::forward<T>(value)));
    }

    void Reserve(size_t capacity);
    void Resize(size_t new_size);
    size_t Size() const;

    JsonNode& At(size_t index);
    const JsonNode& At(size_t index) const;

    JsonNode& operator[](size_t index) { return nodes_[index]; }
    const JsonNode& operator[](size_t index) const { return nodes_[index]; }

    Array::iterator begin() { return nodes_.begin(); }
    Array::iterator end() { return nodes_.end(); }

    Array::const_iterator begin() const { return nodes_.begin(); }
    Array::const_iterator end() const { return nodes_.end(); }

private:
    void CheckBounds(size_t index) const;

private:
    Array nodes_;
};

// =============================================================================

template<typename T>
concept JsonObjetcItem = std::is_same_v<T, std::pair<const std::string, JsonNode>>;

// =============================================================================

class JsonObject
{
public:
    using Key = std::string;
    using Map = std::unordered_map<Key, JsonNode>;
    // using Map = std::map<Key, JsonNode>;

    using CKeyPtr = const Key*;

    JsonObject() = default;
    JsonObject(JsonObject&&) = default;

    JsonObject(const JsonObject& node) = delete;

    template<typename... Args>
    JsonObject(Args&&... args)
    {
        map_.reserve(sizeof...(Args));
        ((map_.emplace(std::forward<Args>(args))), ...);
    }

    bool Has(const std::string& field);
    JsonNode& Get(const std::string& field);
    const JsonNode& Get(const std::string& field) const;

    JsonNode& operator[](const std::string& field) { return Get(field); }
    const JsonNode& operator[](const std::string& field) const { return Get(field); }

    template<typename T>
    void AddField(const std::string& name, T value)
    {
        map_.insert(std::make_pair(name, std::forward<T>(value)));
    }

    size_t Size() const;

    Map::iterator begin() { return map_.begin(); }
    Map::iterator end() { return map_.end(); }

    Map::const_iterator begin() const { return map_.begin(); }
    Map::const_iterator end() const { return map_.end(); }

private:
    Map map_;
};

// =============================================================================

} // namespace mj

// =============================================================================
