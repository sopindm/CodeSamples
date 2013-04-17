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

#ifndef RTRT_CONVERT_XMLSCENEOSTREAM_HPP
#define RTRT_CONVERT_XMLSCENEOSTREAM_HPP

#include <Xml/XmlNode.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! An output stream writing to a xml hierarchy */
    class XmlSceneOStream : public SceneOStream
    {
    public:
      /*! Constructs new stream using xml node object to write into */
      XmlSceneOStream( const XmlNodePtr& node );
    
      /*! A root node of resulting xml */ 
      const XmlNodePtr& root() const;
    
      void write( uint32_t size, const std::string& name );
      void write( int value, const std::string& name );
      void write( float value, const std::string& name );
      void write( const std::string& value, const std::string& name );
    
      void enterSection( const std::string& name );
      void leaveSection();
    
      bool correct();

    private:
      template< class T > 
      void doWrite( T value, const std::string& name );
    
      XmlNodePtr _root;
      XmlNode* _current;
    
      bool _correct;
    };
  }
}
#endif
