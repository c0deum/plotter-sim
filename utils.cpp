#define NDEBUG
#include <cassert>
#include <string>
#include <stdexcept>

void validate( bool statement, const std::string & info )
{
    assert( statement );
    if( !statement )
        throw std::logic_error( info );
}
