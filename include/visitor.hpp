#pragma once

#include "json.hpp"

// =============================================================================

namespace mj
{

// =============================================================================

class JsonAbstractVisitor
{
public:
    virtual ~JsonAbstractVisitor() {}

    virtual void operator()(const JsonNode&) = 0;
    virtual void operator()(const JsonString&) = 0;
    virtual void operator()(const JsonNumber&) = 0;
    virtual void operator()(const JsonBool&) = 0;
    virtual void operator()(const JsonNull&) = 0;
    virtual void operator()(const JsonObjectPtr&) = 0;
    virtual void operator()(const JsonArrayPtr&) = 0;

    template<typename T>
    void operator()(const T&) { throw std::runtime_error("[Visitor] unhandled JSON type"); }
};

// =============================================================================

} // namespace mj

// =============================================================================
