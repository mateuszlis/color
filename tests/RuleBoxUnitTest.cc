/*
 * RuleBoxUnitTest.cc
 *
 *  Created on: 25-06-2013
 *      Author: lisu
 */
// std
#include <iostream>
#include <limits>

// stl
#include <vector>

// Google test tools
#include "gtest/gtest.h"

// project-local
#include "utils.hh"
#include "Rules.hh"
#include "RuleBox.hh"

namespace Color { namespace Test {

TEST( RuleBox, Construct )
{
    RuleBox::Ptr lRuleBox( new RuleBox );
}

TEST( RuleBox, AddRule )
{
    RuleBox::Ptr lRuleBox( new RuleBox );
    
    const uint8_t lLinesCount( 3 );
    NumberRule::Ptr lRule1( new NumberRule( BROWN, 0 ) );
    lRule1->addColor( CYAN );
    const std::string lRegex( "[0-9]+" );
    IRule::Ptr lRule2( new Rule( RED, lRegex, 1 ) );

    lRuleBox->addRule( lRule1 );
    lRuleBox->addRule( lRule2 );
    const std::string lBaseLine( "This is the 1231 line 12" );
    for ( int lI( 0 ) ; lI < lLinesCount ; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, lI )
                , "\033[33m This is the \033[31m1231\033[33m line \033[31m12\033[0m" );

    for ( int lI( lLinesCount ) ; lI < lLinesCount * 2 ; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, 3 )
                , "\033[36m This is the \033[31m1231\033[36m line \033[31m12\033[0m" );

    for ( int lI( lLinesCount * 2 ) ; lI < lLinesCount * 3; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, lI )
                , "\033[33m This is the \033[31m1231\033[33m line \033[31m12\033[0m" );

}

}} // namespace Color::ColorTest

