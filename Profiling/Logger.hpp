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

#ifndef RTRT_LOGGING_LOGGER_HPP
#define RTRT_LOGGING_LOGGER_HPP

#include <stdint.h>
#include <string>
#include <set>
#include <vector>
#include <boost/variant.hpp>

namespace rtrt
{
  namespace logging
  {
    class Listener;

    class Logger
    {
    public:
      Logger();

      typedef boost::variant< bool, int32_t, uint32_t, float, std::string > LogValue;

      void log( const std::string& stream, const std::string& message );

      template< class T >
      void log( const std::string& stream, const std::string& message, T value );

      template< class T1, class T2 >
      void log( const std::string& stream, const std::string& message, T1 value1, T2 value2 );

      template< class T1, class T2, class T3 >
      void log( const std::string& stream, const std::string& message, T1 value1, T2 value2, T3 value3 );

      template< class T1, class T2, class T3, class T4 >
      void log( const std::string& stream, const std::string& message, T1 value1, T2 value2, T3 value3, T4 value4 );

      template< class T1, class T2, class T3, class T4, class T5 >
      void log( const std::string& stream, const std::string& message, 
		T1 value1, T2 value2, T3 value3, T4 value4, T5 value5 );

      void operator+=( Listener& listener );
      void operator-=( Listener& listener );

    private:
      template< class T >
      class Store;

      void _log( const std::string& stream, const std::string& message, const std::vector< LogValue >& values );

      Logger( const Logger& );
      void operator=( const Logger& );

      std::set< Listener* > _listeners;

    };
  }
}
#include "Logger_impl.hpp"
#endif
