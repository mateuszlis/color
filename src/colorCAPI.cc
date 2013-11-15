#include "colorCAPI.h"

#include "CLHandler.hh"
#include "RuleBox.hh"
#include <stdio.h>

ColorErrorCode g_Ret;
Color::RuleBox::Ptr g_AppliedRules;

void color_init( int argc, char** argv )
{
    char* lArgs[] = { const_cast< char* >( "color" )
                   , const_cast< char* >( "syslog" ) }; // default
    int lArgc = 2;
    char** lArgv = lArgs;

    try
    {
        if ( argc ) // arguments were supported by user
        {
            g_Ret = OK;
            Color::CLHandler lHandler(argc, argv);
            g_AppliedRules = lHandler.produceRules();
        }
        else // use default
        {
            g_Ret = INIT_NOT_CALLED_SETTINGS_DEFAULT;
            Color::CLHandler lHandler(lArgc, lArgv);
            g_AppliedRules = lHandler.produceRules();
        }
    }
    catch ( const std::exception& e )
    {
        puts(e.what());
        g_Ret = FAILED_TO_PARSE_ARGUMENTS;
    }
}

ColorErrorCode color_colorize( char* aCString, char* aDest, size_t aDestSize )
{
    static size_t lLineNum( 0 );
    try
    {
        std::string lLine( aCString );
        std::string lRes( g_AppliedRules->process( lLine
                                            , lLineNum++ ) );
        if ( lRes.size() > aDestSize )
        {
            return RESULT_LINE_TOO_LONG;
        }

        strcpy( aDest, lRes.c_str() );
    }
    catch ( const std::exception& e )
    {
        puts(e.what());
        return FAILED_TO_PARSE_ARGUMENTS;
    }
    return g_Ret;
}
