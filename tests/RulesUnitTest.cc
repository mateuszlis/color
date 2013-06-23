/*
 * RulesUnitTest.cc
 *
 *  Created on: 23-06-2013
 *      Author: lisu
 */
// std
#include <iostream>

// stl
#include <vector>

// Google test tools
#include "gtest/gtest.h"

// project-local
#include "utils.hh"
#include "Rules.hh"

namespace Color { namespace Test {

TEST( RuleTest, Construct )
{
    Rule::Ptr lRule( new Rule( RED, "[0-9]+" ) );
}

TEST( RuleTest, colorize_only_numbers_in_the_middle_of_text )
{
    const std::string lRegex( "[0-9]+" );

    Rule::Ptr lRule( new Rule( RED, lRegex ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    std::string lResult( lRule->apply( lMiddleText ) );
    EXPECT_EQ( lResult
            , std::string( 
                "This is text with numbers \033[31m1212\033[0m \033[31m12\033[0m ha" ) );
}

TEST( RuleTest, colorize_everything )
{
    const std::string lRegex( ".*" );

    Rule::Ptr lRule( new Rule( RED, lRegex ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    std::string lResult( lRule->apply( lMiddleText ) );
    EXPECT_EQ( lResult
            , std::string( 
                "\033[31mThis is text with numbers 1212 12 ha\033[0m" ) );
}

TEST( RuleTest, colorize_whole_lines_with_numbers )
{
    const bool lColorizeWholeLines( true );
    const std::string lRegex( "[0-9]+" );

    Rule::Ptr lRule( new Rule( RED, lRegex, lColorizeWholeLines ) );
    std::string lFirstLine( "§ first line " );
    std::string lSecondLine( "2 line " );
    std::string lThirdLine( "third line" );
    std::string lFourthLine( "4 line" );

    std::string lResult( lRule->apply( lFirstLine ) );
    EXPECT_EQ( lResult, lFirstLine );

    lResult = lRule->apply( lSecondLine );
    EXPECT_EQ( lResult
            , std::string( "\033[31m" + lSecondLine + "\033[0m" ) );

    lResult = lRule->apply( lThirdLine );
    EXPECT_EQ( lResult, lThirdLine );

    lResult = lRule->apply( lFourthLine );
    EXPECT_EQ( lResult
            , std::string( "\033[31m" + lFourthLine + "\033[0m" ) );
}

}} // namespace Color::ColorTest

