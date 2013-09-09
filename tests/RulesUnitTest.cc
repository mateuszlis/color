/*
 * RulesUnitTest.cc
 *
 *  Created on: 23-06-2013
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
#include "mocks.hh"

namespace Color { namespace Test {

using ::testing::_;
using ::testing::InSequence;


TEST( RuleTest, Construct )
{
    Rule::Ptr lRule( new Rule( RED, "[0-9]+", 0 ) );
}

TEST( RuleTest, colorize_only_numbers_in_the_middle_of_text )
{
    const std::string lRegex( "[0-9]+" );

    Rule::Ptr lRule( new Rule( RED, lRegex ) );
    MockIntermediateResult lIResult;
    EXPECT_CALL( lIResult, putMarker( 26, RED ) );
    EXPECT_CALL( lIResult, putMarker( 30, RED ) );
    EXPECT_CALL( lIResult, putMarker( 31, RED ) );
    EXPECT_CALL( lIResult, putMarker( 33, RED ) );

    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    lRule->apply( lMiddleText, lIResult );
}

TEST( RuleTest, colorize_everything )
{
    const std::string lRegex( ".*" );

    Rule::Ptr lRule( new Rule( RED, lRegex  ) );
    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    MockIntermediateResult lIResult;
    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 36, RED ) );
    lRule->apply( lMiddleText, lIResult );
}

TEST( RuleTest, colorize_whole_lines_with_numbers )
{
    const bool lColorizeWholeLines( true );
    const std::string lRegex( "[0-9]+" );
    InSequence dummy;
    Rule::Ptr lRule( new Rule( RED, lRegex, lColorizeWholeLines ) );
    MockIntermediateResult lIResult;

    std::string lFirstLine( "§ first line " );
    std::string lSecondLine( "2 line " );
    std::string lThirdLine( "third line" );
    std::string lFourthLine( "4 line" );
    EXPECT_CALL( lIResult, putMarker( _, _ ) ).Times( 0 );
    lRule->apply( lFirstLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 7, RED ) );
    lRule->apply( lSecondLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( _, _ ) ).Times( 0 );
    lRule->apply( lThirdLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 6, RED ) );
    lRule->apply( lFourthLine, lIResult );
}

TEST( NumberRuleTest, one_color )
{
    InSequence dummy;
    const uint8_t lLinesCount( 1 );
    NumberRule::Ptr lRule( new NumberRule( RED, lLinesCount ) );
    
    std::vector< std::string > lLines;
    std::string lBaseLine( "This is a Line" );
    
    typedef uint8_t testedType;
    for ( testedType lI( 0 ) 
            ; lI < std::numeric_limits< testedType >::max() / 20
            ; ++lI )
    {
        std::stringstream lStream;
        lStream << lBaseLine << " " << lI;
        lLines.push_back( lStream.str() );
    }

    for_each( lLines.begin(), lLines.end(), [=]( std::string aLine ) 
           { uint32_t lI( 0 )
            ; MockIntermediateResult lIResult;
            ; EXPECT_CALL( lIResult, putMarker( 0, RED ) )
            ; EXPECT_CALL( lIResult, putMarker( aLine.size(), RED ) )
            ; lRule->apply( aLine, lIResult, ++lI ); } );

}

TEST( NumberRuleTest, zero_color_errorNonValgrind )
{
    InSequence dummy;
    const uint8_t lLinesCount( 0 );
    ASSERT_THROW( NumberRule( RED, lLinesCount )
                  , std::runtime_error);
}

TEST( NumberRuleTest, two_colors )
{
    const uint8_t lLinesCount( 3 );
    InSequence dummy;
    NumberRule::Ptr lRule( new NumberRule( RED, lLinesCount ) );
    
    std::vector< std::string > lLines;
    std::string lBaseLine( "This is a Line" );
    
    typedef uint8_t testedType;
    for ( testedType lI( 0 ) 
            ; lI < std::numeric_limits< testedType >::max() / 20
            ; ++lI )
    {
        std::stringstream lStream;
        lStream << lBaseLine << " " << lI;
        lLines.push_back( lStream.str() );
    }

    for_each( lLines.begin(), lLines.end(), [=]( std::string aLine ) 
           { uint32_t lI( 0 )
            ; MockIntermediateResult lIResult;
            ; EXPECT_CALL( lIResult, putMarker( 0, RED ) )
            ; EXPECT_CALL( lIResult, putMarker( aLine.size(), RED ) )
            ; lRule->apply( aLine, lIResult, ++lI ); } );

}


TEST( GroupRuleTest, group_rule_single_rule )
{
    const bool lColorizeWholeLines( true );
    const std::string lRegex( "[0-9]+" );
    InSequence dummy;
    Rule::Ptr lRule( new Rule( RED, lRegex, lColorizeWholeLines ) );
    RuleBox lBox;
    lBox.addRule( lRule );
    RuleGroup::Ptr lGroup( new RuleGroup( lBox ) );

    MockIntermediateResult lIResult;

    std::string lFirstLine( "§ first line " );
    std::string lSecondLine( "2 line " );
    std::string lThirdLine( "third line" );
    std::string lFourthLine( "4 line" );
    EXPECT_CALL( lIResult, putMarker( _, _ ) ).Times( 0 );
    lGroup->apply( lFirstLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 7, RED ) );
    lGroup->apply( lSecondLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( _, _ ) ).Times( 0 );
    lGroup->apply( lThirdLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 6, RED ) );
    lGroup->apply( lFourthLine, lIResult );
}

TEST( GroupRuleTest, group_rule_multiple_rules )
{
    const bool lColorizeWholeLines( true );
    const size_t lLinesCount( 3 );
    const std::string lRegex( "[0-9]+" );
    InSequence dummy;
    Rule::Ptr lRule( new Rule( BLUE, lRegex, lColorizeWholeLines ) );
    RuleBox lBox;
    lBox.addRule( lRule );
    lBox.addRule( IRule::Ptr( new NumberRule( RED, lLinesCount ) ) );
    RuleGroup::Ptr lGroup( new RuleGroup( lBox ) );

    MockIntermediateResult lIResult;

    std::string lFirstLine( "§ first line " );
    std::string lSecondLine( "2 line " );
    std::string lThirdLine( "third line" );
    std::string lFourthLine( "4 line" );
    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( lFirstLine.size(), RED ) );
    lGroup->apply( lFirstLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 7, RED ) );
    EXPECT_CALL( lIResult, putMarker( 0, BLUE ) );
    EXPECT_CALL( lIResult, putMarker( 7, BLUE ) );
    lGroup->apply( lSecondLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( lThirdLine.size(), RED ) );
    lGroup->apply( lThirdLine, lIResult );

    EXPECT_CALL( lIResult, putMarker( 0, RED ) );
    EXPECT_CALL( lIResult, putMarker( 6, RED ) );
    EXPECT_CALL( lIResult, putMarker( 0, BLUE ) );
    EXPECT_CALL( lIResult, putMarker( 6, BLUE ) );
    lGroup->apply( lFourthLine, lIResult );
}

TEST( ReferenceRuleTest, ProperTest )
{
    const std::string lLine( "some text" );
    const std::string lSchemeName( "scheme" )
                    , lRefScheme( "refScheme" );

    MockRule::Ptr lMockRule( new MockRule );
    RuleBox::Ptr lBox( new RuleBox );
    lBox->addRule( lMockRule );
    MockConfig::Ptr lConfig( new MockConfig );
    Config::RuleMap lMap;
    lMap[ lSchemeName ] = lBox;

    // create reference rule
    RuleBox::Ptr lRefBox( new RuleBox );
    lRefBox->addRule( ReferenceRule::Ptr( new ReferenceRule( lSchemeName, *lConfig ) ) );
    lMap[ lRefScheme ] = lRefBox;
    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( ::testing::Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_CALL( *lMockRule, apply( lLine, _, 0 ) ).Times( 1 );
    lRefBox->process( lLine, 0 );

    std::stringstream lStr;
    lStr << "a"; // prevent compiler from optimizing out
    // the same strings to the same address
    EXPECT_CALL( *lMockRule, apply( lStr.str(), _, 0 ) ).Times( 1 );
    lRefBox->process( lStr.str(), 0 );

    EXPECT_CALL( *lMockRule, apply( "a", _, 0 ) ).Times( 1 );
    lRefBox->process( "a", 0 );

}

TEST( ReferenceRuleTest, Non_existing_conf )
{
    const std::string lLine( "some text" );
    const std::string lSchemeName( "scheme" )
                    , lRefScheme( "refScheme" );

    MockRule::Ptr lMockRule( new MockRule );
    RuleBox::Ptr lBox( new RuleBox );
    lBox->addRule( lMockRule );
    MockConfig::Ptr lConfig( new MockConfig );
    Config::RuleMap lMap;
    // lMap[ lSchemeName ] = lBox; no RuleBox

    // create reference rule
    RuleBox::Ptr lRefBox( new RuleBox );
    lRefBox->addRule( ReferenceRule::Ptr( new ReferenceRule( lSchemeName, *lConfig ) ) );
    lMap[ lRefScheme ] = lRefBox;
    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( ::testing::Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_THROW( lRefBox->process( lLine, 0 ), std::runtime_error );
}

TEST( ReferenceRuleTest, loop )
{
    const std::string lLine( "some text" );
    const std::string lSchemeName( "scheme" );

    MockRule::Ptr lMockRule( new MockRule );
    MockConfig::Ptr lConfig( new MockConfig );
    Config::RuleMap lMap;

    // create reference rule
    RuleBox::Ptr lRefBox( new RuleBox );
    lRefBox->addRule( ReferenceRule::Ptr( new ReferenceRule( lSchemeName, *lConfig ) ) );
    lMap[ lSchemeName ] = lRefBox;
    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( ::testing::Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_THROW( lRefBox->process( lLine, 0 ), std::runtime_error );
}

}} // namespace Color::ColorTest

