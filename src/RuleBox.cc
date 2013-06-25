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

std::string RuleBox::process( const std::string& aText )
{
    return "";
}

} // namespace Color
