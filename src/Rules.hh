#pragma once

#include <boost/regex.hpp>
#include <tr1/memory>
#include <tr1/functional>

#include "utils.hh"

namespace Color {
class IntermediateResult
{
    public: // typedefs
        typedef std::tr1::shared_ptr< IntermediateResult > Ptr;
        typedef uint32_t RuleIndex;
        typedef std::pair< bool, RuleIndex > Marker; ///< open/close, ruleIndex

    public: // functions
        IntermediateResult() {}
        virtual ~IntermediateResult() {}
        void putMarker( size_t aIndex, RuleIndex aRuleIndex );
        void getMarkers( size_t aIndex, std::vector< RuleIndex >& aRules ) const;
}; // class IntermediateResult

class IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< IRule > Ptr;

    public: // functions
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const = 0;
}; // class IRule

class Rule : public IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< Rule > Ptr;
        typedef std::tr1::function< std::string( const boost::smatch ) > Colorizer;

    public: // functions
        explicit Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines = false );
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;
        virtual ~Rule() {}

    protected:
        const bool mWholeLines;
        const ColorName mColor;
        const boost::regex mRegex;

}; // class Rule

class NumberRule : public IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< NumberRule > Ptr;

    public: // functions
        explicit NumberRule( const uint8_t aSimilarLinesCount = 2
                , const ColorName aInitialColor = ColorName::RED );
        void addColor( const ColorName aColor );
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;
        virtual ~NumberRule() {}

    protected:
        const uint8_t mSimilarLinesCount;
        std::vector< ColorName > mColors;
}; // class NumberRule

} // namespace Color
