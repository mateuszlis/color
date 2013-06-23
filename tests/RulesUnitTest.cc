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
    Rule::Ptr lRule( new Rule( RED, "[0-9]+" ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    std::string lResult( lRule->apply( lMiddleText ) );
    EXPECT_EQ( lResult
            , std::string( 
                "This is text with numbers \033[31m1212\033[0m \033[31m12\033[0m ha" ) );
}

TEST( RuleTest, colorize_everything )
{
    Rule::Ptr lRule( new Rule( RED, ".*" ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    std::string lResult( lRule->apply( lMiddleText ) );
    EXPECT_EQ( lResult
            , std::string( 
                "\033[31mThis is text with numbers 1212 12 ha\033[0m" ) );
}

}} // namespace Color::ColorTest

