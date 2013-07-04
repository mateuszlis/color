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
        std::function< NumberRule::Ptr( ColorName, const uint8_t ) > m_NumberRuleCreator;
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




}} // namespace Color::ColorTest

