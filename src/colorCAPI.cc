#include "colorCAPI.h"

#include "CLHandler.hh"
#include "RuleBox.hh"
#include <stdio.h>

char** g_Argv;
int g_Argc = 0;

void color_init( int argc, char** argv )
{
    g_Argv = argv;
    g_Argc = argc;
}

ColorErrorCode color_colorize( char* aCString, char* aDest, size_t aDestSize )
{
    char* lArgs[] = { const_cast< char* >( "color" )
                   , const_cast< char* >( "syslog" ) }; // default
    // options
    ColorErrorCode lRet( OK );
    static size_t lLineNum( 0 );
    char** lArgv;
    if ( g_Argc )
    {
        lArgv = g_Argv;
    }
    else
    {
        lRet = INIT_NOT_CALLED_SETTINGS_DEFAULT;

        lArgv = lArgs;
    }
    try {
        Color::RuleBox::Ptr lAppliedRules;
        Color::CLHandler handler( 2, lArgv );
        lAppliedRules = handler.produceRules();
        std::string lLine( aCString );
        std::string lRes( lAppliedRules->process( lLine
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
    return lRet;
}
