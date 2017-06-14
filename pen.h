#ifndef PEN_H
#define PEN_H

#include <array>
#include <memory>
#include <ostream>
#include <fstream>
#include <string>

#include "typesdefs.h"

class Motor;

class Pen
{
public:
    enum class Axis
    {
        X = 0,
        Y
    };

    explicit Pen( const std::string & name );
    ~Pen();

    void setToggle( bool toggled );    

    void attachMotorToAxis( const std::shared_ptr< Motor > & motor, Axis axis );

    void log( Float logTime );

private:
    std::string name_;

    bool toggled_ = false;

    std::ofstream os_;

    std::array< std::shared_ptr< Motor >, 2 > attachedMotors_ = { nullptr };

    bool stopLog = false;
};

#endif // PEN_H
