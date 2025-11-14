#pragma once

#include <string>

// =============================================================================

namespace mj
{

// =============================================================================

struct JsonSerializeOptions
{
    bool pretty = false;
    std::string field_sep = ":";
    std::string element_sep = ",";
    std::string indent = "    ";
    bool sort_keys = false;
};

// =============================================================================

struct JsonDeserializeOptions
{
    bool strict = false;
};

// =============================================================================

} // namespace mj

// =============================================================================
