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

 // I very much prefer this to cryptic  FRIEND_TEST declarations
#define protected public
// tested lib
#include "RuleBox.hh"
#undef protected

namespace Color { namespace Test {

TEST( RuleBox, Construct )
{
    RuleBox::Ptr lRuleBox( new RuleBox );
}

TEST( RuleBox, MixOfRuleAndNumberRule )
{
    RuleBox::Ptr lRuleBox( new RuleBox );

    const uint8_t lLinesCount( 3 );
    NumberRule::Ptr lRule1( new NumberRule( BROWN, lLinesCount ) );
    lRule1->addColor( CYAN );
    const std::string lRegex( "[0-9]+" );
    IRule::Ptr lRule2( new Rule( RED, lRegex ) );

    lRuleBox->addRule( lRule1 );
    lRuleBox->addRule( lRule2 );
    const std::string lBaseLine( "This is the 1231 line 12" );
    for ( int lI( 0 ) ; lI < lLinesCount ; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, lI )
                , "\033[0;33mThis is the \033[0;31m1231\033[0;33m line \033[0;31m12\033[0;0m" )
            << "Line " << lI << " Not equal";

    for ( int lI( lLinesCount ) ; lI < lLinesCount * 2 ; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, 3 )
                , "\033[0;36mThis is the \033[0;31m1231\033[0;36m line \033[0;31m12\033[0;0m" );

    for ( int lI( lLinesCount * 2 ) ; lI < lLinesCount * 3; ++lI )
        EXPECT_EQ( lRuleBox->process( lBaseLine, lI )
                , "\033[0;33mThis is the \033[0;31m1231\033[0;33m line \033[0;31m12\033[0;0m" );

}

TEST( RuleBox, OverlappingRules )
{
    RuleBox::Ptr lRuleBox( new RuleBox );

    const std::string lRegex1( "abcdef" )
                    , lRegex2( "defghijk" );

    IRule::Ptr lRule1( new Rule( BROWN, lRegex1 ) );
    IRule::Ptr lRule2( new Rule( CYAN, lRegex2 ) );

    lRuleBox->addRule( lRule1 );
    lRuleBox->addRule( lRule2 );
    const std::string lBaseLine( " abcdefghijklmnop " );

    std::string lExp( " \033[0;33mabc\033[0;36mdefghijk\033[0;0mlmnop " );
    std::string lResult( lRuleBox->process( lBaseLine ) );
    EXPECT_EQ( lResult, lExp )
        << " Not equal exp:[" << lExp << "] res:[" << lResult << "]" ;
}

TEST( RuleBox, CombiningBoxes )
{
    // create some boxes and rules
    RuleBox::Ptr lRuleBox1( new RuleBox );
    RuleBox::Ptr lRuleBox2( new RuleBox );
    IRule::Ptr lRule1( new Rule( BROWN, "" ) );
    IRule::Ptr lRule2( new Rule( RED, "" ) );
    IRule::Ptr lRule3( new Rule( GREEN, "" ) );
    lRuleBox1->addRule( lRule1 );
    lRuleBox1->addRule( lRule2 );
    lRuleBox2->addRule( lRule3 );

    // invoke opeator+ to create new RuleBox
    RuleBox lNewOne( *lRuleBox1 + *lRuleBox2 );
    // and modify the original stuff
    lRuleBox2->addBox( *lRuleBox1 );

    // now verify
    ASSERT_EQ( lRuleBox2->m_Rules.size(), 3u );

    // assure proper order
    EXPECT_EQ( lRuleBox2->m_Rules.front().get(), lRule1.get() );
    EXPECT_NO_THROW( lRuleBox2->m_Rules.pop_front() );
    EXPECT_EQ( lRuleBox2->m_Rules.front().get(), lRule2.get() );
    EXPECT_NO_THROW( lRuleBox2->m_Rules.pop_front() );
    EXPECT_EQ( lRuleBox2->m_Rules.front().get(), lRule3.get() );

    ASSERT_EQ( lRuleBox1->m_Rules.size(), 2u );

    ASSERT_EQ( lNewOne.m_Rules.size(), 3u );
    // assure proper order
    EXPECT_EQ( lNewOne.m_Rules.front().get(), lRule1.get() );
    EXPECT_NO_THROW( lNewOne.m_Rules.pop_front() );
    EXPECT_EQ( lNewOne.m_Rules.front().get(), lRule2.get() );
    EXPECT_NO_THROW( lNewOne.m_Rules.pop_front() );
    EXPECT_EQ( lNewOne.m_Rules.front().get(), lRule3.get() );


}

}} // namespace Color::ColorTest

