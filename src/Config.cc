#include "Config.hh"


namespace Color {

const boost::regex Config::RULE_BOX_REG = boost::regex( "\\[[a-zA-Z0-9]\\]" );
// helper wrappers for constructors of rules
Rule::Ptr ruleWrapper( ColorName aColor, const std::string& aRegex
        , bool aWholeLine )
{
    return Rule::Ptr( new Rule( aColor, aRegex, aWholeLine ) );
}

NumberRule::Ptr numberRuleWrapper( ColorName aColor
        , const uint8_t aLinesNum )
{
    return NumberRule::Ptr( new NumberRule( aColor, aLinesNum ) );
}

RuleBox::Ptr createRuleBox()
{
    return RuleBox::Ptr( new RuleBox );
}

Config::Config( std::istream& aFile )
    : m_Rules()
    , m_CreateRule( &ruleWrapper )
    , m_CreateNumberRule( &numberRuleWrapper )
    , m_CreateRuleBox( &createRuleBox )
{
    parseConfig( aFile );
}

Config::Config( std::istream& aFile
        , RuleCreator aRuleCreator
        , NumberRuleCreator aNumberRuleCreator
        , RuleBoxCreator aRuleBoxCreator )
    : m_Rules()
    , m_CreateRule( aRuleCreator )
    , m_CreateNumberRule( aNumberRuleCreator )
    , m_CreateRuleBox( aRuleBoxCreator )
{
    parseConfig( aFile );
}
const RuleBox::Ptr Config::getRuleBox( const std::string& aName ) const
{
    return RuleBox::Ptr( new RuleBox );
}

const Config::RuleMap& Config::getAllRules() const
{
    return m_Rules;
}

void Config::parseConfig( std::istream& aStr )
{
    std::string lLine;
    RuleBox::Ptr lCurrentRuleBox;
    while ( std::getline( aStr, lLine, COMMENT_SIGN ) )
    {
        if ( lLine.size() )
        {
            if( boost::regex_match( lLine, RULE_BOX_REG ) )
            {
                lCurrentRuleBox = m_CreateRuleBox();
            }

        }
    }
    if ( !m_Rules.size() )
    {
        throw std::runtime_error( "Could not create any rules from configuration.");
    }
}

} // namespace Color
