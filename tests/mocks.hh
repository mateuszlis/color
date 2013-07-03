#pragma once

#include "gmock/gmock.h"
#include <istream>

namespace Color {

class MockIntermediateResult : public IntermediateResult 
{
    public:
        MOCK_METHOD2(putMarker,
            void( size_t aIndex, const ColorName aColor ) );
        //MOCK_CONST_METHOD2( getMarkers,
        //    void( size_t aIndex, std::vector< const Marker >& aRules ) );
}; // class MockIntermediateResult

class FakeIstream : public std::istream
{
    public:
        MOCK_METHOD2(getline
                , std::istream&( char* aBuffer, std::streamsize aSize ));

}; // class FakeIstream
}  // namespace Color


