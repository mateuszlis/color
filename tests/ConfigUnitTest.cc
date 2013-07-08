// std
#include <iostream>
#include <memory>
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
        FakeRuleProducer::Ptr m_RuleProducer;
        Config::RuleCreator m_RuleCreator;
        Config::NumberRuleCreator m_NumberRuleCreator;
        Config::RuleBoxCreator m_RuleBoxCreator;
        RuleBox::Ptr m_DefaultBox;
    protected: // functions
        virtual void SetUp()
        {
            m_RuleProducer = FakeRuleProducer::Ptr( new FakeRuleProducer );
            m_DefaultBox.reset( new RuleBox );
            m_RuleCreator = std::bind(
                    &FakeRuleProducer::produceRule
                    , m_RuleProducer.get()
                    , std::placeholders::_1
                    , std::placeholders::_2
                    , std::placeholders::_3 );
            m_NumberRuleCreator = std::bind(
                    &FakeRuleProducer::produceNumberRule
                    , m_RuleProducer.get()
                    , std::placeholders::_1
                    , std::placeholders::_2 );
            m_RuleBoxCreator = std::bind(
                    &FakeRuleProducer::produceRuleBox
                    , m_RuleProducer.get() );
            //ON_CALL( m_RuleProducer, produceRuleBox() )
            //          .WillByDefault( Return( m_DefaultBox ) );
        }

        Config* createConfig( std::istream& aStr )
        {
            return new Config( aStr, m_RuleCreator, m_NumberRuleCreator, m_RuleBoxCreator );
        }

    void setExpectations()
    {
        MockRuleBox::Ptr lFirstRuleBox( new MockRuleBox )
            , lSecondRuleBox( new MockRuleBox );
        m_RuleProducer->addRuleBox( lFirstRuleBox );
        m_RuleProducer->addRuleBox( lSecondRuleBox );

        EXPECT_CALL( *m_RuleProducer, produceRuleBoxMock() ).Times( 2 );
        EXPECT_CALL( *m_RuleProducer, produceRuleMock( RED, REGEX, true ) )
            .Times( 1 );
        EXPECT_CALL( *m_RuleProducer, produceRuleMock( BROWN, REGEX, false ) )
            .Times( 1 );
        EXPECT_CALL( *lFirstRuleBox, addRule( _ ) ).Times( 2 );
        EXPECT_CALL( *m_RuleProducer, produceNumberRuleMock( BLUE, 3 ) )
            .Times( 1 );
        EXPECT_CALL( *lSecondRuleBox, addRule( _ ) ).Times( 1 );
    }

    void proceedWithSingleColorRuleConf( ColorName aCol, std::istream& aStr, const std::string& aRuleName )
    {
        EXPECT_CALL( *m_RuleProducer, produceRuleMock( aCol, REGEX, false ) )
            .Times( 1 );

        Config::Ptr lConfig( createConfig( aStr ) );

        ASSERT_EQ( lConfig->getAllRules().size(), ONE )
            << " Number of rules in file should be 1";
        RuleBox::Ptr lRuleBox;
        ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( aRuleName ) )
            << "RuleBox " << aRuleName << " not found";
    }

}; // class ConfigTest

TEST_F( ConfigTest, Construct )
{
    std::istringstream lStr( "" );
    Config::Ptr lConfig;
    EXPECT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}
#define SingleColorTest(col) \
TEST_F( ConfigTest, SingleBoxWithSingleRegexRule##col ) \
{ \
    InSequence lSeq; \
    const std::string RULE_NAME( "BoxName" ); \
    std::istringstream lStr(  "[" + RULE_NAME + "]\n" \
        "color=[" + #col + "]:" + REGEX + "\n" \
        ); \
    \
    proceedWithSingleColorRuleConf(col, lStr, RULE_NAME );\
}

// let's check if all colors are supported
SingleColorTest(RED);
SingleColorTest(BOLD_RED);
SingleColorTest(BROWN);
SingleColorTest(BOLD_BROWN);
SingleColorTest(CYAN);
SingleColorTest(BOLD_CYAN);
SingleColorTest(LIGHT_GRAY);
SingleColorTest(BOLD_LIGHT_GRAY);
SingleColorTest(BLUE);
SingleColorTest(BOLD_BLUE);
SingleColorTest(RESET);

TEST_F( ConfigTest, SingleBoxWithSingleNumberRule )
{
    InSequence lSeq;
    const std::string RULE_NAME( "BoxName" );
    std::istringstream lStr( "[" + RULE_NAME + "]\n"
            "alternate=3:[BLUE],[BROWN]\n" );

    MockNumberRule::Ptr lMockRule( new MockNumberRule );
    m_RuleProducer->addNumberRule( lMockRule );

    EXPECT_CALL( *m_RuleProducer, produceRuleBoxMock() )
        .Times( 1 );
        //.WillOnce( Return( RuleBox::Ptr( new RuleBox) ) );
    EXPECT_CALL( *m_RuleProducer, produceNumberRuleMock( BLUE, THREE ) )
        .Times( 1 );
        //.WillOnce( Return( lMockRule ) );
    EXPECT_CALL( *lMockRule, addColor( BROWN ) ).Times( 1 );

    Config::Ptr lConfig( createConfig( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), ONE )
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    lRuleBox = lConfig->getRuleBox( RULE_NAME );
    lMockRule.reset();
    //ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) )
    //    << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, SingleBoxWithSingleRule_WholeLine )
{
    const std::string RULE_NAME( "BoxName" );
    std::istringstream lStr(  "[" + RULE_NAME + "]\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        );

    EXPECT_CALL( *m_RuleProducer, produceRuleBoxMock() )
        .Times( 1 );
        //.WillOnce( Return( RuleBox::Ptr( new RuleBox) ) );
    EXPECT_CALL( *m_RuleProducer, produceRuleMock( RED, REGEX, true ) )
        .Times( 1 );
        //.WillOnce( Return( Rule::Ptr( new Rule( RED, REGEX, true ) ) ) );

    Config::Ptr lConfig( createConfig( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), ONE )
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) )
        << "RuleBox " << RULE_NAME << " not found";
}

// helper for next few tests


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

    setExpectations();

    Config::Ptr lConfig( createConfig( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), TWO )
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    //ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) )
    //    << "RuleBox " << RULE_NAME << " not found";
    //ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( SECOND_RULE ) )
    //    << "RuleBox " << RULE_NAME << " not found";
}

TEST_F( ConfigTest, ConfigWithComments )
{
    InSequence lSeq;
    const std::string RULE_NAME( "BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "# sdkjasldjasvl\n"
        "# rule comment\n"
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    setExpectations();

    Config::Ptr lConfig( createConfig( lStr ) );
    ASSERT_EQ( lConfig->getAllRules().size(), TWO )
        << " Number of rules in file should be 1";
    RuleBox::Ptr lRuleBox;
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( RULE_NAME ) )
        << "RuleBox " << RULE_NAME << " not found";
    ASSERT_NO_THROW( lRuleBox = lConfig->getRuleBox( SECOND_RULE ) )
        << "RuleBox " << RULE_NAME << " not found";

}

TEST_F( ConfigTest, WrongBoxName )
{
    InSequence lSeq;
    const std::string RULE_NAME( " BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    Config::Ptr lConfig;
    ASSERT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}

TEST_F( ConfigTest, WrongBoxName2 )
{
    InSequence lSeq;
    const std::string RULE_NAME( "*BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_line=[RED]:" + REGEX + "\n"
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    Config::Ptr lConfig;
    ASSERT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}

TEST_F( ConfigTest, SpacesInContent )
{
    InSequence lSeq;
    const std::string RULE_NAME( "BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_line=[RED] :" + REGEX + "\n" // space error
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    Config::Ptr lConfig;
    ASSERT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}

TEST_F( ConfigTest, SpacesInContent2 )
{
    InSequence lSeq;
    const std::string RULE_NAME( "BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_line= [RED]:" + REGEX + "\n" // space error
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    Config::Ptr lConfig;
    ASSERT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}

TEST_F( ConfigTest, WrongPrefixName )
{
    InSequence lSeq;
    const std::string RULE_NAME( "BoxName" ), SECOND_RULE( "Name" );
    std::istringstream lStr(
        "[" + RULE_NAME + "]\n"
        "#comment\n"
        "color_full_lise= [RED]:" + REGEX + "\n" // error
        "#color=[BROWN]:ala\n"
        "color=[BROWN]:" + REGEX + "\n"
        "#aas\n"
        "\n"
        "[" + SECOND_RULE + "]\n"
        "\n"
        "alternate=3:[BLUE],[BROWN]\n" );

    Config::Ptr lConfig;
    ASSERT_THROW( lConfig = Config::Ptr( new Config( lStr ) ), std::runtime_error );
}
}} // namespace Color::ColorTest

