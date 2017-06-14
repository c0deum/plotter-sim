#ifndef MOTOR_H
#define MOTOR_H

#include "typesdefs.h"

class Motor
{
public:    
    void setMaxVelocity( Float velocity );    
    void setAcceleration( Float acceleration );   
    void setTargetPosition( Float position );

    Float position() const;

    void update( Float dt );

private:
     Float maxVelocity_     = 1.0f;
     Float acceleration_    = 1.0f;
     Float velocity_        = 0.0f;
     Float currentPos_      = 0.0f;
     Float targetPos_       = 0.0f;
};

#endif // MOTOR_H
