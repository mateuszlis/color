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

TEST( RuleTest, Construct )
{
    Rule::Ptr lRule( new Rule( RED, "[0-9]+", 0 ) );
}

TEST( RuleTest, colorize_only_numbers_in_the_middle_of_text )
{
    const std::string lRegex( "[0-9]+" );
    const RuleIndex lIndex( 0 );

    Rule::Ptr lRule( new Rule( RED, lRegex, lIndex ) );
    ColorName lReset( RESET );
    MockIntermediateResult lIResult;
    EXPECT_CALL( lIResult, putMarker( 26, lIndex ) );
    EXPECT_CALL( lIResult, putMarker( 30, lIndex ) );
    EXPECT_CALL( lIResult, putMarker( 31, lIndex ) );
    EXPECT_CALL( lIResult, putMarker( 34, lIndex ) );

    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
    lRule->apply( lMiddleText, lIResult );
}

//TEST( RuleTest, colorize_everything )
//{
//    const std::string lRegex( ".*" );
//
//    Rule::Ptr lRule( new Rule( RED, lRegex ) );
//    ColorName lReset( RESET );
//    std::string lMiddleText( "This is text with numbers 1212 12 ha" );
//    std::string lResult( lRule->apply( lMiddleText, lReset ) );
//    EXPECT_EQ( lResult
//            , std::string( 
//                "\033[31mThis is text with numbers 1212 12 ha\033[0m" ) );
//}
//
//TEST( RuleTest, colorize_whole_lines_with_numbers )
//{
//    const bool lColorizeWholeLines( true );
//    const std::string lRegex( "[0-9]+" );
//
//    Rule::Ptr lRule( new Rule( RED, lRegex, lColorizeWholeLines ) );
//    std::string lFirstLine( "§ first line " );
//    std::string lSecondLine( "2 line " );
//    std::string lThirdLine( "third line" );
//    std::string lFourthLine( "4 line" );
//
//    ColorName lReset( RESET );
//    std::string lResult( lRule->apply( lFirstLine, lReset ) );
//    EXPECT_EQ( lResult, lFirstLine );
//
//    lResult = lRule->apply( lSecondLine, lReset );
//    EXPECT_EQ( lResult
//            , std::string( "\033[31m" + lSecondLine + "\033[0m" ) );
//
//    lResult = lRule->apply( lThirdLine, lReset );
//    EXPECT_EQ( lResult, lThirdLine );
//
//    lResult = lRule->apply( lFourthLine, lReset );
//    EXPECT_EQ( lResult
//            , std::string( "\033[31m" + lFourthLine + "\033[0m" ) );
//}
//
//TEST( NumberRuleTest, one_color )
//{
//    const uint8_t lLinesCount( 1 );
//    NumberRule::Ptr lRule( new NumberRule( lLinesCount, RED ) );
//    
//    std::vector< std::string > lLines;
//    std::string lBaseLine( "This is a Line" );
//    
//    typedef uint8_t testedType;
//    for ( testedType lI( 0 ) 
//            ; lI < std::numeric_limits< testedType >::max() 
//            ; ++lI )
//    {
//        std::stringstream lStream;
//        lStream << lBaseLine << " " << lI;
//        lLines.push_back( lStream.str() );
//    }
//
//    for_each(lLines.begin(), lLines.end(), [=]( std::string aLine ) 
//           { uint32_t lI( 0 )
//            ; ColorName lBackCol
//            ; EXPECT_EQ( "\033[31m" + aLine + "\033[0m"
//                , lRule->apply( aLine, lBackCol, ++lI ) );  });
//
//}
//
//TEST( NumberRuleTest, zero_color_error )
//{
//    const uint8_t lLinesCount( 0 );
//    NumberRule::Ptr lRule;
//    ASSERT_THROW(lRule = NumberRule::Ptr( new NumberRule( lLinesCount, RED ) ), std::runtime_error);
//}
//
//TEST( NumberRuleTest, two_colors )
//{
//    const uint8_t lLinesCount( 3 );
//    NumberRule::Ptr lRule( new NumberRule( lLinesCount, RED ) );
//    lRule->addColor( BOLD_RED );
//
//    std::vector< std::string > lLines;
//    std::string lBaseLine( "This is a Line" );
//
//    ColorName lBackCol;
//    for ( int lI( 0 ) ; lI < lLinesCount ; ++lI )
//    {
//        EXPECT_EQ( lRule->apply(lBaseLine, lBackCol, lI ), "\033[31m" + lBaseLine + "\033[0m" );
//    }
//
//    for ( int lI( 3 ) ; lI < 2 * lLinesCount ; ++lI )
//    {
//        EXPECT_EQ( lRule->apply(lBaseLine, lBackCol, lI ), "\033[1;31m" + lBaseLine + "\033[0m" );
//    }
//}

}} // namespace Color::ColorTest

