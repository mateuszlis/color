#include <iostream>
#include <boost/regex.hpp>
#include <sys/select.h>

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
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_RED, "error", false ) ) );
        Color::NumberRule::Ptr lNumberRule( new Color::NumberRule( Color::LIGHT_GRAY, 2 ) );
        lNumberRule->addColor( Color::RESET );
        lRules->addRule( lNumberRule );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BROWN, "[a-zA-Z_]+\\.hh" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BROWN, "[a-zA-Z_]+\\.cc" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_RED, "undefined" ) ) );
        lRules->addRule( Color::IRule::Ptr( new Color::Rule( Color::BOLD_BLUE, "warning" ) ) );
        std::string lLine;

        // handle stdin with timeout
        fd_set selectset;
        struct timeval timeout = {1,0}; //timeout of 10 secs.
        int ret;
        FD_ZERO(&selectset);
        FD_SET(0,&selectset);
        ret =  select(1,&selectset,NULL,NULL,&timeout);
        if(ret == 0)
            return 0;
              //timeout
        else if(ret == -1)
            return 0;
              //  //error
        else 
        {
            size_t lI( 0 );
            while ( std::getline(std::cin, lLine ) )
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
