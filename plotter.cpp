#include <cassert>

#include "motor.h"

#include "plotter.h"

void Plotter::createMotor( const std::string & name )
{
    assert( motors_.find( name ) == motors_.end() );

    motors_[ name ] = std::make_shared< Motor >();
}

void Plotter::createPen( const std::string & name )
{
    assert( pens_.find( name ) == pens_.end() );

    pens_[ name ] = std::make_shared< Pen >( name );
}

void Plotter::attachMotorToPenAxis( const std::string & motorName, const std::string & penName, Pen::Axis axis )
{
    assert( motors_.find( motorName ) != motors_.end() );
    assert( pens_.find( penName ) != pens_.end() );

    auto & motor = motors_[ motorName ];
    auto & pen = pens_[ penName ];

    pen->attachMotorToAxis( motor, axis );
}

void Plotter::setMotorMaxVelocity( const std::string & name, Float velocity )
{
    assert( motors_.find( name ) != motors_.end() );

    motors_[ name ]->setMaxVelocity( velocity );
}

void Plotter::setMotorAcceleration( const std::string & name, Float acceleration )
{
    assert( motors_.find( name ) != motors_.end() );

    motors_[ name ]->setAcceleration( acceleration );
}

void Plotter::togglePen( const std::string & name, bool toggle )
{
    assert( pens_.find( name ) != pens_.end() );

    pens_[ name ]->setToggle( toggle );
}

void Plotter::setMotorTargetPosition( const std::string & name, Float position )
{
    assert( motors_.find( name ) != motors_.end() );

    motors_[ name ]->setTargetPosition( position );
}

void Plotter::update( Float dt )
{
    for( auto & motor : motors_  )
    {
        motor.second->update( dt );
    }
}

void Plotter::log( Float logTime )
{
    for( const auto & pen : pens_ )
    {
        pen.second->log( logTime );
    }
}
