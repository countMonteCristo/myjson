#include "serialize_visitor.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <variant>

// =============================================================================

namespace mj
{

// =============================================================================

JsonSerializeVisitor::JsonSerializeVisitor(std::ostream &stream, const JsonSerializeOptions& opts) :
    stream_(stream),
    options_(opts)
{}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonNode &n)
{
    std::visit(*this, n.Value());
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonString& s)
{
    stream_ << "\"" << s << "\"";
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonNumber& n)
{
    if (options_.strict && !std::isfinite(n))
        throw JsonException("non-finite numbers (nan, +-inf) are not allowed in strict mode");

    if (std::isnan(n))
    {
        stream_ << "NaN";
        return;
    }
    if (std::isinf(n))
    {
        if (n < 0.0) stream_ << "-";
        stream_ << "Infinity";
        return;
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(12) << n;
    std::string result = ss.str();

    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.')
        result.pop_back();

    stream_ << result;
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonBool& b)
{
    stream_ << std::boolalpha << b;
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonNull&)
{
    stream_ << "null";
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonObjectPtr& o)
{
    stream_ << "{";
    AddNewLineIfPretty();
    IncIndent();
    bool is_first = true;
    if (options_.sort_keys)
    {
        std::vector<JsonObject::CKeyPtr> keys;
        keys.reserve(o->Size());
        for (auto it = o->begin(); it != o->end(); ++it)
            keys.push_back(&it->first);

        std::sort(keys.begin(), keys.end(), [](JsonObject::CKeyPtr a, JsonObject::CKeyPtr b) -> bool {
            return *a < *b;
        });

        for (const std::string* key: keys)
        {
            const auto& node = o->Get(*key);
            SerializeObjectField(*key, node, is_first);
        }
    }
    else
    {
        for (const auto& [key, node]: *o)
            SerializeObjectField(key, node, is_first);
    }

    DecIndent();
    AddNewLineIfPretty();
    SerializeArgs("}");
}

// =============================================================================

void JsonSerializeVisitor::operator()(const JsonArrayPtr& a)
{
    stream_ << "[";
    AddNewLineIfPretty();
    IncIndent();
    bool is_first = true;
    for (const auto& node: *a)
    {
        if (!is_first)
        {
            stream_ << options_.element_sep;
            AddNewLineIfPretty();
        }
        SerializeArgs("");
        std::visit(*this, node.Value());
        is_first = false;
    }
    DecIndent();
    AddNewLineIfPretty();
    SerializeArgs("]");
}

// =============================================================================

void JsonSerializeVisitor::SerializeObjectField(const std::string& key, const JsonNode& node, bool& is_first)
{
    if (!is_first)
    {
        stream_ << options_.element_sep;
        AddNewLineIfPretty();
    }
    SerializeArgs("\"", key, "\"", options_.field_sep);
    std::visit(*this, node.Value());
    is_first = false;
}

// =============================================================================

} // namespace mj

// =============================================================================
