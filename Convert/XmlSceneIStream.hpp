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

#ifndef RTRT_CONVERT_XMLSCENEISTREAM_HPP
#define RTRT_CONVERT_XMLSCENEISTREAM_HPP

#include <stack>
#include <Xml/XmlNode.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! An input stream reading from xml hierarchy */
    class XmlSceneIStream : public SceneIStream
    {
    public:
      /*! Constructs new stream using existing xml */
      XmlSceneIStream( const XmlNodePtr& node );
    
      void read( uint32_t* size, const std::string& name );
      void read( int* value, const std::string& name );
      void read( float* value, const std::string& name );
      void read( std::string* value, const std::string& name );
    
      void enterSection( const std::string& name );
      void leaveSection();
    
      bool correct();
    protected:
      /*! Sets xml source */
      void setRoot( XmlNodePtr root );
      /*! Current xml node for stream */
      XmlNode* current();
    
    private:
      template< class T > 
      void doRead( T* value, const std::string& name );
    
      XmlNodePtr _root;
      XmlNode* _current;
    
      typedef std::map< std::string, std::pair< NodeSet::const_iterator, NodeSet::const_iterator > > ChildNodes;
      std::stack< ChildNodes > _childs;
      bool _correct;
    };
  }
}
#endif
