#include <iostream>
#include <sstream>
#include <regex>
#include <cassert>
#include <thread>

#include "simulator.h"

static const std::string KEYWORD_ATTACH( "attach" );
static const std::string KEYWORD_CREATE( "create" );
static const std::string KEYWORD_DT_ASSIGN( "dT" );
static const std::string KEYWORD_LOG( "log" );
static const std::string KEYWORD_MOTOR( "motor" );
static const std::string KEYWORD_OF( "of" );
static const std::string KEYWORD_OFF( "off" );
static const std::string KEYWORD_ON( "on" );
static const std::string KEYWORD_PEN( "pen" );
static const std::string KEYWORD_SET( "set" );
static const std::string KEYWORD_SIM( "sim" );
static const std::string KEYWORD_START( "start" );
static const std::string KEYWORD_STOP( "stop" );
static const std::string KEYWORD_WITH( "with" );
static const std::string KEYWORD_X( "x" );
static const std::string KEYWORD_Y( "y" );
static const std::string KEYWORD_A_ASSIGN( "A=" );
static const std::string KEYWORD_S_ASSIGN( "S=" );
static const std::string KEYWORD_P_ASSIGN( "P=" );

static const std::string REGEX_NAME( "[A-Z_][A-Z0-9_]*" );
static const std::string REGEX_DECIMAL( "-?([0-9]*.[0-9]+|[0-9]+)" );

void Simulator::simulate()
{
    while( State::FIN != state_ )
    {
        std::string line;
        std::getline( std::cin, line );
        parse( line );
    }
}

void Simulator::update()
{
    //start time
    time_ = 0.0;
    now_ = std::chrono::high_resolution_clock::now();

    while( State::FIN != state_ )
    {
        old_ = now_;
        now_ = std::chrono::high_resolution_clock::now();

        std::chrono::duration<Float> duration = now_ - old_;

        simDuration_ += duration.count();

        if( simDuration_ > simDt_ )
        {
            simDuration_ -= simDt_;
            plotter_.update( simDt_ );
        }

        logDuration_ += duration.count();

        if( logDuration_ > logDt_ )
        {
            logDuration_ -= logDt_;

            time_ += logDt_;

            plotter_.log( time_ );
        }
    }
}

void Simulator::parse( const std::string & line )
{
    std::stringstream lineStream( line );

    std::string token;

    lineStream >> token;

    if( KEYWORD_START == token )
    {
        assert( State::CONF == state_ );
        state_ = State::SIMS;

        //start simulation thread
        std::thread simThread( &Simulator::update, this );
        simThread.detach();
    }
    else if( KEYWORD_STOP == token )
    {
        assert( State::SIMS == state_ );
        state_ = State::FIN;
    }
    else if( KEYWORD_CREATE == token )
    {
        assert( State::CONF == state_ );
        parseCreate( lineStream );
    }
    else if( KEYWORD_ATTACH == token )
    {
        assert( State::CONF == state_ );
        parseAttach( lineStream );
    }
    else if ( KEYWORD_SET == token )
    {
        parseSet( lineStream );
    }
    else
    {
        assert( false && "unknown command" );
    }
}

void Simulator::parseCreate( std::stringstream & lineStream )
{
    std::string token;

    lineStream >> token;
    if( KEYWORD_MOTOR == token )
    {
        std::string name;
        lineStream >> name;
        assert( isValidToken( name, REGEX_NAME ) );

        plotter_.createMotor( name );
    }
    else if( KEYWORD_PEN == token )
    {
        std::string name;
        lineStream >> name;
        assert( isValidToken( name, REGEX_NAME ) );

        plotter_.createPen( name );
    }
    else
    {
        assert( false && "unknown param" );
    }
}

void Simulator::parseAttach( std::stringstream & lineStream )
{    
    std::string motorName;
    lineStream >> motorName;
    assert( isValidToken( motorName, REGEX_NAME ) );

    std::string token;
    lineStream >> token;
    assert( KEYWORD_WITH == token );

    std::string axisName;
    lineStream >> axisName;
    assert( KEYWORD_X == axisName || KEYWORD_Y == axisName );

    static std::map< std::string, Pen::Axis >
            axisMap( { { KEYWORD_X, Pen::Axis::X },
                       { KEYWORD_Y, Pen::Axis::Y } } );

    lineStream >> token;
    assert( KEYWORD_OF == token );

    std::string penName;
    lineStream >> penName;
    assert( isValidToken( penName, REGEX_NAME ) );

    plotter_.attachMotorToPenAxis( motorName, penName, axisMap[ axisName ] );
}

void Simulator::parseSet( std::stringstream & lineStream )
{    
    std::string token;
    lineStream >> token;

    if( KEYWORD_SIM == token )
    {
        std::string dtToken;
        lineStream >> dtToken;
        assert( isValidToken( dtToken, "dT=" + REGEX_DECIMAL ) && dtToken.length() > 3 );

        dtToken = dtToken.substr( 3 );
        simDt_ = std::stod( dtToken );
    }
    else if( KEYWORD_LOG == token )
    {
        std::string dtToken;
        lineStream >> dtToken;
        assert( isValidToken( dtToken, "dT=" + REGEX_DECIMAL ) && dtToken.length() > 3 );

        dtToken = dtToken.substr( 3 );
        logDt_ = std::stod( dtToken );
    }
    else if( KEYWORD_PEN == token )
    {
        std::string penName;
        lineStream >> penName;

        std::string penState;
        lineStream >> penState;

        if( KEYWORD_ON == penState || KEYWORD_OFF == penState )
        {
            plotter_.togglePen( penName, KEYWORD_ON == penState );
        }
        else
        {
            assert( false && "unknown state" );
        }

    }
    else if( KEYWORD_MOTOR == token )
    {
        std::string motorName;
        lineStream >> motorName;

        std::string assignToken;

        lineStream >> assignToken;

        if( isValidToken( assignToken, KEYWORD_A_ASSIGN + REGEX_DECIMAL ) )
        {
            Float acceleration = std::stod( assignToken.substr( 2 ) );
            plotter_.setMotorAcceleration( motorName, acceleration );
        }
        else if( isValidToken( assignToken, KEYWORD_P_ASSIGN + REGEX_DECIMAL ) )
        {
            Float targetPosition = std::stod( assignToken.substr( 2 ) );
            plotter_.setMotorTargetPosition( motorName, targetPosition );
        }
        else if( isValidToken( assignToken, KEYWORD_S_ASSIGN + REGEX_DECIMAL ) )
        {
            Float maxVelocity = std::stod( assignToken.substr( 2 ) );
            plotter_.setMotorMaxVelocity( motorName, maxVelocity );
        }
        else
        {
            assert( false && "unknown param" );
        }
    }
}

bool Simulator::isValidToken( const std::string token, const std::string & regexp )
{
    return std::regex_match( token, std::regex( regexp ) );
}
