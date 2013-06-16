#include <iostream>

#include "colorCommandLine.hxx"

namespace Color {

auto RED = "\033[31m";
auto BOLD_RED = "\033[1;31m";
auto RESET = "\033[0m";

typedef std::string colorname;

void color( colorname col, std::string& txt, std::ostream& stream )
{
    stream << col << txt << RESET;
}

} // namespace Color
void usage()
{
    std::cerr << "usage: color [options]" << std::endl << "options:" << std::endl;
            options::print_usage( std::cerr );
}

int main( int argc, char* argv[] )
{
    try 
    {   
            int end; // End of options.
            options opt( argc, argv, end);

            if ( opt.help() )
            {   
                usage();
                return 0;
            }   

        auto lRegexp = opt.regexp();
        auto lWholeLines = opt.f();
        std::string lLine;
        while (std::getline(std::cin, lLine))
        {
            Color::color(Color::BOLD_RED, lLine, std::cout );
            std::cout << std::endl;
        }
        return 0;
    }
    catch ( const cli::exception& e )
    {       
        std::cerr << e << std::endl;
        usage();
        return 1;
    }

}
