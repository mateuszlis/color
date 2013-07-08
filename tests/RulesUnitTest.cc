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


}} // namespace Color::ColorTest

