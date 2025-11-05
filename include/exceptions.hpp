#pragma once

#include <exception>
#include <format>
#include <string_view>

// =============================================================================

namespace mj
{

// =============================================================================

class JsonException : public std::exception
{
public:
    template<typename... Args>
    JsonException(std::string_view format, Args... args)
    {
        message_ = "[JSON] " + std::vformat(format, std::make_format_args(args...));
    }

    // Override the virtual what() method to return the custom message
    const char* what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};

// =============================================================================

} // namespace mj
