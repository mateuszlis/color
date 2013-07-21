#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <sys/select.h>

#include "Config.hh"
#include "utils.hh"
#include "RuleBox.hh"
#include "Rules.hh"

void findConfig( std::ifstream& aStr );
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

    // handle stdin with timeout
    fd_set selectset;
    struct timeval timeout = { 10, 0 }; //timeout of 10 secs.
    int ret;
    FD_ZERO( &selectset );

    FD_SET( 0, &selectset );
    ret =  select( 1, &selectset, NULL, NULL, &timeout );
    if(ret == 0)
    {
        displayHelp();
        return 0;
          //timeout
    }
    else if ( ret == -1 )
    {
        displayHelp();
        return 2;
          //  //error
    }
    else 
    {
        size_t lI( 0 );
        while ( std::getline( std::cin, lLine ) )
            if ( lLine.size() )
                std::cout << lAppliedRules->process( lLine, lI++ ) << std::endl;
    }
    return 0;
}


void findConfig( std::ifstream& aStr )
{
    if ( aStr.is_open() )
        return;
    aStr.open( "/etc/color.conf" );
}
