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

#ifndef RTRT_PROFILING_PROFILING_HPP
#define RTRT_PROFILING_PROFILING_HPP

#ifdef USE_PROFILING

#define BEGIN_LOOP( name ){ rtrt::logging::beginLoop( name ); }
#define END_LOOP { rtrt::logging::endLoop( "ms" ); }
#define END_LOOP_SECONDS { rtrt::logging::endLoop( "s" ); }
#define END_LOOP_MICROSECONDS { rtrt::logging::endLoop( "mcs" ); }

#define BEGIN_ITERATION( name ){ rtrt::logging::beginIteration( name ); }
#define END_ITERATION { rtrt::logging::endIteration( "ms" ); };
#define END_ITERATION_MICROSECONDS { rtrt::logging::endIteration( "mcs" ); };
#define END_ITERATION_SECONDS { rtrt::logging::endIteration( "s" ); };
#define BEGIN_PHASE( name ){ rtrt::logging::beginPhase( name ); }
#define END_PHASE { rtrt::logging::endPhase( "ms" ); }
#define END_PHASE_MICROSECONDS {  rtrt::logging::endPhase( "mcs" ); }
#define END_PHASE_SECONDS {  rtrt::logging::endPhase( "s" ); }
#define ADD_VALUE( name, value ) { rtrt::logging::addValue( name, value ); }
#define ADD_VALUE_MEASURED( name, value, measure ) { rtrt::logging::addValue( name, value, measure ); }

#else

#define BEGIN_LOOP( name )
#define END_LOOP
#define END_LOOP_SECONDS
#define END_LOOP_MICROSECONDS
#define BEGIN_ITERATION( name )
#define END_ITERATION
#define END_ITERATION_MICROSECONDS
#define END_ITERATION_SECONDS
#define BEGIN_PHASE( name ) 
#define END_PHASE 
#define END_PHASE_MICROSECONDS
#define END_PHASE_SECONDS
#define ADD_VALUE( name, value )
#define ADD_VALUE_MEASURED( name, value, measure )

#endif

#include <string>
#include "Logging.hpp"

namespace rtrt
{
  namespace logging
  {
    void beginPhase( const std::string& name );
    void endPhase( const std::string& measure );
    
    void beginLoop( const std::string& name );
    void endLoop( const std::string& measure );
    
    template< class T >
    void beginIteration( const T& name )
    {
      Logging::logger().log( "profiling", "iteration", "+", name );
    }

    void endIteration( const std::string& measure );
    
    template< class T >
    void addValue( const std::string& name, const T& value )
    {
      Logging::logger().log( "profiling", "value", name, value );
    }

    template< class T >
    void addValue( const std::string& name, const T& value, const std::string& measure )
    {
      Logging::logger().log( "profiling", "value", name, value, measure );
    }
  }
}
#endif
