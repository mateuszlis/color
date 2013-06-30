#pragma once

#include "gmock/gmock.h"

namespace Color {

class MockIntermediateResult : public IntermediateResult 
{
    public:
        MOCK_METHOD2(putMarker,
            void( size_t aIndex, const ColorName aColor ) );
        //MOCK_CONST_METHOD2( getMarkers,
        //    void( size_t aIndex, std::vector< const Marker >& aRules ) );
}; // class MockIntermediateResult

}  // namespace Color


