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

#ifndef RTRT_CONVERT_SCENEWRITER_HPP
#define RTRT_CONVERT_SCENEWRITER_HPP

#include <map>
#include <Scenegraph/Node.hpp>
#include <Scenegraph/Kernel.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    class SceneWriter
    {
    public:
      SceneWriter();
      explicit SceneWriter( SceneOStreamPtr stream );

      SceneOStreamPtr stream();
      void setStream( const SceneOStreamPtr& stream );

      void write( const scenegraph::NodePtr& scene );

    private:
      void traverseNode( const scenegraph::NodePtr& node );
      void writeNode( const scenegraph::NodePtr& node );
      void writeKernel( scenegraph::Kernel* kernel, uint32_t id );

      SceneWriter( const SceneWriter& writer );
      void operator=( const SceneWriter& writer );

      SceneOStreamPtr _stream;

      uint32_t kernelId( scenegraph::Kernel* kernel, bool addIfDoesNotExists = false );

      typedef std::map< scenegraph::Kernel*, size_t > KernelIds;
      typedef std::pair< scenegraph::Kernel*, size_t > KernelId;

      KernelIds _ids;
    };
  }
}
#endif
