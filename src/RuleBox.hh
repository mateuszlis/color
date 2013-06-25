#pragma once

#include <tr1/memory>

#include "utils.hh"
#include "Rules.hh"

namespace Color {

class RuleBox
{
    public: // typedefs
        std::tr1::shared_ptr< RuleBox > Ptr;

    public: // functions
        RuleBox();
        void addRule( const IRule::Ptr& aRule );
        std::string process( const std::string& aText );

    protected: // fields
        std::list< IRule::Ptr > mRules;
}; // class RuleBox 

} // namespace Color
