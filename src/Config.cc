#include "Config.hh"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


namespace Color {

const boost::regex Config::RULE_BOX_REG = boost::regex( "\\[[a-zA-Z0-9]*\\]" );
const std::string COLOR_NAME ="(\\[(RED|GREEN|BLUE|BROWN)\\],)*\\[(RED|GREEN|BLUE|BROWN)\\]";
const boost::regex Config::NUMBER_RULE_REG = boost::regex( "alternate=[0-9]*:" + COLOR_NAME );
const boost::regex Config::RULE_REG = boost::regex( "color=" + COLOR_NAME + ":.*" );
const boost::regex Config::RULE_WHOLE_REG = boost::regex( "color_full_line=" + COLOR_NAME + ":.*" );
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

RuleBox::Ptr ruleBoxWrapper()
{
    return RuleBox::Ptr( new RuleBox );
}

Config::Config( std::istream& aFile )
    : m_Rules()
    , m_CreateRule( &ruleWrapper )
    , m_CreateNumberRule( &numberRuleWrapper )
    , m_CreateRuleBox( &ruleBoxWrapper )
{
    parseConfig( aFile );
}

Config::Config( std::istream& aFile
        , RuleCreator aRuleCreator
        , NumberRuleCreator aNumberRuleCreator
        , RuleBoxCreator aRuleBoxCreator )
    : m_Rules()
    , m_CreateRule( aRuleCreator )
    , m_CreateNumberRule( aNumberRuleCreator )
    , m_CreateRuleBox( aRuleBoxCreator )
{
    parseConfig( aFile );
}
const RuleBox::Ptr Config::getRuleBox( const std::string& aName ) const
{
    RuleMap::const_iterator lRulesIt( m_Rules.find( aName ) );
    std::cout << "AAA" << std::endl;
    if ( lRulesIt == m_Rules.end() )
    {
        throw std::runtime_error( "Expected rule cannot be found in config file " + aName );
    }
    std::cout << "AAA" << std::endl;
    return lRulesIt->second;
}

const Config::RuleMap& Config::getAllRules() const
{
    return m_Rules;
}

void Config::parseConfig( std::istream& aStr )
{
    std::string lLine;
    RuleBox::Ptr lCurrentRuleBox;
    while ( std::getline( aStr, lLine ) )
    {
        std::cout << "HERE" << lLine << "!" << std::endl;
        if ( lLine.size() && lLine[ 0 ] != COMMENT_SIGN )
        {
            if( boost::regex_match( lLine, RULE_BOX_REG ) )
            {
                std::cout << "HERE2" << std::endl;
                lCurrentRuleBox = m_CreateRuleBox();
                m_Rules.insert(
                        RuleMapElem( lLine.substr(
                                        OMIT_FIRST_BRACKET
                                        , lLine.size() - NUMBER_OF_BRACKETS_RULEBOX )
                            , lCurrentRuleBox ) );
            }
            else if ( boost::regex_match( lLine, NUMBER_RULE_REG ) )
            {
                std::cout << "Number" << std::endl;
                Words lValues; // unlucky name
                preprocessLine( lLine, lValues );
                std::cout << lValues[0] << "!" << std::endl;
                uint8_t lColorLineNumber(
                        boost::lexical_cast< int >( lValues[ 0 ] ) );
                Words lColorNames;
                boost::split( lColorNames, lValues[ 1 ], boost::is_any_of( "," ) );
                if ( lColorNames.size() < MIN_SIZE_NUM_RULE )
                {
                    throw std::runtime_error( "Not enough parameters in line " + lLine );
                }
                Colors lColors( lColorNames.size() );

                std::transform( begin( lColorNames ), end( lColorNames )
                        , begin( lColors )
                        , std::bind( &Config::matchColor
                                , this
                                , std::placeholders::_1 ) );

                NumberRule::Ptr lRule( m_CreateNumberRule( *begin( lColors )
                            , lColorLineNumber ) );

                std::for_each( ++begin( lColors ) // start from second element
                            , end( lColors )
                            , std::bind( &NumberRule::addColor
                                    , std::ref( *lRule )
                                    , std::placeholders::_1 ) );
                lCurrentRuleBox->addRule( lRule );
            }
            else if ( boost::regex_match( lLine, RULE_REG ) )
            {
                std::cout << "Rule" << std::endl;
                static const bool doNotColoWholeLines( false );
                handleRule( lCurrentRuleBox, lLine, doNotColoWholeLines );
            }
            else if ( boost::regex_match( lLine, RULE_WHOLE_REG ) )
            {
                std::cout << "Whole rule " << std::endl;
                static const bool colorWholeLines( true );
                handleRule( lCurrentRuleBox, lLine, colorWholeLines );
            }

        }
    }
    if ( !m_Rules.size() )
    {
        throw std::runtime_error( "Could not create any rules from configuration.");
    }
}

ColorName Config::matchColor( const std::string& aColorStr )
{
    std::cout << "In match color " << aColorStr << std::endl;
    if ( aColorStr == "[RED]" )
        return RED;
    if ( aColorStr == "[BOLD_RED]" )
        return BOLD_RED;
    if ( aColorStr == "[BROWN]" )
        return BROWN;
    if ( aColorStr == "[BOLD_BROWN]" )
        return BOLD_BROWN;
    if ( aColorStr == "[CYAN]" )
        return CYAN;
    if ( aColorStr == "[BOLD_CYAN]" )
        return BOLD_CYAN;
    if ( aColorStr == "[LIGHT_GRAY]" )
        return LIGHT_GRAY;
    if ( aColorStr == "[BOLD_LIGHT_GRAY]" )
        return BOLD_LIGHT_GRAY;
    if ( aColorStr == "[BLUE]" )
        return BLUE;
    if ( aColorStr == "[BOLD_BLUE]" )
        return BOLD_BLUE;
    if ( aColorStr == "[RESET]" )
        return RESET;
    return RESET;
}

void Config::preprocessLine( const std::string& aLine, Words& aValues )
{
    size_t lEqualizerLocation( aLine.find_first_of( "=" ) );
    std::string lValuePart( aLine.substr( lEqualizerLocation + 1
                , aLine.size() - lEqualizerLocation ) );
    size_t lSplitter( lValuePart.find_first_of( ":" ) );
    aValues.push_back( lValuePart.substr( 0, lSplitter ) );
    aValues.push_back( lValuePart.substr( lSplitter + 1, lValuePart.size() - lSplitter ) );
}

void Config::handleRule( RuleBox::Ptr& aCurrentRuleBox
        , const std::string& aLine
        , const bool aWholeL )
{
    Words lValues; // unlucky name
    preprocessLine( aLine, lValues );
    Rule::Ptr lRule( m_CreateRule( matchColor( lValues[ 0 ] )
                , lValues[ 1 ]
                , aWholeL ) );
    std::cout << "After" << std::endl;
    aCurrentRuleBox->addRule( lRule );
    std::cout << "AfterAfter" << std::endl;
}

} // namespace Color
