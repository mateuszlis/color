#pragma once

#include "gmock/gmock.h"
#include <istream>

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
              , m_BoxIndex( 0 )
              , m_RuleIndex( 0 )
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
                std::cout << "REturned" << std::endl;
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
        MOCK_METHOD3( produceRuleMock
                , void( ColorName, const std::string&
                    , bool ) );
        MOCK_METHOD2( produceNumberRuleMock
                , void( ColorName, const uint8_t ) );
        MOCK_METHOD0( produceRuleBoxMock
                , void( void ) );

    public: // helper functions
        void addRuleBox( RuleBox::Ptr aBox ) { m_RuleBoxes.push_back( aBox ); };
        void addNumberRule( NumberRule::Ptr aRule ) { m_NumberRules.push_back( aRule ); };

    protected: // fields
        std::vector< RuleBox::Ptr > m_RuleBoxes;
        std::vector< NumberRule::Ptr > m_NumberRules;
        size_t m_BoxIndex;
        size_t m_RuleIndex;


}; // class FakeRuleProducer

class MockRuleBox : public RuleBox
{
    public: // typedefs
        typedef std::shared_ptr< MockRuleBox > Ptr;

    public: // functions
        MOCK_METHOD1( addRule
                , void( const IRule::Ptr& ) );

}; // class MockRuleBox

}  // namespace Color


