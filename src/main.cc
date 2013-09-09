#include <iostream>

#include "CLHandler.hh"
#include "RuleBox.hh"

using namespace Color;
int main( int aArgc, char* aArgv[] )
{
    RuleBox::Ptr lAppliedRules;
    try
    {
        CLHandler handler( aArgc, aArgv );
        lAppliedRules = handler.produceRules();
        std::string lLine;

        size_t lI( 0 );
        while ( std::getline( std::cin, lLine ) )
            if ( lLine.size() )
                std::cout << lAppliedRules->process( lLine, lI++ ) << std::endl << std::flush;
        return 0;
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
    return 1;

}
