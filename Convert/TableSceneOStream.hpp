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

#ifndef RTRT_CONVERT_TABLESCENEOSTREAM_HPP
#define RTRT_CONVERT_TABLESCENEOSTREAM_HPP

#include <ostream>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! Stream writing sections in a binary format */
    class TableSceneOStream 
    {
    public:
      /*! Constructs new stream from ostream object */
      TableSceneOStream( const std::tr1::shared_ptr< std::ostream >& stream );
    
      /*! Base binary stream used for writing */
      SceneOStreamPtr baseStream();
    
      /*! Begins writing of new node
       * \return Index of new node in stream 
       */
      uint32_t beginNode();
      /*! End writing of node
       * \return SHA hash value of created node 
       */
      std::string endNode();
    
      /*! Writes all stream information to specified ostream */
      void flush();
    private:
      void writeSize( const uint32_t& value );
    
      std::vector< uint32_t > _offsets;
    
      std::tr1::shared_ptr< std::ostream > _stream;
      std::tr1::shared_ptr< std::stringstream > _tempStream;
      SceneOStreamPtr _binaryStream;
    };
  
    typedef std::tr1::shared_ptr< TableSceneOStream > TableSceneOStreamPtr;
  }
}
#endif
