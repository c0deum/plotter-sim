#ifndef PLOTTER_H
#define PLOTTER_H

#include <memory>
#include <string>
#include <map>

#include "pen.h"

class Motor;

class Plotter
{
public:
    void createMotor( const std::string & name );

    void createPen( const std::string & name );

    void attachMotorToPenAxis( const std::string & motorName, const std::string & penName, Pen::Axis axis );

    void setMotorMaxVelocity( const std::string & name, Float velocity );

    void setMotorAcceleration( const std::string & name, Float acceleration );

    void togglePen( const std::string & name, bool toggle );

    void setMotorTargetPosition( const std::string & name, Float position );

    void update( Float dt );

    void log( Float logTime );

private:
    std::map< std::string, std::shared_ptr< Pen > > pens_;
    std::map< std::string, std::shared_ptr< Motor > > motors_;
};

#endif // PLOTTER_H
