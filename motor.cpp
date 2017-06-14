#include "motor.h"

void Motor::setMaxVelocity( Float velocity )
{
    maxVelocity_ = velocity;
}

void Motor::setAcceleration( Float acceleration )
{
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
    Float sign = ( targetPos_ > currentPos_  )? 1.0 : -1.0;

    if( velocity_ > maxVelocity_ )
        velocity_ = maxVelocity_;

    if( velocity_ < -maxVelocity_ )
        velocity_ = -maxVelocity_;

    Float oldPos = currentPos_;

    currentPos_ += velocity_ * dt;


    if( ( targetPos_ - oldPos ) * ( targetPos_ - currentPos_ ) <= 0.0 )
    {        
        currentPos_ = targetPos_;
    }

    velocity_ += sign * acceleration_ * dt;
}
