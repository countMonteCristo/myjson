#include "json.hpp"

#include "exceptions.hpp"
#include "serialize_visitor.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

void JsonNode::SerializeToStream(std::ostream& stream, JsonSerializeOptions options) const
{
    std::visit(JsonSerializeVisitor{stream, options}, value_);
}

// =============================================================================

void JsonArray::Reserve(size_t capacity)
{
    nodes_.reserve(capacity);
}

// =============================================================================

void JsonArray::Resize(size_t new_size)
{
    nodes_.resize(new_size);
}

// =============================================================================

size_t JsonArray::Size() const
{
    return nodes_.size();
}

// =============================================================================

JsonNode& JsonArray::At(size_t index)
{
    CheckBounds(index);
    return nodes_.at(index);
}

// =============================================================================

const JsonNode& JsonArray::At(size_t index) const
{
    CheckBounds(index);
    return nodes_.at(index);
}

// =============================================================================

void JsonArray::CheckBounds(size_t index) const
{
    if (index >= Size())
        throw JsonException("Out of bounds: index {} exceeds array size {}", index, Size());
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
