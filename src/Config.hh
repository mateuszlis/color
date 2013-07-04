#pragma once

#include <memory>
#include <map>

#include "RuleBox.hh"

namespace Color {

class Config
{
    public: // typedefs
        typedef std::shared_ptr< Config > Ptr;
        typedef std::map< std::string, RuleBox > RuleMap;
        typedef std::function< Rule::Ptr( ColorName
                , const std::string&, bool ) > RuleCreator;
        typedef std::function< NumberRule::Ptr( ColorName
                , const uint8_t ) > NumberRuleCreator;

    public: // functions
        Config( std::istream& aFile );

        virtual const RuleBox::Ptr getRuleBox( const std::string& aName ) const;
        virtual const RuleMap& getAllRules() const;

        virtual void setRuleFactory( RuleCreator aRuleCreator );
        virtual void setNumberRuleFactory( NumberRuleCreator aNumberRuleCreator );

    protected: // fields
        RuleMap m_Rules;
        RuleCreator m_CreateRule;
        NumberRuleCreator m_CreateNumberRule;

}; // class Config


} // namespace Color
