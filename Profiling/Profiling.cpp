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

#include "Logging.hpp"
#include "Profiling.hpp"

using namespace rtrt::logging;

void rtrt::logging::beginPhase( const std::string& name )
{
  Logging::logger().log( "profiling", "phase", "+", name );
}

void rtrt::logging::endPhase( const std::string& measure )
{
  Logging::logger().log( "profiling", "phase", "-", measure );
}
    
void rtrt::logging::beginLoop( const std::string& name )
{
  Logging::logger().log( "profiling", "loop", "+", name );
}

void rtrt::logging::endLoop( const std::string& measure )
{
  Logging::logger().log( "profiling", "loop", "-", measure );
}

void rtrt::logging::endIteration( const std::string& measure )
{
  Logging::logger().log( "profiling", "iteration", "-", measure );
}
