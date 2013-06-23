#include <iostream>
#include <boost/regex.hpp>

#include "colorCommandLine.hxx"
#include "utils.hh"

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
        const boost::regex lMatcher("[0-9]", boost::regex_constants::basic);
        const std::string  lColor = "A&AA";

        std::string lLine;
        while ( std::getline(std::cin, lLine ) )
        {
            std::cout << boost::regex_replace( lLine, lMatcher, Color::colorizeString< Color::RED >  ) << std::endl;
            //{
            //    //std::cerr << *lBegin << " " << *lEnd << std::endl;
            //    Color::color(Color::BOLD_RED, lLine, std::cout );
            //}
            //else
            //{
            //    std::cout << lLine;
            //}
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
