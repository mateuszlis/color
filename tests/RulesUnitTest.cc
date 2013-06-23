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

TEST( RuleTest, apply_middle )
{
    Rule::Ptr lRule( new Rule( RED, "[0-9]+" ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    std::string lResult( lRule->apply( lMiddleText ) );
    EXPECT_EQ( lResult
            , std::string( 
                "This is text with numbers \033[31m1212\033[0m \033[31m12\033[0m ha" ) );
}

}} // namespace Color::ColorTest

