#pragma once

#include "gmock/gmock.h"
#include <istream>

namespace Color {

class MockIntermediateResult : public IntermediateResult 
{
    public:
        MOCK_METHOD2( putMarker
            , void( size_t aIndex, const ColorName aColor ) );
        //MOCK_CONST_METHOD2( getMarkers,
        //    void( size_t aIndex, std::vector< const Marker >& aRules ) );
}; // class MockIntermediateResult


class MockRuleProducer
{
    public: // functions
        MOCK_METHOD3( produceRule
                , Rule::Ptr( ColorName, const std::string&
                    , bool ) );
        MOCK_METHOD2( produceNumberRule
                , NumberRule::Ptr( ColorName, const uint8_t ) );
}; // class MockRuleProducer


                
}  // namespace Color


