#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>

#include "Config.hh"
#include "utils.hh"
#include "RuleBox.hh"
#include "Rules.hh"

namespace po = boost::program_options;

using namespace Color;
int main( int aArgc, char* aArgv[] )
{
    // Declare the supported options.
    po::options_description lDesc( "Allowed options" );
    lDesc.add_options()
        ( "help", "produce help message" )
            ;

            po::variables_map lVm;
            po::store( po::parse_command_line( aArgc, aArgv, lDesc ), lVm );
            po::notify( lVm );

            if ( lVm.count( "help" ) ) 
            {
                displayHelp();
                return 1;
            }

    // read configuration
    std::ifstream lStr( "color.conf" );
    findConfig( lStr );

    Config::Ptr lConf( new Config( lStr ) );
    RuleBox::Ptr lAppliedRules;
    if ( aArgc > 1 )
    {
        try 
        {
            lAppliedRules = lConf->getRuleBox( aArgv[ 1 ] );
        }
        catch( std::exception& e)
        {

        }
    }
    if ( !lAppliedRules )
    {
        lAppliedRules = lConf->getAllRules().begin()->second;
    }

    std::string lLine;

    size_t lI( 0 );
    while ( std::getline( std::cin, lLine ) )
        if ( lLine.size() )
            std::cout << lAppliedRules->process( lLine, lI++ ) << std::endl << std::flush;
    return 0;
}
