#pragma once

#include <memory>
#include <map>

#include "RuleBox.hh"

namespace Color {

class Config
{
    public: // constants
        static const char COMMENT_SIGN = '#';
        static const boost::regex RULE_BOX_REG;
        static const boost::regex NUMBER_RULE_REG;
        static const boost::regex RULE_REG;
        static const boost::regex RULE_WHOLE_REG;
        static const boost::regex REF_RULE_REG;
        static const uint8_t OMIT_FIRST_BRACKET = 1;
        static const uint8_t NUMBER_OF_BRACKETS_RULEBOX = 2;
        static const uint8_t NUMBER_OF_WORDS = 2;
        static const uint8_t MIN_SIZE_NUM_RULE = 2;

    public: // typedefs
        typedef std::shared_ptr< Config > Ptr;
        typedef std::map< std::string, RuleBox::Ptr > RuleMap;
        typedef std::pair< std::string, RuleBox::Ptr > RuleMapElem;
        typedef std::function< Rule::Ptr( ColorName
                , const std::string&, bool ) > RuleCreator;
        typedef std::function< NumberRule::Ptr( ColorName
                , const uint8_t ) > NumberRuleCreator;
        typedef std::function< ReferenceRule::Ptr( 
            const std::string&, Config&  ) >ReferenceRuleCreator;
        typedef std::function< RuleBox::Ptr( void ) > RuleBoxCreator;
        typedef std::vector< std::string > Words;
        typedef std::vector< ColorName > Colors;

    public: // functions
        Config( std::istream& aFile );
        Config( std::istream& aFile
                , RuleCreator aRuleCreator
                , NumberRuleCreator aNumberRuleCreator
                , ReferenceRuleCreator aRefCreator
                , RuleBoxCreator aRuleBoxCreator );

        virtual const RuleBox::Ptr getRuleBox( const std::string& aName ) const;
        virtual const RuleMap& getAllRules() const;
        virtual const RuleBox::Ptr getFirstRuleBox() const
            { return m_FirstBox; }

    protected: // functions
        void parseConfig( std::istream& aStr );
        ColorName matchColor( const std::string& aColorStr );
        void preprocessLine( const std::string& aLine
                , Words& aValues );
        std::string getDataPart( const std::string& aLine );
        void handleRuleBox( RuleBox::Ptr& aCurrentRule
                , const std::string& aLine );
        void handleNumberRule( RuleBox::Ptr& aCurrentRuleBox
                , const std::string& aLine );
        void handleRule( RuleBox::Ptr& aCurrentRule
                , const std::string& aLine
                , const bool aWholeL );
        void handleError( const std::string& aLine
                , const size_t aLineNumber );
        void handleRefRule( const std::string& aLine 
                , RuleBox::Ptr& aCurrentRuleBox );

        // condition simplifiers
        bool isRelevant( const std::string& aLine )
        { return aLine.size() && aLine[ 0 ] != COMMENT_SIGN; }

        bool couldBeRuleBox( const std::string& aLine )
        { return boost::regex_match( aLine, RULE_BOX_REG ); }
        bool couldBeNumberRule( const std::string& aLine
                , const RuleBox::Ptr& aCurrentRuleBox )
        { return ( boost::regex_match( aLine, NUMBER_RULE_REG )
                && aCurrentRuleBox ); }
        bool couldBeRule( const std::string& aLine
                , const RuleBox::Ptr& aCurrentRuleBox )
        { return ( boost::regex_match( aLine, RULE_REG )
                && aCurrentRuleBox ); }
        bool couldBeWholeLineRule( const std::string& aLine
                , const RuleBox::Ptr& aCurrentRuleBox )
        { return ( boost::regex_match( aLine, RULE_WHOLE_REG )
                && aCurrentRuleBox ); }
        bool couldBeRefRule( const std::string& aLine
                , const RuleBox::Ptr& aCurrentRuleBox )
        { return ( boost::regex_match( aLine, REF_RULE_REG )
                && aCurrentRuleBox ); }

        // for Mock class
        Config() {}

    protected: // fields
        RuleMap m_Rules;
        RuleCreator m_CreateRule;
        NumberRuleCreator m_CreateNumberRule;
        RuleBoxCreator m_CreateRuleBox;
        ReferenceRuleCreator m_CreateRefRule;
        RuleBox::Ptr m_FirstBox;



}; // class Config


} // namespace Color
