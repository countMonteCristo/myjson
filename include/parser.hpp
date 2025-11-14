#pragma once

#include <string_view>

#include "json.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

JsonNode ParseFrom(std::string_view str, const JsonDeserializeOptions& options = {});

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseArray(std::string_view str, const JsonDeserializeOptions& options);

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseObject(std::string_view str, const JsonDeserializeOptions& options);

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseBool(std::string_view str, const JsonDeserializeOptions&);

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseNull(std::string_view str, const JsonDeserializeOptions&);

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseString(std::string_view str, const JsonDeserializeOptions&);

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseNumber(std::string_view str, const JsonDeserializeOptions& options);

// =============================================================================

} // namespace mj

// =============================================================================
