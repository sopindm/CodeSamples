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

#ifndef RTRT_CONVERT_SCENEREADER_HPP
#define RTRT_CONVERT_SCENEREADER_HPP

#include <map>
#include <Scenegraph/Node.hpp>
#include <Scenegraph/Kernel.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    class SceneReader
    {
    public:
      SceneReader();
      explicit SceneReader( SceneIStreamPtr stream );

      SceneIStreamPtr stream();
      void setStream( const SceneIStreamPtr& stream );

      scenegraph::NodePtr read();

    private:
      void readNode( scenegraph::NodePtr& node );
      void readKernel();

      SceneReader( const SceneReader& reader );
      void operator=( const SceneReader& reader );

      SceneIStreamPtr _stream;

      const scenegraph::KernelPtr& kernelById( uint32_t id );

      typedef std::map< size_t, scenegraph::KernelPtr > KernelIds;
      KernelIds _ids;
    };
  }
}
#endif
