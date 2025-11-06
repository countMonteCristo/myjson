#include "visitor.hpp"

#include <algorithm>
#include <iomanip>
#include <variant>

// =============================================================================

namespace mj
{

// =============================================================================

JsonPrintVisitor::JsonPrintVisitor(std::ostream &stream, const SerializeOptions& opts) :
    stream_(stream),
    options_(opts)
{}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonNode &n)
{
    std::visit(*this, n.Value());
}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonString& s)
{
    stream_ << "\"" << s << "\"";
}

// =============================================================================

// TODO: check for nan, +-inf
// Possible variants:
// * add 'base' and 'strict' modes -the former will silently convert nans and infs
//   into null while the latter wil throw an exception
// * use allow_nan flag like in Python json.dumps() function
// * ...
void JsonPrintVisitor::operator()(const JsonNumber& n)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(12) << n;
    std::string result = ss.str();

    result.erase(result.find_last_not_of('0') + 1, std::string::npos);
    if (result.back() == '.')
        result.pop_back();

    stream_ << result;
}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonBool& b)
{
    stream_ << std::boolalpha << b;
}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonNull&)
{
    stream_ << "null";
}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonObjectPtr& o)
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

void JsonPrintVisitor::operator()(const JsonArrayPtr& a)
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

void JsonPrintVisitor::SerializeObjectField(const std::string& key, const JsonNode& node, bool& is_first)
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
