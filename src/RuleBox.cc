#include "RuleBox.hh"

#include <stack>
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
    m_Rules.push_back( aRule );
}

std::string RuleBox::process( const std::string& aText, const uint64_t aLineNumber )
{
    IntermediateResult lIResult( aText.size() );
    std::function< void( const IRule::Ptr& ) >
        lProcessor ( std::bind( &RuleBox::singleProcess
                    , this
                    , std::placeholders::_1
                    , std::ref( lIResult )
                    , aText
                    , aLineNumber )
                   );
    std::for_each( begin( m_Rules ), end( m_Rules ), lProcessor );

    std::stringstream lOutput;
    std::stack< ColorName > lColors;
    ColorName lCurrentColor( RESET );
    for ( size_t lIndex( 0 ) ; lIndex < aText.size() ; ++lIndex )
    {
        IntermediateResult::Markers lMarkers;
        lIResult.getMarkers( lIndex, lMarkers );
        ColorName lOpeningColor;
        bool lReset( false );
        if ( !lMarkers.empty() )
        {
            for ( size_t lRuleIndex( 0 ) ; lRuleIndex < lMarkers.size() ; ++lRuleIndex )
            {
                if ( !lMarkers[ lRuleIndex ].first )
                {
                    lColors.push( lMarkers[ lRuleIndex ].second );
                    lOpeningColor = lColors.top();
                }
                else
                {
                    if ( lColors.top() == lMarkers[ lRuleIndex ].second )
                    {
                        lColors.pop();
                    }
                    if ( lColors.empty() )
                    {
                        lReset = true;
                        lCurrentColor = RESET;
                    }
                }
            }
            if ( lReset || lCurrentColor != lColors.top() )
            {
                lOutput << COLOR_NAMES[ lColors.top() ];
                lReset = false;
            }
            lCurrentColor = lColors.top();
        }
        lOutput << aText[ lIndex ];
    }
    if ( !lColors.empty() )
        lOutput << COLOR_NAMES[ RESET ];
    return lOutput.str();
}

void RuleBox::singleProcess( const IRule::Ptr& aRule
                            , IntermediateResult& aResult
                            , const std::string& aLine
                            , const uint64_t aLineNumber )
{
    aRule->apply( aLine, aResult, aLineNumber );
}

} // namespace Color
