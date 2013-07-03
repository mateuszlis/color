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

static const uint8_t ZERO( 0 ); // used against gtest warning about comparison of signed/unsigned
static const uint8_t ONE( 1 );
static const uint8_t THREE( 3 );
static const bool OPEN( false );
static const bool CLOSE( true );

TEST( IntermediateResultTest, Construct )
{
    IntermediateResult::Ptr lIntermediateResult( new IntermediateResult );
}

TEST( IntermediateResultTest, NoRules )
{
    InSequence dummy;
    IntermediateResult::Markers lMarkersRef;
    IntermediateResult::Ptr lIResult( new IntermediateResult );
    for ( uint8_t lI( 1 ) ; lI < 9 ; ++lI )
    {
        lIResult->getMarkers( lI, lMarkersRef );
        EXPECT_EQ( lMarkersRef.size(), ZERO );
    }
}

TEST( IntermediateResultTest, SingleRule_MultipleOpenClose )
{
    InSequence dummy;
    IntermediateResult::Ptr lIResult( new IntermediateResult );
    size_t lFirstMarker( 0 )
         , lSecondMarker( 2 )
         , lThirdMarker( 10 )
         , lLastMarker( 11 );

    lIResult->putMarker( lFirstMarker, RED );
    lIResult->putMarker( lSecondMarker, RED );
    lIResult->putMarker( lThirdMarker, RED );
    lIResult->putMarker( lLastMarker, RED );
    IntermediateResult::Markers lMarkersRef;

    ASSERT_NO_THROW( lIResult->getMarkers( lFirstMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, OPEN );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lSecondMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, CLOSE );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lThirdMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, OPEN );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lLastMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, CLOSE );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
}

TEST( IntermediateResultTest, SingleRule_SingleOpenClose )
{
    InSequence dummy;
    IntermediateResult::Ptr lIResult( new IntermediateResult );
    size_t lFirstMarker( 0 ), lLastMarker( 10 );

    lIResult->putMarker( lFirstMarker, RED );
    lIResult->putMarker( lLastMarker, RED );
    IntermediateResult::Markers lMarkersRef;
    for ( uint8_t lI( 1 ) ; lI < 9 ; ++lI )
    {
        lIResult->getMarkers( lI, lMarkersRef );
        EXPECT_EQ( lMarkersRef.size(), ZERO );
    }

    lIResult->getMarkers( lFirstMarker, lMarkersRef );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, OPEN );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    lIResult->getMarkers( lLastMarker, lMarkersRef );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, CLOSE );
    EXPECT_EQ( lMarkersRef.front().second, RED );
}

TEST( IntermediateResultTest, MultipleRules )
{
    InSequence dummy;
    IntermediateResult::Ptr lIResult( new IntermediateResult );
    size_t lFirstMarker( 0 ), lLastMarker( 10 );

    lIResult->putMarker( lFirstMarker, RED );
    lIResult->putMarker( lLastMarker, RED );
    lIResult->putMarker( lFirstMarker, BOLD_RED );
    lIResult->putMarker( lLastMarker, BOLD_RED );
    lIResult->putMarker( lFirstMarker, CYAN );
    lIResult->putMarker( lLastMarker, CYAN );
    IntermediateResult::Markers lMarkersRef;
    for ( uint8_t lI( 1 ) ; lI < 9 ; ++lI )
    {
        lIResult->getMarkers( lI, lMarkersRef );
        EXPECT_EQ( lMarkersRef.size(), ZERO );
    }

    ASSERT_NO_THROW( lIResult->getMarkers( lFirstMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), THREE );
    EXPECT_EQ( lMarkersRef.back().first, OPEN );
    EXPECT_EQ( lMarkersRef.back().second, CYAN );
    lMarkersRef.pop_back();
    EXPECT_EQ( lMarkersRef.back().first, OPEN );
    EXPECT_EQ( lMarkersRef.back().second, BOLD_RED );
    lMarkersRef.pop_back();
    EXPECT_EQ( lMarkersRef.back().first, OPEN );
    EXPECT_EQ( lMarkersRef.back().second, RED );
    lMarkersRef.pop_back();
    lMarkersRef.clear();

    ASSERT_NO_THROW( lIResult->getMarkers( lLastMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), THREE );
    EXPECT_EQ( lMarkersRef.back().first, CLOSE );
    EXPECT_EQ( lMarkersRef.back().second, CYAN );
    lMarkersRef.pop_back();
    EXPECT_EQ( lMarkersRef.back().first, CLOSE );
    EXPECT_EQ( lMarkersRef.back().second, BOLD_RED );
    lMarkersRef.pop_back();
    EXPECT_EQ( lMarkersRef.back().first, CLOSE );
    EXPECT_EQ( lMarkersRef.back().second, RED );
    lMarkersRef.pop_back();
    lMarkersRef.clear();
}

TEST( IntermediateResultTest, SingleRule_Extended_Position )
{
    InSequence dummy;
    const size_t MAX_SIZE( 5 );
    IntermediateResult::Ptr lIResult( new IntermediateResult( MAX_SIZE ) );
    size_t lFirstMarker( 0 )
         , lSecondMarker( 2 )
         , lThirdMarker( 10 )
         , lLastMarker( 11 )
         , lCrazyNumber( 10000 );

    lIResult->putMarker( lFirstMarker, RED );
    lIResult->putMarker( lSecondMarker, RED );
    lIResult->putMarker( lThirdMarker, RED );
    lIResult->putMarker( lLastMarker, RED );
    IntermediateResult::Markers lMarkersRef;

    ASSERT_NO_THROW( lIResult->getMarkers( lFirstMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, OPEN );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lSecondMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, CLOSE );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lThirdMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, OPEN );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();
    ASSERT_NO_THROW( lIResult->getMarkers( lLastMarker, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ONE );
    EXPECT_EQ( lMarkersRef.front().first, CLOSE );
    EXPECT_EQ( lMarkersRef.front().second, RED );
    lMarkersRef.clear();

    ASSERT_NO_THROW( lIResult->getMarkers( lCrazyNumber, lMarkersRef ) );
    ASSERT_EQ( lMarkersRef.size(), ZERO );
    lMarkersRef.clear();
}


}} // namespace Color::ColorTest

