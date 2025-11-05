#pragma once

#include <ostream>
#include <stdexcept>

#include "json.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

class JsonPrintVisitor
{
public:
    JsonPrintVisitor(std::ostream& stream, const SerializeOptions& opts = SerializeOptions{});

    void operator()(const JsonNode& n);
    void operator()(const JsonString& s);
    void operator()(const JsonNumber& n);
    void operator()(const JsonBool& b);
    void operator()(const JsonNull&);
    void operator()(const JsonObjectPtr& o);
    void operator()(const JsonArrayPtr& a);

    template<typename T>
    void operator()(const T&) { throw std::runtime_error("[JsonPrintVisitor] unhandled json type"); }

private:
    void AddNewLineIfPretty()
    {
        if (options_.pretty)
            stream_ << '\n';
    }

    void IncIndent()
    {
        level_ += 1;
    }
    void DecIndent()
    {
        level_ -= 1;
    }

    template<typename... Args>
    void serializeArgs(Args... args)
    {
        if (options_.pretty)
        {
            for (size_t _ = 0; _ < level_; _++)
                stream_ << options_.indent;
        }
        (stream_ << ... << args);
    }

private:
    std::ostream& stream_;
    const SerializeOptions& options_;

    size_t level_ = 0;
    bool need_indent_  = false;
};

// =============================================================================

} // namespace mj

// =============================================================================
