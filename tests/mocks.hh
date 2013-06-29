#pragma once

#include "gmock/gmock.h"

namespace Color {

class MockIntermediateResult : public IntermediateResult 
{
    public:
        MOCK_METHOD2(putMarker,
            void(size_t aIndex, RuleIndex aRuleIndex));
        MOCK_CONST_METHOD2(getMarkers,
            void(size_t aIndex, std::vector< RuleIndex >& aRules));
}; // class MockIntermediateResult

}  // namespace Color


