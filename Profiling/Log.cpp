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

#include "Log.hpp"

using std::string;
using namespace rtrt::logging;

Log::Log( const std::string& stream, 
	  const std::string& message, 
	  const std::vector< Logger::LogValue >& values,
	  const Timestamp& timestamp ):
  _stream( stream ),
  _message( message ),
  _values( values ),
  _timestamp( timestamp )
{
}

Log::Log( const Log& log ): _stream( log.stream() ),
			    _message( log.message() ),
			    _values( log.values() ),
			    _timestamp( log.timestamp() )
{
}

void Log::operator=( const Log& log )
{
  _stream = log._stream;
  _message = log._message;
  _values = log._values;
  _timestamp = log._timestamp;
}


