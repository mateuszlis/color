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

Config::Config( std::istream& aFile )
    : m_Rules() 
    , m_CreateRule( &ruleWrapper )
    , m_CreateNumberRule( &numberRuleWrapper )
{
    char* a = new char[ 100 ];
    aFile.getline( a, 100 );
    delete a;
}

const RuleBox::Ptr Config::getRuleBox( const std::string& aName ) const
{
    return RuleBox::Ptr( new RuleBox );
}

const Config::RuleMap& Config::getAllRules() const
{
    return m_Rules;
}

void Config::setRuleFactory( Config::RuleCreator aRuleCreator )
{
    m_CreateRule = aRuleCreator;
}

void Config::setNumberRuleFactory( Config::NumberRuleCreator aNumberRuleCreator )
{
    m_CreateNumberRule = aNumberRuleCreator; 
}
} // namespace Color
