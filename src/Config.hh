#pragma once

#include <memory>
#include <map>

#include "RuleBox.hh"

namespace Color {

class Config
{
    public: // constants
        static const char COMMENT_SIGN = '#';

    public: // typedefs
        typedef std::shared_ptr< Config > Ptr;
        typedef std::map< std::string, RuleBox > RuleMap;
        typedef std::function< Rule::Ptr( ColorName
                , const std::string&, bool ) > RuleCreator;
        typedef std::function< NumberRule::Ptr( ColorName
                , const uint8_t ) > NumberRuleCreator;
        typedef std::function< RuleBox::Ptr( void ) > RuleBoxCreator;

    public: // functions
        Config( std::istream& aFile );
        Config( std::istream& aFile
                , RuleCreator aRuleCreator
                , NumberRuleCreator aNumberRuleCreator
                , RuleBoxCreator aRuleBoxCreator );

        virtual const RuleBox::Ptr getRuleBox( const std::string& aName ) const;
        virtual const RuleMap& getAllRules() const;

    protected: // functions
        void parseConfig( std::istream& aStr );

    protected: // fields
        RuleMap m_Rules;
        RuleCreator m_CreateRule;
        NumberRuleCreator m_CreateNumberRule;
        RuleBoxCreator m_CreateRuleBox;

}; // class Config


} // namespace Color
