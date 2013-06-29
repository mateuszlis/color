#include "RuleBox.hh"

namespace Color {

void applyTheRule( std::string& aTextRef, IRule::Ptr aRule, const uint64_t aLineNumber, ColorName& aBackCol )
{
    //std::cout << "Applying " << aRule->apply( aTextRef, aBackCol, aLineNumber ) << std::endl;
    //aTextRef = aRule->apply( aTextRef, aBackCol, aLineNumber );
}

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
//    ColorName lBackColorName( RESET );
//    std::cout << "DUPA" << std::endl;
//    std::string lResult( aText );
//    std::tr1::function< void( IRule::Ptr ) > lApplier = std::tr1::bind
//        ( applyTheRule
//          , lResult 
//          , std::tr1::placeholders::_1
//          , aLineNumber
//          , lBackColorName );
//    std::for_each( begin( mRules ), end( mRules ), lApplier );
//    return lResult; 
}

} // namespace Color
