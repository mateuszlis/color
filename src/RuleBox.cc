#include "RuleBox.hh"

namespace Color {

RuleBox::RuleBox()
{
    ;
}

void RuleBox::addRule( const IRule::Ptr& aRule )
{
    mRules.push_back( aRule );
}

std::string RuleBox::process( const std::string& aText, const uint64_t aLineNumber )
{
    return "";
}

} // namespace Color
