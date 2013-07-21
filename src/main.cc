#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <sys/select.h>

#include "Config.hh"
#include "utils.hh"
#include "RuleBox.hh"
#include "Rules.hh"

void findConfig( std::ifstream& aStr );

using namespace Color;
int main( int argc, char* argv[] )
{
    std::ifstream lStr( "color.conf" );
    findConfig( lStr );

    Config::Ptr lConf( new Config( lStr ) );
    RuleBox::Ptr lAppliedRules;
    if ( argc > 1 )
    {
        try 
        {
            lAppliedRules = lConf->getRuleBox( argv[ 1 ] );
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
        return 0;
          //timeout
    else if ( ret == -1 )
        return 0;
          //  //error
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
