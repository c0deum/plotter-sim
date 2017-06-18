#include "utils.h"

#include "motor.h"

#include "plotter.h"

void Plotter::createMotor( const std::string & name )
{    
    validate( motors_.find( name ) == motors_.end(),
              "Motor with name " + name + " already exists..." );

    motors_[ name ] = std::make_shared< Motor >();
}

void Plotter::createPen( const std::string & name )
{
    validate( pens_.find( name ) == pens_.end(),
              "Pen with name " + name + " already exists..." );

    pens_[ name ] = std::make_shared< Pen >( name );
}

void Plotter::attachMotorToPenAxis( const std::string & motorName,
                                    const std::string & penName,
                                    Pen::Axis axis )
{
    validate( motors_.find( motorName ) != motors_.end(),
              "Can not find motor with name " + motorName );

    validate( pens_.find( penName ) != pens_.end(),
              "Can not find pen with name " + penName );

    auto & motor = motors_[ motorName ];
    auto & pen = pens_[ penName ];

    pen->attachMotorToAxis( motor, axis );
}

void Plotter::setMotorMaxVelocity( const std::string & name, Float velocity )
{    
    validate( motors_.find( name ) != motors_.end(),
              "Can not find motor with name " + name );

    motors_[ name ]->setMaxVelocity( velocity );
}

void Plotter::setMotorAcceleration( const std::string & name, Float acceleration )
{
    validate( motors_.find( name ) != motors_.end(),
              "Can not find motor with name " + name );

    motors_[ name ]->setAcceleration( acceleration );
}

void Plotter::setMotorTargetPosition( const std::string & name, Float position )
{
    validate( motors_.find( name ) != motors_.end(),
              "Can not find motor with name " + name );

    motors_[ name ]->setTargetPosition( position );
}


void Plotter::togglePen( const std::string & name, bool toggle )
{
    validate( pens_.find( name ) != pens_.end(),
              "Con not find pen with name " + name );

    pens_[ name ]->setToggle( toggle );
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
