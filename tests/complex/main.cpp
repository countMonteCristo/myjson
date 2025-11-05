#include <fstream>

#include "parser.hpp"

// =============================================================================

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        mj::JsonNode node = mj::ParseFrom(line);
        node.PrintToStream(std::cout);
        std::cout << std::endl;
    }
    return 0;
}

// =============================================================================
