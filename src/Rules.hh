#pragma once

#include <boost/regex.hpp>
#include <memory>
#include <functional>

#include "utils.hh"

namespace Color {

class IntermediateResult
{
    public: // typedefs
        typedef std::shared_ptr< IntermediateResult > Ptr;
        typedef std::pair< bool, ColorName > Marker; ///< isClose, ruleIndex
        typedef std::vector< Marker > Markers;

    public: // functions
        explicit IntermediateResult( const size_t aSize = RESERVED_SIZE );
        virtual ~IntermediateResult() {}
        virtual void putMarker( size_t aIndex, const ColorName aColor );
        virtual void getMarkers( size_t aIndex, Markers& aRules ) const;

    protected: // functions
        void increaseSize( const size_t aSize );
    protected: // fields
        static const size_t RESERVED_SIZE = 1000;
        std::vector< Markers > m_RulesOnPositions;
        bool m_IsOpened;

}; // class IntermediateResult

class IRule
{
    public: // typedefs
        typedef std::shared_ptr< IRule > Ptr;

    public: // functions
        explicit IRule() {}
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const = 0;

}; // class IRule

class Rule : public IRule
{
    public: // typedefs
        typedef std::shared_ptr< Rule > Ptr;
        typedef std::function< std::string( const boost::smatch ) > Colorizer;

    public: // functions
        explicit Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines = false );
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
        typedef std::shared_ptr< NumberRule > Ptr;

    public: // functions
        explicit NumberRule( const ColorName aInitialColor
                , const uint8_t aSimilarLinesCount = 2 );
        virtual void addColor( const ColorName aColor );
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;
        virtual ~NumberRule() {}

    protected:
        const uint8_t m_SimilarLinesCount;
        std::vector< ColorName > m_Colors;
}; // class NumberRule

class RuleBox;

class RuleGroup : public IRule
{
    public: // typedefs
        typedef std::shared_ptr< RuleGroup > Ptr;

    public: // functions
        explicit RuleGroup( const RuleBox& aRuleBox );
        virtual ~RuleGroup() {}

        // IRule Interface
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;

    protected: // fields
        std::list< IRule::Ptr > m_Rules;

}; // class RuleGroup

class Config;

class ReferenceRule : public IRule
{
    public: // typedefs
        typedef std::shared_ptr< ReferenceRule > Ptr;

    public: // functions
        explicit ReferenceRule( const std::string& aSchemeName
                , const Config& aConfig);
        virtual ~ReferenceRule() {}
        // IRule interface
        virtual void apply( const std::string& aLine
                , IntermediateResult& aResContainer
                , uint64_t aLineNumber = 0 ) const;

    protected: // functions
        virtual void checkLoops( const std::string& aLine ) const;
    protected: // fields
        const std::string m_SchemeName;
        const Config& m_Config;
        mutable const char* m_InvestigatedString;
        mutable std::string  m_InvestigatedStringCopy;
}; // class ReferenceRule

} // namespace Color
