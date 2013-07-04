#pragma once

#include "gmock/gmock.h"
#include <istream>

namespace Color {

class MockIntermediateResult : public IntermediateResult 
{
    public:
        MOCK_METHOD2( putMarker
            , void( size_t aIndex, const ColorName aColor ) );
}; // class MockIntermediateResult

class MockNumberRule : public NumberRule
{
    public:  // typedefs
        typedef std::shared_ptr< MockNumberRule > Ptr;

    public: // functions
        MockNumberRule() : NumberRule( RED ) {}
        MOCK_METHOD1( addColor
               , void( const ColorName ) );

}; // class MockNumberRule
class MockRuleProducer
{
    public: // functions
        MOCK_METHOD3( produceRule
                , Rule::Ptr( ColorName, const std::string&
                    , bool ) );
        MOCK_METHOD2( produceNumberRule
                , NumberRule::Ptr( ColorName, const uint8_t ) );
        MOCK_METHOD0( produceRuleBox
                , RuleBox::Ptr( void ) );
}; // class MockRuleProducer

class MockRuleBox : public RuleBox
{
    public: // typedefs
        typedef std::shared_ptr< MockRuleBox > Ptr;

    public: // functions
        MOCK_METHOD1( addRule
                , void( const IRule::Ptr& ) );

}; // class MockRuleBox
                
}  // namespace Color


