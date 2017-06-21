#include <cassert>

#include "motor.h"

#include "pen.h"

Pen::Pen( const std::string & name )
    : name_( name )
{
    assert( !name_.empty() );

    os_.open( name + ".log" );
}

Pen::~Pen()
{
    os_.close();
}

void Pen::setToggle( bool toggled )
{
    std::lock_guard< std::mutex > guard( mutex_ );
    toggled_ = toggled;
    if( !toggled_ )
        stopLog = false;
}

void Pen::attachMotorToAxis( const std::shared_ptr<Motor> & motor, Pen::Axis axis )
{
    attachedMotors_[ static_cast< std::size_t >( axis ) ] = motor;
}

void Pen::log( Float logTime )
{
    std::lock_guard< std::mutex > guard( mutex_ );
    if( !toggled_ )
    {
        if( !stopLog )
        {
            os_ << logTime << ";-;-"  << std::endl;
            stopLog = true;
        }
        return;
    }
    os_ << logTime << ";"
        << attachedMotors_[ static_cast< std::size_t >( Axis::X ) ]->position() << ";"
        << attachedMotors_[ static_cast< std::size_t >( Axis::Y ) ]->position() << std::endl;
}
