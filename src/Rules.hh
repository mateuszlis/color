#pragma once

#include <boost/regex.hpp>
#include <tr1/memory>
#include <tr1/functional>

#include "utils.hh"

namespace Color {

typedef uint32_t RuleIndex;

class IntermediateResult
{
    public: // typedefs
        typedef std::tr1::shared_ptr< IntermediateResult > Ptr;
        typedef std::pair< bool, ColorName > Marker; ///< isClose, ruleIndex
        typedef std::vector< Marker > Markers;

    public: // functions
        explicit IntermediateResult( const size_t aSize = RESERVED_SIZE );
        virtual ~IntermediateResult() {}
        virtual void putMarker( size_t aIndex, const ColorName aColor );
        virtual void getMarkers( size_t aIndex, Markers& aRules ) const;

    protected: // fields
        static const size_t RESERVED_SIZE = 1000;
        std::vector< Markers > m_RulesOnPositions;

}; // class IntermediateResult

class IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< IRule > Ptr;

    public: // functions
        explicit IRule( const RuleIndex aIndex ) : m_Index( aIndex ) {}
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const = 0;

    protected: // fields
        const RuleIndex m_Index;

}; // class IRule

class Rule : public IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< Rule > Ptr;
        typedef std::tr1::function< std::string( const boost::smatch ) > Colorizer;

    public: // functions
        explicit Rule( ColorName aColor, const std::string& aRegex, const RuleIndex aIndex, bool aWholeLines = false );
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;
        virtual ~Rule() {}

    protected: // functions
        void applyWholeLine( const std::string& aLIne
                , IntermediateResult& aResContainer ) const;
        void applyPartial( const std::string& aLIne
                , IntermediateResult& aResContainer ) const;
    protected: // fields
        const bool m_WholeLines;
        const ColorName m_Color;
        const boost::regex m_Regex;

}; // class Rule

class NumberRule : public IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< NumberRule > Ptr;

    public: // functions
        explicit NumberRule( const ColorName aInitialColor
                , const RuleIndex aIndex 
                , const uint8_t aSimilarLinesCount = 2 );
        void addColor( const ColorName aColor );
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;
        virtual ~NumberRule() {}

    protected:
        const uint8_t m_SimilarLinesCount;
        std::vector< ColorName > m_Colors;
}; // class NumberRule

} // namespace Color
