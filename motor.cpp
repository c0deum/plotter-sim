#include <cassert>
#include <cmath>
#include "motor.h"

void Motor::setMaxVelocity( Float velocity )
{
    assert( velocity >= 0.0 && "Max velocity must be >= 0" );
    maxVelocity_ = velocity;
}

void Motor::setAcceleration( Float acceleration )
{
    assert( acceleration >= 0.0 && "acceleration must be >= 0" );
    acceleration_ = acceleration;
}

void Motor::setTargetPosition( Float position )
{
    targetPos_ = position;
}

Float Motor::position() const
{
    return currentPos_;
}

void Motor::update( Float dt )
{
    //calculate movement direction
    Float sign = ( targetPos_ >= currentPos_  )? 1.0 : -1.0;

    if( velocity_ > maxVelocity_ )
        velocity_ = maxVelocity_;

    Float oldPos = currentPos_;

    currentPos_ += sign * velocity_ * dt;

    //value <= 0 means that we at target position
    if( ( targetPos_ - oldPos ) * ( targetPos_ - currentPos_ ) <= 0.0 )
    {
        //calculate part of dt
        if( std::abs( currentPos_ - oldPos ) != 0.0 )
        {
            dt *= std::abs( ( currentPos_ - targetPos_ ) / ( currentPos_ - oldPos ) );
        }

        velocity_ -= acceleration_ * dt;

        currentPos_ = targetPos_;
    }
    else
    {
        //increase velocity
        velocity_ += acceleration_ * dt;
    }

    //velocite must be not negative
    if( velocity_ < 0.0 )
        velocity_ = 0.0;
}
