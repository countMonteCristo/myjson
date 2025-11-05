#include "json.hpp"

#include <cstddef>

#include "exceptions.hpp"
#include "visitor.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

void JsonNode::PrintToStream(std::ostream& stream, SerializeOptions options) const
{
    std::visit(JsonPrintVisitor{stream, options}, value_);
}

// =============================================================================

size_t JsonArray::Size() const
{
    return nodes_.size();
}

// =============================================================================

bool JsonObject::Has(const std::string &field)
{
    return map_.contains(field);
}

// =============================================================================

JsonNode& JsonObject::Get(const std::string &field)
{
    if (!Has(field))
        throw mj::JsonException("Unknown object field: `{}`", field);
    return map_.find(field)->second;
}

// =============================================================================

const JsonNode& JsonObject::Get(const std::string &field) const
{
    return map_.find(field)->second;
}

// =============================================================================

size_t JsonObject::Size() const
{
    return map_.size();
}

// =============================================================================

} // namespace mj

// =============================================================================
