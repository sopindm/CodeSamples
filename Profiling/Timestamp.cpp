/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O F I L I N G   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
                             
   Copyright (c) 2010 - 2012  Dmitry Sopin ( sopindm @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <iomanip>
#include "Timestamp.hpp"

using std::ostream;
using namespace  rtrt::logging;

Timestamp::Timestamp( const timespec& spec ): _seconds( spec.tv_sec ),
					      _nanoseconds( spec.tv_nsec )
{
}

Timestamp::Timestamp( time_t seconds, uint32_t nanoseconds ): _seconds( seconds ),
							      _nanoseconds( nanoseconds )
{
}

Timestamp::Timestamp( const Timestamp& stamp ): _seconds( stamp._seconds ),
						_nanoseconds( stamp._nanoseconds )
{
}

void Timestamp::operator=( const Timestamp& stamp )
{
  _seconds = stamp._seconds;
  _nanoseconds= stamp._nanoseconds;
}

bool Timestamp::operator==( const Timestamp& stamp ) const
{
  return ( _seconds == stamp._seconds ) && ( _nanoseconds == stamp._nanoseconds );
}

bool Timestamp::operator!=( const Timestamp& stamp ) const
{
  return !operator==( stamp );
}

bool Timestamp::operator<( const Timestamp& stamp ) const
{
  if( _seconds < stamp._seconds )
    return true;
  else if( _seconds > stamp._seconds )
    return false;

  return _nanoseconds < stamp._nanoseconds;
}

bool Timestamp::operator<=( const Timestamp& stamp ) const
{
  return (*this) < stamp  || (*this) == stamp;
}

bool Timestamp::operator>=( const Timestamp& stamp ) const
{
  return stamp <= (*this);
}

bool Timestamp::operator>( const Timestamp& stamp ) const
{
  return stamp < (*this);
}

Timestamp Timestamp::current()
{
  timespec spec;
  clock_gettime( CLOCK_MONOTONIC, &spec );
    
  return Timestamp( spec );
}

ostream& rtrt::logging::operator<<( ostream& stream, const Timestamp& stamp )
{
  stream << stamp._seconds;
  stream << std::setfill( '0' ) << std::setw( 9 ) << stamp._nanoseconds;

  return stream;
}

int Timestamp::delta( const Timestamp& stamp ) const
{
  return (_nanoseconds - stamp._nanoseconds) / 1000000 + (_seconds - stamp._seconds) * 1000;
}
