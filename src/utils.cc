#include "utils.hh"

namespace Color {

const char* COLOR_NAMES[]
{
        "\033[31m"
        , "\033[1;31m"
        , "\033[33m"
        , "\033[1;33m"
        , "\033[36m"
        , "\033[1;36m"
        , "\033[0m"
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream )
{
    aStream << COLOR_NAMES[aCol] << aTxt << COLOR_NAMES[RESET];
}


} // namespace Color
