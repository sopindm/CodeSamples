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

#ifndef RTRT_CONVERT_TABLESCENEISTREAM_HPP
#define RTRT_CONVERT_TABLESCENEISTREAM_HPP

#include <istream>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! A stream reading data from existing nodes array */
    class TableSceneIStream 
    {
    public:
      /*! Constructs new stream from specified istream object */
      TableSceneIStream( const std::tr1::shared_ptr< std::istream >& stream );
      /*! Initializes node's offsets */
      void initOffsets();
    
      /*! Base stream used for reading */
      SceneIStreamPtr baseStream();
    
      /*! Sets base stream to read from specified node */
      void beginNode( uint32_t index );
    
      /*! Returns sha hash sum for node */
      std::string sha( uint32_t index );
    
    private:
      uint32_t totalSize();
      static std::string countSha( char* buffer, uint32_t size );
    
      uint32_t readSize();
    
      std::vector< uint32_t > _offsets;
    
      std::tr1::shared_ptr< std::istream > _stream;
      SceneIStreamPtr _binaryStream;
    };
  
    typedef std::tr1::shared_ptr< TableSceneIStream > TableSceneIStreamPtr;
  }
}
#endif
