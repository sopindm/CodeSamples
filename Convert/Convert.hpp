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

#ifndef RTRT_CONVERT_CONVERT_HPP
#define RTRT_CONVERT_CONVERT_HPP

#include <utility>
#include <Scenegraph/Node.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! Class containing functions for reading and writing models. */
    class Convert
    {
    public:
      /*! Loads model from specified file */
      static scenegraph::NodePtr load( const std::string& filename, bool checkScene = false );

      /*! Saves model to specified file
       *\return true is success. False otherwise. */
      static bool save( scenegraph::NodePtr node, const std::string& filename, const std::string& extension = "" );

      /*! Parses filename into name and extension */
      static std::pair< std::string, std::string > parseFilename( const std::string& filename );

    private:
      static SceneIStreamPtr createIStream( const std::string& extension, std::tr1::shared_ptr< std::istream > baseStream );
      static SceneOStreamPtr createOStream( const std::string& extension, std::tr1::shared_ptr< std::ostream > baseStream );

      static scenegraph::NodePtr innerLoad( SceneIStreamPtr stream );
      static scenegraph::NodePtr assimpLoad( const std::string& filename );

      static bool isRTRTExtension( const std::string& extension );
    };
  }
}
#endif
