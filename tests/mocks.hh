#pragma once

#include "gmock/gmock.h"
#include <istream>
#include "Config.hh"

namespace Color {

class MockIntermediateResult : public IntermediateResult
{
    public:
        MOCK_METHOD2( putMarker
            , void( size_t aIndex, const ColorName aColor ) );
}; // class MockIntermediateResult

class MockNumberRule : public NumberRule
{
    public:  // typedefs
        typedef std::shared_ptr< MockNumberRule > Ptr;

    public: // functions
        MockNumberRule() : NumberRule( RED ) {}
        MOCK_METHOD1( addColor
               , void( const ColorName ) );

}; // class MockNumberRule

class FakeRuleProducer
{
    public: // typedefs
        typedef std::shared_ptr< FakeRuleProducer > Ptr;
    public: // functions
        FakeRuleProducer()
            : m_RuleBoxes()
              , m_NumberRules()
              , m_Configs()
              , m_BoxIndex( 0 )
              , m_RuleIndex( 0 )
              , m_ConfigIndex( 0 )
        {}

        Rule::Ptr produceRule( ColorName aColor
                , const std::string& aRegex
                , bool aWholeLines )
        {
            produceRuleMock( aColor, aRegex, aWholeLines );
            return Rule::Ptr( new Rule( aColor
                            , aRegex
                            , aWholeLines ) );
        }
        NumberRule::Ptr produceNumberRule( ColorName aColor
                , const uint8_t aSimilarLines )
        {
            produceNumberRuleMock( aColor, aSimilarLines );
            if ( m_NumberRules.size() > m_RuleIndex )
            {
                return m_NumberRules[ m_RuleIndex++ ];
            }

            return NumberRule::Ptr( new NumberRule( aColor, aSimilarLines ) );
        }
        RuleBox::Ptr produceRuleBox()
        {
            produceRuleBoxMock();
            if ( m_RuleBoxes.size() > m_BoxIndex )
                return m_RuleBoxes[ m_BoxIndex++ ];

            return RuleBox::Ptr( new RuleBox );
        }

        Config::Ptr produceConfig( std::istream& aStream )
        {
            //produceConfigMock( aStream );
            if ( m_Configs.size() > m_ConfigIndex )
                return m_Configs[ m_ConfigIndex++ ];
            return Config::Ptr( new Config( aStream ) );
        }
        MOCK_METHOD3( produceRuleMock
                , void( ColorName, const std::string&
                    , bool ) );
        MOCK_METHOD2( produceNumberRuleMock
                , void( ColorName, const uint8_t ) );
        MOCK_METHOD0( produceRuleBoxMock
                , void( void ) );
        MOCK_METHOD1( produceConfigMock
                , void( std::istream& aStream ) );

    public: // helper functions
        void addRuleBox( RuleBox::Ptr aBox ) { m_RuleBoxes.push_back( aBox ); };
        void addNumberRule( NumberRule::Ptr aRule ) { m_NumberRules.push_back( aRule ); };
        void addConfig( Config::Ptr aConfig ) { m_Configs.push_back( aConfig ); };

    protected: // fields
        std::vector< RuleBox::Ptr > m_RuleBoxes;
        std::vector< NumberRule::Ptr > m_NumberRules;
        std::vector< Config::Ptr > m_Configs;
        size_t m_BoxIndex;
        size_t m_RuleIndex;
        size_t m_ConfigIndex;

}; // class FakeRuleProducer

class MockRuleBox : public RuleBox
{
    public: // typedefs
        typedef std::shared_ptr< MockRuleBox > Ptr;

    public: // functions
        MOCK_METHOD1( addRule
                , void( const IRule::Ptr& ) );
        MOCK_METHOD1( addBox
                , RuleBox&( const RuleBox& ) );
        virtual ~MockRuleBox() {}

}; // class MockRuleBox

class MockConfig : public Config
{
    public: // typedefs
        typedef std::shared_ptr< MockConfig > Ptr;

    public: // functions
        MockConfig() : Config() {}
        MOCK_METHOD0( getAllRules
                , Config::RuleMap() );
        MOCK_METHOD1( getRuleBoxMock
                , RuleBox::Ptr( const std::string& ) );

    public: // non-mock helpers
        void setRules( Config::RuleMap& aMap ) { m_Rules = aMap; }
        RuleBox::Ptr getRuleBox( const std::string& aName )
        {
            getRuleBoxMock( aName );
            return Config::getRuleBox( aName );
        }


}; // class MockConfig

}  // namespace Color


