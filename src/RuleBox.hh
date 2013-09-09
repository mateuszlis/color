#pragma once

#include <memory>

#include "utils.hh"
#include "Rules.hh"

namespace Color {

class RuleBox
{
    public: // typedefs
        typedef std::shared_ptr< RuleBox > Ptr;

    public: // functions
        RuleBox();
        virtual void addRule( const IRule::Ptr& aRule );
        virtual std::string process( const std::string& aText
                , const uint64_t aLineNumber = 0 ) const;
        virtual RuleBox& addBox( const RuleBox& aBox );
        virtual RuleBox operator+( const RuleBox& aBox ) const;
        virtual const std::list< IRule::Ptr >& getRules() const;

    protected: // functions supporting processing
        IntermediateResult applyRules( const std::string& aText
                        , const size_t aLineNumber ) const;
        void singleProcess( const IRule::Ptr& aRule
                        , IntermediateResult& aResult
                        , const std::string& aLine
                        , const uint64_t aLineNumber ) const;
        void processLetter( const size_t aIndex
                        , std::list< ColorName >& aColors
                        , std::ostream& aOutput 
                        , const IntermediateResult& aIResult
                        , ColorName& aCurrentColor ) const;
        void removeFirstOccurence( const ColorName aColor
                        , std::list< ColorName >& aColors ) const;
        void processMarkerForLetter( 
                        const IntermediateResult::Marker& aMarker
                        , std::list< ColorName >& aColors
                        , bool& aReset
                        , ColorName& aCurrentColor ) const;
        void updateCurrentColor( std::ostream& aOutput
                        , bool& aReset
                        , ColorName& aCurrentColor
                        , const ColorName& aTopColor ) const;

    protected: // fields
        std::list< IRule::Ptr > m_Rules;
}; // class RuleBox

} // namespace Color
