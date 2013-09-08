
// tested lib
#include "CLHandler.hh"

// others
#define protected public
#include "RuleBox.hh"
#include "Rules.hh"
#undef protected

// Google test tools
#include "gtest/gtest.h"
#include "mocks.hh"

namespace Color { namespace Test {

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
class CLHandlerTest : public ::testing::Test
{
    protected: // fields
        FakeRuleProducer::Ptr m_RuleProducer;
        CLHandler::ConfigCreator m_ConfigCreator;
        CLHandler::ConfFinder m_ConfigFinder;
        RuleBox::Ptr m_DefaultBox;
    protected: // functions
        virtual void SetUp()
        {
            m_RuleProducer = FakeRuleProducer::Ptr( new FakeRuleProducer );
            m_DefaultBox.reset( new RuleBox );
            m_ConfigCreator = std::bind(
                    &FakeRuleProducer::produceConfig
                    , m_RuleProducer.get()
                    , std::placeholders::_1 );
            m_ConfigFinder = []( std::ifstream& ) { return true; };

        }


}; // class CLHandlerTest

class CLHandlerTestWParam : public CLHandlerTest
                          , public ::testing::WithParamInterface<const char*>
{
}; // class CLHandlerTestWParam

class CLHandlerTestError : public CLHandlerTest
                          , public ::testing::WithParamInterface<const char*>
{
}; // class CLHandlerTestError

TEST_F( CLHandlerTest, Construct_empty )
{
    char* lArgv[] = { const_cast< char* >( "" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 0, lArgv ) ) );
}

TEST_F( CLHandlerTest, Construct_not_entirely_empty )
{
    char* lArgv[] = { const_cast< char* >( "color" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 1, lArgv ) ) );
}

TEST_F( CLHandlerTest, Construct_scheme_name_not_exists )
{
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( "non_existent" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 2, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;

    ON_CALL( *lConfig, getAllRules() ).WillByDefault( Return( lMap ) );

    EXPECT_THROW( lHandler->produceRules(), std::runtime_error );
}


TEST_F( CLHandlerTest, Construct_scheme_name_exists )
{
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( "gcc" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 2, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;
    lMap[ "gcc" ] = RuleBox::Ptr( new RuleBox );
    lConfig->setRules( lMap );

    ON_CALL( *lConfig, getAllRules() ).WillByDefault( Return( lMap ) );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );

    EXPECT_EQ( lMap[ "gcc" ].get(), lReturned.get() );
}

TEST_P( CLHandlerTestWParam, regexp_arg )
{
    static const char* REGEX( ".*" );
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( GetParam() )
                    , const_cast< char* >( REGEX )
    };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 3, lArgv ) ) );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );

    ASSERT_EQ( lReturned->m_Rules.size(), 1u );
    IRule::Ptr lRetIRule( lReturned->m_Rules.front() );
    Rule::Ptr lRetRule( std::dynamic_pointer_cast< Rule >( lRetIRule ) );
    EXPECT_EQ( lRetRule->m_Regex, boost::regex( REGEX ) );
}

using ::testing::_;

TEST_F( CLHandlerTest, two_schemes )
{
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( "gcc" )
                    , const_cast< char* >( "valgrind" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 3, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;
    MockRuleBox::Ptr lValgrindBox( new MockRuleBox );
    MockRuleBox::Ptr lGccBox( new MockRuleBox );
    lMap[ "valgrind" ] = lValgrindBox;
    lMap[ "gcc" ] = lGccBox;

    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_CALL( *lGccBox, addBox( _ ) )
        .Times( 1 ).WillOnce( ReturnRef( *lGccBox ) );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );

    EXPECT_EQ( lMap[ "gcc" ].get(), lReturned.get() );
}


TEST_F( CLHandlerTest, five_schemes )
{
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( "gcc" )
                    , const_cast< char* >( "cmake" )
                    , const_cast< char* >( "gtest" )
                    , const_cast< char* >( "log" )
                    , const_cast< char* >( "valgrind" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 6, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;
    MockRuleBox::Ptr lValgrindBox( new MockRuleBox );
    MockRuleBox::Ptr lGccBox( new MockRuleBox );
    MockRuleBox::Ptr lCmakeBox( new MockRuleBox );
    MockRuleBox::Ptr lGtestBox( new MockRuleBox );
    MockRuleBox::Ptr lLogBox( new MockRuleBox );
    lMap[ "valgrind" ] = lValgrindBox;
    lMap[ "gcc" ] = lGccBox;
    lMap[ "cmake" ] = lCmakeBox;
    lMap[ "gtest" ] = lGtestBox;
    lMap[ "log" ] = lLogBox;

    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_CALL( *lGccBox, addBox( _ ) )
        .Times( 4 ).WillRepeatedly( ReturnRef( *lGccBox ) );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );

    EXPECT_EQ( lMap[ "gcc" ].get(), lReturned.get() );
}
TEST_P( CLHandlerTestWParam, scheme_and_regex )
{
    static const char* REGEX( ".*" );
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( "gcc" )
                    , const_cast< char* >( GetParam() )
                    , const_cast< char* >( REGEX ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 4, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;
    MockRuleBox::Ptr lValgrindBox( new MockRuleBox );
    MockRuleBox::Ptr lGccBox( new MockRuleBox );
    lMap[ "gcc" ] = lGccBox;

    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( Return( lMap ) );
    lConfig->setRules( lMap );

    EXPECT_CALL( *lGccBox, addBox( _ ) )
        .Times( 1 ).WillOnce( ReturnRef( *lGccBox ) );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );
}


TEST_P( CLHandlerTestWParam, regex_and_scheme )
{
    static const char* REGEX( ".*" );
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( GetParam() )
                    , const_cast< char* >( REGEX )
                    , const_cast< char* >( "gcc" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 4, lArgv ) ) );
    lHandler->setCustomBehaviour( m_ConfigFinder
                                , m_ConfigCreator );
    MockConfig::Ptr lConfig( new MockConfig() );
    m_RuleProducer->addConfig( lConfig );

    Config::RuleMap lMap;
    MockRuleBox::Ptr lValgrindBox( new MockRuleBox );
    MockRuleBox::Ptr lGccBox( new MockRuleBox );
    lMap[ "gcc" ] = lGccBox;

    ON_CALL( *lConfig, getAllRules() )
        .WillByDefault( Return( lMap ) );
    lConfig->setRules( lMap );

    RuleBox::Ptr lReturned;
    EXPECT_NO_THROW( lReturned = lHandler->produceRules() );

    ASSERT_EQ( lReturned->m_Rules.size(), 1u );
    IRule::Ptr lRetIRule( lReturned->m_Rules.front() );
    Rule::Ptr lRetRule( std::dynamic_pointer_cast< Rule >( lRetIRule ) );
    EXPECT_EQ( lRetRule->m_Regex, boost::regex( REGEX ) );
}

INSTANTIATE_TEST_CASE_P( scheme_and_short_regex
                        , CLHandlerTestWParam
                        , ::testing::Values( "-r", "--regex" ) );


// error case

TEST_P( CLHandlerTestError, error_nonexistent_option )
{
    static const char* REGEX( ".*" );
    char* lArgv[] = { const_cast< char* >( "color" )
                    , const_cast< char* >( GetParam() )
                    , const_cast< char* >( REGEX )
                    , const_cast< char* >( "gcc" ) };
    CLHandler::Ptr lHandler;
    ASSERT_NO_THROW( lHandler.reset( new CLHandler( 4, lArgv ) ) );
    EXPECT_THROW( lHandler->produceRules(), std::runtime_error );

}

INSTANTIATE_TEST_CASE_P( error_case
                        , CLHandlerTestError
                        , ::testing::Values( "-l", "--r", "-w", "--w", "-s", "--scheme", "-", "+" ) );
}} // namespace Color::Test
