// std
#include <iostream>
#include <limits>
#include <stdexcept>

// stl
#include <vector>

// Google test tools
#include "gtest/gtest.h"

// project-local
#include "utils.hh"
#include "Rules.hh"
#include "RuleBox.hh"
#include "Config.hh"
#include "mocks.hh"
#include "regex.hh"
#include "testUtils.hh"

namespace Color { namespace Test {

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
class ConfigTest : public ::testing::Test 
{
    protected: // fields
        MockRuleProducer m_RuleProducer;
        Config::RuleCreator m_RuleCreator;
        Config::NumberRuleCreator m_NumberRuleCreator;
    protected: // functions
        virtual void SetUp() 
        {
            m_RuleCreator = std::bind( 
                    &MockRuleProducer::produceRule
                    , &m_RuleProducer
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , std::placeholders::_3 );
            m_NumberRuleCreator = std::bind(
                    &MockRuleProducer::produceNumberRule
                    , &m_RuleProducer
                    , std::placeholders::_1
                    , std::placeholders::_2 );
        }

}; // class ConfigTest

TEST_F( ConfigTest, Construct )
{
    std::istringstream lStr( "" );
    Config::Ptr lConfig;
    EXPECT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}

TEST_F( ConfigTest, SingleBoxWithSingleRegexRule )
{
    const std::string RULE_NAME( "BoxName" );
    std::istringstream lStr(  "[" + RULE_NAME + "]\n"
        "color=[RED]:" + REGEX + "\n"
        );
    
    EXPECT_CALL( m_RuleProducer, produceRule( RED, REGEX, false ) )
        .Times( 1 )
        .WillOnce( Return( Rule::Ptr( new Rule( RED, REGEX, false ) ) ) );

    Config::Ptr lConfig( new Config( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), ONE ) 
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) ) 
        << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, SingleBoxWithSingleNumberRule )
{
    const std::string RULE_NAME( "BoxName" );
    std::istringstream lStr( "[" + RULE_NAME + "]\n"
            "alternate=3:[BLUE],[BROWN]\n" );

    MockNumberRule::Ptr lMockRule( new MockNumberRule );
    EXPECT_CALL( m_RuleProducer, produceNumberRule( BLUE, 3 ) )
        .Times( 1 )
        .WillOnce( Return( lMockRule ) );
    EXPECT_CALL( *lMockRule, addColor( BROWN ) ).Times( 1 );

    Config::Ptr lConfig( new Config( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), ONE ) 
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) ) 
        << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, SingleBoxWithSingleRule_WholeLine )
{
    const std::string RULE_NAME( "BoxName" );
    std::istringstream lStr(  "[" + RULE_NAME + "]\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        );
    
    EXPECT_CALL( m_RuleProducer, produceRule( RED, REGEX, true ) )
        .Times( 1 )
        .WillOnce( Return( Rule::Ptr( new Rule( RED, REGEX, true ) ) ) );

    Config::Ptr lConfig( new Config( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), ONE ) 
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) ) 
        << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, MultipleBoxesMultipleRules )
{
    const std::string RULE_NAME( "BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(  "[" + RULE_NAME + "]\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        "color=[BROWN]:" + REGEX + "\n"
        "\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );
    
    EXPECT_CALL( m_RuleProducer, produceRule( RED, REGEX, true ) )
        .Times( 1 )
        .WillOnce( Return( Rule::Ptr( new Rule( RED, REGEX, true ) ) ) );
    EXPECT_CALL( m_RuleProducer, produceRule( BROWN, REGEX, false ) )
        .Times( 1 )
        .WillOnce( Return( Rule::Ptr( new Rule( BROWN, REGEX, false ) ) ) );
    EXPECT_CALL( m_RuleProducer, produceNumberRule( BLUE, 3 ) )
        .Times( 1 )
        .WillOnce( Return( NumberRule::Ptr( new NumberRule( BLUE, 3 ) ) ) );

    Config::Ptr lConfig( new Config( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), TWO ) 
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) ) 
        << "RuleBox " << RULE_NAME << " not found";
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( SECOND_RULE ) ) 
        << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, ConfigWithComments )
{

}

// wrong rule name
// 
// spaces
//
// wrong color name
//
// wrong prefix name
//
// empty rules
//
// every supported color
//
// too many rules
//
// too many boxes

}} // namespace Color::ColorTest

