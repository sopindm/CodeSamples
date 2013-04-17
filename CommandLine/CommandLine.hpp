/*
   -----------------------------------------------------------------------------
   |                 R T R T    C O M M A N D L I N E   L I B R A R Y                 |
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

#ifndef RTRT_COMMANDLINE_COMMANDLINE_HPP
#define RTRT_COMMANDLINE_COMMANDLINE_HPP
#include <ostream>
#include "Iterator.hpp"
#include "Action.hpp"

namespace rtrt
{
  /*! A command line for application */
  class CommandLine : public commandLine::Action
  {
  public:
    /*! Constructs command line object */
    explicit CommandLine( const std::string& programName = "" );
    
    /*! Parses command line */
    void parse( int argc, const char* argv[] );
  };

}
#endif

