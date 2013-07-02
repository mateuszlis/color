#include <iostream>
#include <boost/regex.hpp>

#include "colorCommandLine.hxx"
#include "utils.hh"
#include "RuleBox.hh"
#include "Rules.hh"

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

        // prototype rules for compilation
        Color::RuleBox::Ptr lRules( new Color::RuleBox );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_RED, "error", true ) ) );
        Color::NumberRule::Ptr lNumberRule( new Color::NumberRule( Color::LIGHT_GRAY, 2 ) );
        lNumberRule->addColor( Color::RESET );
        lRules->addRule( lNumberRule );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BROWN, "[a-zA-Z_]+\\.hh" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BROWN, "[a-zA-Z_]+\\.cc" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_RED, "undefined" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_BLUE, "warning" ) ) );

        std::string lLine;
        size_t lI( 0 );
        while ( std::getline(std::cin, lLine ) )
        {
            std::cout << lRules->process( lLine, lI++ ) << std::endl;
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
