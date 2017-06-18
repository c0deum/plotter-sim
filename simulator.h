#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <chrono>
#include "typesdefs.h"

#include "plotter.h"

class Simulator
{
public:
    enum class State
    {
        CONF,
        SIMS,
        FIN
    };

    void simulate();

    void update();
private:
    void parse( const std::string & line );

    void parseCreate( std::stringstream & lineStream );
    void parseAttach( std::stringstream & lineStream );
    void parseSet( std::stringstream & lineStream );

    bool isValidToken( const std::string token, const std::string & regexp );        
private:
    Plotter plotter_;
    State state_ = State::CONF;
    Float simDt_ = 1.0;
    Float simDuration_ = 0.0;
    Float logDt_ = 1.0;
    Float logDuration_ = 0.0;
    Float time_ = 0.0;
    std::chrono::time_point< std::chrono::high_resolution_clock > old_;
    std::chrono::time_point< std::chrono::high_resolution_clock > now_;
};

#endif // SIMULATOR_H
