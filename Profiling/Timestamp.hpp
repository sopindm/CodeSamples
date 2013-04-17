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

#ifndef RTRT_LOGGING_TIMESTAMP_HPP
#define RTRT_LOGGING_TIMESTAMP_HPP

#include <ctime>
#include <ostream>
#include <stdint.h>

namespace rtrt
{
  namespace logging
  {
    class Timestamp
    {
    public:
      static Timestamp current();

      Timestamp( const Timestamp& stamp );
      Timestamp( time_t seconds, uint32_t nanoseconds );
      void operator=( const Timestamp& stamp );

      bool operator==( const Timestamp& stamp ) const;
      bool operator!=( const Timestamp& stamp ) const;
      bool operator<( const Timestamp& stamp ) const;
      bool operator<=( const Timestamp& stamp ) const;
      bool operator>=( const Timestamp& stamp ) const;
      bool operator>( const Timestamp& stamp ) const;

      int delta( const Timestamp& stamp ) const;

    private:
      friend std::ostream& operator<<( std::ostream& stream, const Timestamp& stamp );
      Timestamp( const timespec& spec );

      time_t _seconds;
      uint32_t _nanoseconds;

    };
    
    std::ostream& operator<<( std::ostream& stream, const Timestamp& stamp );
  }
}
#endif
