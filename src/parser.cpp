#include "parser.hpp"

#include <algorithm>
#include <cmath>

// =============================================================================

namespace
{

auto is_whitespace = [](char c) -> bool {
    return std::isspace(static_cast<unsigned char>(c));
};

// =============================================================================

std::string_view StripWhitespaces(std::string_view sv) {
    auto first_non_whitespace = std::find_if_not(sv.begin(), sv.end(), is_whitespace);

    if (first_non_whitespace == sv.end())
        return {};

    auto last_non_whitespace = std::find_if_not(sv.rbegin(), sv.rend(), is_whitespace);

    size_t new_length = std::distance(first_non_whitespace, last_non_whitespace.base());

    return sv.substr(std::distance(sv.begin(), first_non_whitespace), new_length);
}

// =============================================================================

std::string_view ChopFirstAndStrip(size_t n, std::string_view s)
{
    return StripWhitespaces(s.substr(n, s.size() - n));
}

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ChopNode(std::string_view str, const mj::JsonDeserializeOptions& options)
{
    if (str.empty())
        throw mj::JsonException("ChopNode from empty string");

    switch (str[0])
    {
    case '[': return mj::ParseArray(str, options);
    case '{': return mj::ParseObject(str, options);
    case '"': return mj::ParseString(str, options);
    case 't': case 'f': return mj::ParseBool(str, options);
    case 'n': return mj::ParseNull(str, options);
    default: return mj::ParseNumber(str, options);
    }
}

// =============================================================================

} // namespace

// =============================================================================


// =============================================================================

namespace mj
{

// =============================================================================

JsonNode ParseFrom(std::string_view str, const JsonDeserializeOptions& options)
{
    str = StripWhitespaces(str);

    auto [node, tail] = ChopNode(str, options);
    if (!tail.empty())
        throw mj::JsonException("Bad JSON: `{}...`", str.substr(0, 64));

    return node;
}

// =============================================================================

std::pair<JsonNode, std::string_view> ParseArray(std::string_view str, const JsonDeserializeOptions& options)
{
    if (str.size() > 1)
    {
        std::string_view content = ChopFirstAndStrip(1, str);
        JsonArray array;

        bool has_at_least_one_element = false;
        while (true)
        {
            if (content.front() == ']')
                return {JsonNode{array}, ChopFirstAndStrip(1, content)};

            if (has_at_least_one_element)
            {
                if (content.empty() || content.front() != ',')
                    return {JsonNode{nullptr}, str};
                content = ChopFirstAndStrip(1, content);
            }

            auto [node, tail] = ChopNode(content, options);
            if (tail == content || tail.empty())
                return {JsonNode{nullptr}, str};

            array.PushBack(std::move(node));
            has_at_least_one_element = true;
            content = tail;
        }
    }
    return {JsonNode{nullptr}, str};
}

// =============================================================================

std::pair<JsonNode, std::string_view> ParseObject(std::string_view str, const JsonDeserializeOptions& options)
{
    if (str.size() > 1)
    {
        std::string_view content = ChopFirstAndStrip(1, str);
        JsonObject object;

        bool has_at_least_one_field = false;
        while (true)
        {
            if (content.front() == '}')
                return {JsonNode{object}, ChopFirstAndStrip(1, content)};

            if (has_at_least_one_field)
            {
                if (content.empty() || content.front() != ',')
                    return {JsonNode{nullptr}, str};
                content = ChopFirstAndStrip(1, content);
            }

            auto [node_field, tail_field] = ChopNode(content, options);
            if (tail_field == content || tail_field.empty() || tail_field.front() != ':')
                return {JsonNode{nullptr}, str};
            content = ChopFirstAndStrip(1, tail_field);

            auto [node_value, tail_value] = ChopNode(content, options);
            if (tail_value == content || tail_value.empty())
                return {JsonNode{nullptr}, str};

            object.AddField(node_field.AsString(), std::move(node_value));
            has_at_least_one_field = true;
            content = tail_value;
        }
    }
    return {JsonNode{nullptr}, str};
}

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseBool(std::string_view str, const JsonDeserializeOptions&)
{
    static const std::string true_str("true");
    static const std::string false_str("false");

    if (str.starts_with(true_str))
    {
        std::string_view tail = ChopFirstAndStrip(true_str.size(), str);
        return {mj::JsonNode{true}, tail};
    }

    if (str.starts_with(false_str))
    {
        std::string_view tail = ChopFirstAndStrip(false_str.size(), str);
        return {mj::JsonNode{false}, tail};
    }

    return {mj::JsonNode{nullptr}, str};
}

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseNull(std::string_view str, const JsonDeserializeOptions&)
{
    static const std::string null_str("null");

    std::string_view tail = str;
    if (str.starts_with(null_str))
        tail = ChopFirstAndStrip(null_str.size(), str);

    return {mj::JsonNode{nullptr}, tail};
}

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseString(std::string_view str, const JsonDeserializeOptions&)
{
    bool found = false;
    size_t close_quote_idx;
    for (close_quote_idx = 1; close_quote_idx < str.size() && !found;)
    {

        switch (str[close_quote_idx])
        {
            case '\\':
                close_quote_idx += 2;
                break;
            case '"':
                found = true;
                break;
            default:
                close_quote_idx++;
                break;
        }
    }
    if (!found)
        return {mj::JsonNode{nullptr}, str};

    std::string_view tail = ChopFirstAndStrip(close_quote_idx + 1, str);
    return {mj::JsonString{str.substr(1, close_quote_idx - 1)}, tail};
}

// =============================================================================

std::pair<mj::JsonNode, std::string_view> ParseNumber(std::string_view str, const JsonDeserializeOptions& options)
{
    size_t tail_start_idx;
    try
    {
        double number = std::stod(str.begin(), &tail_start_idx);
        if (options.strict && !std::isfinite(number))
            throw JsonException("non-finite numbers (nan, +-inf) are not allowed in strict mode");
        return {mj::JsonNode(number), ChopFirstAndStrip(tail_start_idx, str)};
    }
    catch(const std::exception& e)
    {
        return {mj::JsonNode{nullptr}, str};
    }
}

// =============================================================================

} // namespace mj

// =============================================================================
