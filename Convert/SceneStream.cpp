/*
   -----------------------------------------------------------------------------
   |                 R T R T    C O N V E R T   L I B R A R Y                 |
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

#include <boost/lexical_cast.hpp>
#include "SceneStream.hpp"

using std::string;
using boost::lexical_cast;

using namespace Eigen;
using namespace rtrt::convert;

SceneStream::SceneStream() : _errorLog()
{
}

const string& SceneStream::errorLog() const
{
  return _errorLog;
}

void SceneStream::pushError( const string& error )
{
  _errorLog += error + '\n';
}

bool SceneStream::correct()
{
  return _errorLog == "";
}

/*
 *  SceneIStream
 */

SceneIStream::SceneIStream(): SceneStream()
{
}

SceneOStream::SceneOStream() : SceneStream()
{
}

