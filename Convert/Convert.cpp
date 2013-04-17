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

#include <string>
#include <fstream>
#include <boost/tr1/memory.hpp>
#include <glog/logging.h>
#include <Profiling.hpp>

#include "AssimpStaticLoader.hpp"
#include "MixedSceneIStream.hpp"
#include "MixedSceneOStream.hpp"
#include "BinarySceneIStream.hpp"
#include "BinarySceneOStream.hpp"
#include "XmlSceneIStream.hpp"
#include "XmlSceneOStream.hpp"
#include "SceneWriter.hpp"
#include "SceneReader.hpp"
#include "Convert.hpp"

using std::pair;
using std::string;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::tr1::shared_ptr;
using namespace rtrt::convert;
using namespace rtrt::scenegraph;
using namespace rtrt;

NodePtr Convert::innerLoad( SceneIStreamPtr stream )
{
  SceneReader reader( stream );
    
  NodePtr ret = reader.read();

  if( !stream->correct() )
  {
    LOG( ERROR ) << stream->errorLog();
    exit( EXIT_FAILURE );
  }
    
  END_PHASE;

  return ret;  
}

NodePtr Convert::assimpLoad( const string& filename )
{
  BEGIN_PHASE( "loading" );

  AssimpStaticLoader loader;
  
  bool success = loader.load( filename.c_str() );
  NodePtr ret;

  if( success )
    ret = loader.toScenegraph();
  else
  {
    LOG( ERROR ) << "Assimp failed to load scene";
    exit( EXIT_FAILURE );
  }
  
  END_PHASE;
  return ret;
}

NodePtr Convert::load( const string& name, bool check )
{
  pair< string, string > filename = parseFilename( name );
  
  if( filename.second == "" )
  {
    LOG( ERROR ) << "Unknown extension for file " << name;
    exit( EXIT_FAILURE );
  }

  if( isRTRTExtension( filename.second ))
  {
    shared_ptr< istream > input( new ifstream( name.c_str() ));
    SceneIStreamPtr stream( createIStream( filename.second, input ));

    BEGIN_PHASE( "loading" );
    return innerLoad( stream );
  }
  else if( filename.second == "rtxml" )
  {
    BEGIN_PHASE( "loading" );
    ifstream input( name.c_str() );
    input.unsetf(std::ios::skipws);
    
    BEGIN_PHASE( "XmlParsing" );
    XmlNodePtr xml( XmlNode::parse( input ));
    END_PHASE;
    
    if( xml == NULL )
    {
      LOG( ERROR ) << "Failed to load model";
      exit( EXIT_FAILURE );
    }
    
    SceneIStreamPtr stream( new XmlSceneIStream( xml ));
    return innerLoad( stream );
  }
  else if( filename.second == "rtm" )
  {
    BEGIN_PHASE( "loading" );

    shared_ptr< istream > baseInput( new ifstream( name.c_str() ));
    baseInput->unsetf(std::ios::skipws);
    shared_ptr< istream > binaryInput( new ifstream( (name+".raw").c_str() ));
    
    BEGIN_PHASE( "XmlParsing" );

    shared_ptr< MixedSceneIStream > stream( new MixedSceneIStream( baseInput, binaryInput ));
    if( check )
      stream->setChecking( true );

    stream->init();
    END_PHASE;
    
    return innerLoad( stream );
  }
  
  return assimpLoad( name );
}

bool Convert::isRTRTExtension( const string& extension )
{
  if( extension == "rtb" )
    return true;

  return false;
}

pair< string, string > Convert::parseFilename( const string& name )
{
  size_t extensionPos( name.rfind( '.' ));
  if( extensionPos == string::npos ) 
    return pair< string, string >( name, "" );
  
  string extension = name.substr( extensionPos + 1 );
  if( extension.find( '/' ) != string::npos )
    return pair< string, string >( name, "" );
  
  string baseName = name.substr( 0, extensionPos );
  return make_pair( baseName, extension );
}

SceneIStreamPtr Convert::createIStream( const string& extension, shared_ptr< istream > baseStream )
{
  if( extension == "rtb" )
    return SceneIStreamPtr( new BinarySceneIStream( baseStream ));

  return SceneIStreamPtr();
}
    
SceneOStreamPtr Convert::createOStream( const string& extension, shared_ptr< ostream > baseStream )
{
  if( extension == "rtb" )
    return SceneOStreamPtr( new BinarySceneOStream( baseStream ));
  else if( extension == "rtxml" )
    return SceneOStreamPtr( new XmlSceneOStream( XmlNode::create( "xml" ) ));

  return SceneOStreamPtr();
}

bool Convert::save( NodePtr scene, const string& name, const string& extension )
{
  pair< string, string > filename( parseFilename( name ));
  if( extension != "" )
    filename.second = extension;
  
  if( filename.second == "rtm" )
  {
    string outName( filename.first + '.' + filename.second );
    string binaryName( outName + ".raw" );
    
    shared_ptr< ostream > output( new ofstream( outName.c_str() ));
    shared_ptr< ostream > binaryOutput( new ofstream( binaryName.c_str() ));
    
    SceneOStreamPtr stream( new MixedSceneOStream( output, binaryOutput ));
    
    SceneWriter writer( stream );
    writer.write( scene );
    
    MixedSceneOStream* mixed = dynamic_cast< MixedSceneOStream* >( stream.get() );
    mixed->flush();
    
    return true;
  }
  
  string outName( filename.first + '.' + filename.second );
  
  shared_ptr< ostream > output( new ofstream( outName.c_str() ));
  SceneOStreamPtr stream( createOStream( filename.second, output ));

  if( stream == NULL )
    return false;
  
  SceneWriter writer( stream );
  writer.write( scene );
  
  if( !stream->correct() )
  {
    LOG( ERROR ) << stream->errorLog();
    return false;
  }
  
  if( filename.second == "rtxml" )
  {
    XmlSceneOStream* xmlStream = dynamic_cast< XmlSceneOStream* >( stream.get() );
    xmlStream->root()->write( *output );
  }
  
  return true;
}
