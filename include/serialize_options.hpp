#pragma once

#include <string>

// =============================================================================

namespace mj
{

// =============================================================================

struct SerializeOptions
{
    bool pretty = false;
    std::string field_sep = ":";
    std::string element_sep = ",";
    std::string indent = "    ";
};

// =============================================================================

} // namespace mj
