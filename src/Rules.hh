#pragma once

#include <boost/regex.hpp>
#include <tr1/memory>
#include <tr1/functional>

#include "utils.hh"

namespace Color {
class IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< IRule > Ptr;
    public: // functions
        virtual std::string apply( const std::string& aLine, ColorName& aResetCol, uint64_t aLineNumber = 0 ) const = 0;
}; // class IRule

class Rule : public IRule
{
    public: // typedefs
        typedef std::tr1::shared_ptr< Rule > Ptr;
        typedef std::tr1::function< std::string( const boost::smatch ) > Colorizer;

    public: // functions
        explicit Rule( ColorName aColor, const std::string& aRegex, bool aWholeLines = false );
        virtual std::string apply( const std::string& aLine, ColorName& aResetCol, uint64_t aLineNumber = 0 ) const;
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
        virtual std::string apply( const std::string& aLine, ColorName& aResetCol, uint64_t aLineNumber = 0 ) const;
        virtual ~NumberRule() {}
    protected:
        const uint8_t mSimilarLinesCount;
        std::vector< ColorName > mColors;
}; // class NumberRule

} // namespace Color
