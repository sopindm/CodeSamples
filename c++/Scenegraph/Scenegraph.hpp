/*
   -----------------------------------------------------------------------------
   |                 R T R T    S C E N E G R A P H   L I B R A R Y                 |
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

#ifndef RTRT_SCENEGRAPH_SCENEGRAPH_HPP
#define RTRT_SCENEGRAPH_SCENEGRAPH_HPP

#include <map>

#include "Time.hpp"
#include "BaseStash.hpp"
#include "GeometryStash.hpp"
#include "Lights/LightStash.hpp"
#include "Materials/MaterialAttribute.hpp"

#define SCENEGRAPH_STASH( name, scene ) (boost::dynamic_pointer_cast< name >((scene)->scenegraph()->stash( #name )))

namespace rtrt
{
  namespace scenegraph
  {
    class Node;

    /*! Class representing inner scenegraph engine */
    class Scenegraph
    {
    public:
      /*! Creates new scenegraph with specified root node */
      Scenegraph( Node* root );
      
      /*! Current time value of scenegraph */
      const Time& time() const;
      /*! Updates scenegraph */
      void update( double time );

      /*! Scenegraph's root node */
      Node* root() const;

      /*! Registers node in scenegraph */
      void addNode( Node* node );
      /*! Unregisters node in scenegraph */
      void removeNode( Node* node );

      /*! Gets scenegraph's stash with specified name */
      BaseStashPtr stash( const std::string& name ) const;
      
      /*! Scenegraph's geometry stashes */
      const GeometryVector& geometry() const;
      /*! Scenegraph's material stashes */
      const MaterialStashVector& materials() const;
      /*! Scenegraph's light stashes */
      const LightVector& lights() const;

      bool haveEnvironmentMap() const;

      size_t triangles() const;

      float updateDelta() const { return _updateDelta; };
      void setUpdateDelta( float newDelta ) { _updateDelta = newDelta; };

    private:
      Scenegraph( const Scenegraph& scenegraph );
      void operator=( const Scenegraph& scenegraph );

      void ensureHaveStash( const std::string& name );

      void updateNode( Node* node, double time, const AttributeList& list );

      Node* _root;
      Time _time;

      typedef std::map< std::string, BaseStashPtr > StashMap;
      StashMap _stashes;

      GeometryVector _geometry;
      MaterialStashVector _materials;
      LightVector _lights;

      float _updateTime;
      float _updateDelta;
    };
  }
}
#endif
