#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>

#include "Config.hh"
#include "CLHandler.hh"
#include "utils.hh"
#include "RuleBox.hh"
#include "Rules.hh"

namespace po = boost::program_options;

using namespace Color;
int main( int aArgc, char* aArgv[] )
{
    RuleBox::Ptr lAppliedRules;
    try
    {
        CLHandler handler( aArgc, aArgv );
        lAppliedRules = handler.produceRules();
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Error occurred " << e.what() << std::endl;
        displayHelp();
    }
    catch ( ... )
    {
        displayHelp();
    }

    std::string lLine;

    size_t lI( 0 );
    while ( std::getline( std::cin, lLine ) )
        if ( lLine.size() )
            std::cout << lAppliedRules->process( lLine, lI++ ) << std::endl << std::flush;
    return 0;
}
