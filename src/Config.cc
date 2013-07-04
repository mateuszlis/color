#include "Config.hh"


namespace Color {
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
    std::getline( aStr, lLine, COMMENT_SIGN );
}

} // namespace Color
