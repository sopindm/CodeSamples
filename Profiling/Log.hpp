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

#ifndef RTRT_LOGGING_LOG_HPP
#define RTRT_LOGGING_LOG_HPP

#include <string>
#include "Timestamp.hpp"
#include "Logger.hpp"

namespace rtrt
{
  namespace logging
  {
    class Log
    {
    public:
      Log( const std::string& stream, 
	   const std::string& message, 
	   const std::vector< Logger::LogValue >& values, 
	   const Timestamp& timestamp );

      Log( const Log& log );

      void operator=( const Log& log );

      const std::string& stream() const { return _stream; };
      const std::string& message() const { return _message; };
      const Timestamp& timestamp() const { return _timestamp; };
      const std::vector< Logger::LogValue > values() const { return _values; };

    private:
      std::string _stream;
      std::string _message;
      std::vector< Logger::LogValue > _values;

      Timestamp _timestamp;

    };
  }
}
#endif
