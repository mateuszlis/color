#include "RuleBox.hh"

#include <stack>
namespace Color {

RuleBox::RuleBox()
{
    ;
}

void RuleBox::addRule( const IRule::Ptr& aRule )
{
    m_Rules.push_front( aRule );
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
    std::list< ColorName > lColors;
    ColorName lCurrentColor( RESET );
    for ( size_t lIndex( 0 ) ; lIndex < aText.size() ; ++lIndex )
    {
        IntermediateResult::Markers lMarkers;
        lIResult.getMarkers( lIndex, lMarkers );
        bool lReset( false );
        if ( !lMarkers.empty() )
        {
            for ( size_t lRuleIndex( 0 ) ; lRuleIndex < lMarkers.size() ; ++lRuleIndex )
            {
                if ( !lMarkers[ lRuleIndex ].first )
                {
                    lColors.push_front( lMarkers[ lRuleIndex ].second );
                }
                else
                {
                    if ( lColors.front() == lMarkers[ lRuleIndex ].second )
                    {
                        lColors.pop_front(); // ninenty percent of the time
                    }
                    else
                    {
                        removeFirstOccurence( lMarkers[ lRuleIndex ].second
                                , lColors );
                    }
                    if ( lColors.empty() )
                    {
                        lReset = true;
                        lCurrentColor = RESET;
                    }
                }
            }
            if ( lReset )
            {
                lOutput << COLOR_NAMES[ RESET ];
                lReset = false;
            }
            else if ( lCurrentColor != lColors.front() )
            {
                lOutput << COLOR_NAMES[ lColors.front() ];
            }
            lCurrentColor = lColors.front();
        }
        lOutput << aText[ lIndex ];
    }
    if ( !lColors.front() )
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

void RuleBox::removeFirstOccurence( const ColorName aColor
                            , std::list< ColorName >& aColors )
{
    for ( std::list< ColorName >::iterator lColIt( aColors.begin() )
            ; lColIt != aColors.end()
            ; ++lColIt )
    {
        if ( *lColIt == aColor )
        {
            aColors.erase( lColIt );
            return;
        }
    }
}

} // namespace Color
