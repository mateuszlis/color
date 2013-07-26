#include "utils.hh"

namespace Color {

const char* COLOR_NAMES[]
{
        "\033[0;30m"
        , "\033[1;30m"
        , "\033[0;31m"
        , "\033[1;31m"
        , "\033[0;32m"
        , "\033[1;32m"
        , "\033[0;33m"
        , "\033[1;33m"
        , "\033[0;34m"
        , "\033[1;34m"
        , "\033[0;35m"
        , "\033[1;35m"
        , "\033[0;36m"
        , "\033[1;36m"
        , "\033[0;37m"
        , "\033[1;37m"
        , "\033[0;0m"
};

void color( ColorName aCol, const std::string& aTxt, std::ostream& aStream, ColorName resetCol )
{
    aStream << COLOR_NAMES[aCol] << aTxt << COLOR_NAMES[resetCol];
}

void displayHelp()
{
    std::cerr << "Usage: [some-program] | color [color-scheme] " << std::endl
        << "Examples: " << std::endl
        << "$ cat /var/log/syslog | color syslog" << std::endl
        << "$ make 2>&1 | color gcc" << std::endl << std::endl
        << "Enter man color for more information" << std::endl;
}


} // namespace Color
