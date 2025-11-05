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
    bool first = true;
    for (const auto& [key, node]: *o)
    {
        if (!first)
        {
            stream_ << options_.element_sep;
            AddNewLineIfPretty();
        }
        serializeArgs("\"", key, "\"", options_.field_sep);
        std::visit(*this, node.Value());
        first = false;
    }
    DecIndent();
    AddNewLineIfPretty();
    serializeArgs("}");
}

// =============================================================================

void JsonPrintVisitor::operator()(const JsonArrayPtr& a)
{
    stream_ << "[";
    AddNewLineIfPretty();
    IncIndent();
    bool first = true;
    for (const auto& node: *a)
    {
        if (!first)
        {
            stream_ << options_.element_sep;
            AddNewLineIfPretty();
        }
        serializeArgs("");
        std::visit(*this, node.Value());
        first = false;
    }
    DecIndent();
    AddNewLineIfPretty();
    serializeArgs("]");
}

// =============================================================================

} // namespace mj

// =============================================================================
