#pragma once

#include "visitor.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

class JsonSerializeVisitor : public JsonAbstractVisitor
{
public:
    JsonSerializeVisitor(std::ostream& stream, const JsonSerializeOptions& opts = JsonSerializeOptions{});

    void operator()(const JsonNode& n) final;
    void operator()(const JsonString& s) final;
    void operator()(const JsonNumber& n) final;
    void operator()(const JsonBool& b) final;
    void operator()(const JsonNull&) final;
    void operator()(const JsonObjectPtr& o) final;
    void operator()(const JsonArrayPtr& a) final;

private:
    void AddNewLineIfPretty()
    {
        if (options_.pretty)
            stream_ << '\n';
    }

    inline void IncIndent() { ++level_; }
    inline void DecIndent() { --level_; }

    template<typename... Args>
    void SerializeArgs(Args... args)
    {
        if (options_.pretty)
        {
            for (size_t _ = 0; _ < level_; _++)
                stream_ << options_.indent;
        }
        (stream_ << ... << args);
    }

    void SerializeObjectField(const std::string& key, const JsonNode& node, bool &is_first);

private:
    std::ostream& stream_;
    const JsonSerializeOptions& options_;

    size_t level_ = 0;
};

// =============================================================================

}

// =============================================================================
