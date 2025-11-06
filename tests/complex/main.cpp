#include <fstream>

#include "parser.hpp"

// =============================================================================

int main(int argc, char* argv[])
{
    mj::SerializeOptions options;

    // NOTE: use -jq flag to obtain jq-like output
    if (argc > 1 && std::string(argv[1]) == "-jq")
    {
        options.pretty = true;
        options.indent = "  ";
        options.field_sep = ": ";
    }

    std::string line;
    while (std::getline(std::cin, line))
    {
        mj::JsonNode node = mj::ParseFrom(line);
        node.PrintToStream(std::cout, options);
        std::cout << std::endl;
    }
    return 0;
}

// =============================================================================
