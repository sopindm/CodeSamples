#include <gtest/gtest.h>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <boost/assign/std/vector.hpp>
#include <Core/Types.hpp>
#include <Core/VectorUtils.hpp>
#include <Convert/XmlSceneIStream.hpp>
#include <Convert/XmlSceneOStream.hpp>
#include <Convert/BinarySceneIStream.hpp>
#include <Convert/BinarySceneOStream.hpp>
#include <Convert/PropertyWriter.hpp>
#include <Convert/PropertyReader.hpp>
#include <Property/PropertyBuilder.hpp>
#include <Property/VectorProperty.hpp>

using namespace boost::assign;
using std::string;
using std::vector;
using std::stringstream;
using std::tr1::shared_ptr;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::convert;
using namespace rtrt::property;

class PropertyVisitorTest : public testing::Test
{
public:
  void SetUp();
  
protected:
  
  SceneOStreamPtr stream;
  SceneIStreamPtr istream;

  PropertyWriter writer;
  PropertyReader reader;

  void setInput( const string& input )
  {
    stringstream stream( input );
    inputNode = XmlNode::create();
    inputNode->childs() += XmlNode::parse( stream );

    istream.reset( new XmlSceneIStream( inputNode ));
    reader.setStream( istream );
  }

  string output()
  {
    stringstream stream;
    (*outputNode->childs().begin())->write( stream );

    return stream.str();
  }

  XmlNodePtr inputNode;
  XmlNodePtr outputNode;
};

void PropertyVisitorTest::SetUp()
{
  outputNode = XmlNode::create();
  stream.reset( new XmlSceneOStream( outputNode ));

  istream.reset();

  writer.setStream( stream );
  reader.setStream( istream );
}

TEST_F( PropertyVisitorTest, IntPropertyWriting )
{
  stream->enterSection( "section" );

  writer.write( buildProperty( "value", 42 ));

  stream->leaveSection();

  string expected( "<section value=42/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, UIntPropertyWriting )
{
  stream->enterSection( "section" );

  writer.write( buildProperty( "value", static_cast< uint >( 42 ) ));

  stream->leaveSection();

  string expected( "<section value=42/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, FloatPropertyWriting )
{
  stream->enterSection( "section" );

  writer.write( buildProperty( "value", 12.375F ));

  stream->leaveSection();

  string expected( "<section value=12.375/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, StringPropertyWriting )
{
  stream->enterSection( "section" );

  writer.write( buildProperty< string >( "value", "sample string" ) );
  stream->leaveSection();

  string expected( "<section value=\"sample string\"/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, BoolPropertyWriting )
{
  stream->enterSection( "section" );

  writer.write( buildProperty( "value", false ) );
  writer.write( buildProperty( "value2", true ) );
  stream->leaveSection();

  string expected( "<section value=0 value2=1/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, ListPropertyWriting )
{
  writer.write( buildListProperty( "list",
				   buildProperty( "value1", 0 ),
				   buildProperty( "value2", 1.25F ),
				   buildProperty< string >( "value3", "a string" ) ) );

  string expected( "<list value1=0 value2=1.25 value3=\"a string\"/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, VectorPropertyWriting )
{
  Array2f vector;
  vector.push_back( Vector2f( 0, 1 ) );
  vector.push_back( Vector2f( 2, 3 ) );
  vector.push_back( Vector2f( 4, 5 ) );

  writer.write( buildProperty< Vector2f >( "array", "vector", vector ) );

  string expected( "<array size=3>\n"\
		   "  <vector x=0 y=1/>\n"\
		   "  <vector x=2 y=3/>\n"\
		   "  <vector x=4 y=5/>\n"\
		   "</array>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, AtomVectorPropertyWriting )
{
  vector< int > vector;
  vector.push_back( 1 );
  vector.push_back( 8 );
  vector.push_back( 64 );

  writer.write( buildProperty< int >( "vector", "element", vector ) );

  string expected( "<vector size=3>\n"\
		   "  <element value=1/>\n"\
		   "  <element value=8/>\n"\
		   "  <element value=64/>\n"\
		   "</vector>" );
  EXPECT_EQ( output(), expected );
}

template< class T >
string toBinary( const T& value )
{
  const char* data( reinterpret_cast< const char* >( &value ));
  return string( data, sizeof( T ));
}

TEST_F( PropertyVisitorTest, RawVectorWriting )
{
  vector< int > vector;
  vector.push_back( 1 );
  vector.push_back( 8 );
  vector.push_back( 64 );

  boost::shared_ptr< stringstream > baseStream( new stringstream );
  SceneOStreamPtr stream( new BinarySceneOStream( baseStream ));

  writer.setStream( stream );

  writer.write( buildProperty< int >( "vector", "element", vector ) );

  string expected( toBinary( 3 ) + toBinary( 1 ) + toBinary( 8 ) + toBinary( 64 ) );
  EXPECT_EQ( baseStream->str(), expected );
}

class MyConstructor
{
public:
  MyConstructor(): _length( 0 ) {}

  void setup( const string& value ) { _length = value.length(); };
  void construct( string& value ) { value.resize( _length ); };

  PropertyPtr propertizeConstructor()
  {
    return buildProperty( "length", _length );
  }

  static ListPropertyPtr propertize( string& value )
  {
    return buildListProperty( "", buildProperty( "string", value ) );
  }

private:
  int _length;

};

TEST_F( PropertyVisitorTest, ConstructorPropertyWriting )
{
  string value( "abc" );
  writer.write( buildConstructorProperty( "cons", value, MyConstructor() ));

  string expected( "<cons length=3 string=\"abc\"/>" );
  EXPECT_EQ( output(), expected );
}

TEST_F( PropertyVisitorTest, IntPropertyReading )
{
  setInput( "<section value=42/>" );

  istream->enterSection( "section" );

  int value;
  reader.read( buildProperty( "value", value ));

  istream->leaveSection();

  EXPECT_EQ( value, 42 );
}

TEST_F( PropertyVisitorTest, UIntPropertyReading )
{
  setInput( "<section value=1242/>" );

  istream->enterSection( "section" );

  uint32_t value;
  reader.read( buildProperty( "value", value ));

  istream->leaveSection();

  EXPECT_EQ( value, 1242 );
}

TEST_F( PropertyVisitorTest, FloatPropertyReading )
{
  setInput( "<section value=12.25/>" );

  istream->enterSection( "section" );

  float value;
  reader.read( buildProperty( "value", value ));

  istream->leaveSection();

  EXPECT_EQ( value, 12.25 );
}

TEST_F( PropertyVisitorTest, StringPropertyReading )
{
  setInput( "<section value=\"a string\"/>" );

  istream->enterSection( "section" );

  string value;
  reader.read( buildProperty( "value", value ));

  istream->leaveSection();

  EXPECT_EQ( value, "a string" );
}

TEST_F( PropertyVisitorTest, BoolPropertyReading )
{
  setInput( "<section value=0 value2=1/>" );

  istream->enterSection( "section" );

  bool value;
  reader.read( buildProperty( "value", value ));
  bool value2;
  reader.read( buildProperty( "value2", value2 ));

  istream->leaveSection();

  EXPECT_FALSE( value );
  EXPECT_TRUE( value2 );
}

TEST_F( PropertyVisitorTest, ListPropertyReading )
{
  setInput( "<list value1=42 value2=1.25 value3=\"some string\"/>" );

  int value1;
  float value2;
  string value3;

  reader.read( buildListProperty( "list",
				  buildProperty( "value1", value1 ),
				  buildProperty( "value2", value2 ),
				  buildProperty( "value3", value3 ) ) );

  EXPECT_EQ( value1, 42 );
  EXPECT_EQ( value2, 1.25 );
  EXPECT_EQ( value3, "some string" );
}

TEST_F( PropertyVisitorTest, VectorPropertyReading )
{
  setInput( "<vector size=4>\n"\
	    "  <elt value=1/>\n"\
	    "  <elt value=2/>\n"\
	    "  <elt value=4/>\n"\
	    "  <elt value=8/>\n"\
	    "</vector>" );

  vector< int > value;

  reader.read( buildProperty< int >( "vector", "elt", value ));

  ASSERT_EQ( value.size(), 4 );

  EXPECT_EQ( value[ 0 ], 1 );
  EXPECT_EQ( value[ 1 ], 2 );
  EXPECT_EQ( value[ 2 ], 4 );
  EXPECT_EQ( value[ 3 ], 8 );
}

TEST_F( PropertyVisitorTest, RawVectorReading )
{
  vector< int > vector;

  boost::shared_ptr< stringstream > baseStream( new stringstream );
  baseStream->str( toBinary( 3 ) + toBinary( 1 ) + toBinary( 8 ) + toBinary( 64 ) );

  SceneIStreamPtr stream( new BinarySceneIStream( baseStream ));
  reader.setStream( stream );

  reader.read( buildProperty< int >( "vector", "element", vector ) );

  ASSERT_EQ( vector.size(), 3 );

  EXPECT_EQ( vector[ 0 ], 1 );
  EXPECT_EQ( vector[ 1 ], 8 );
  EXPECT_EQ( vector[ 2 ], 64 );
}

class NameConstructor
{
public:
  NameConstructor( const std::string& name ): _name( name ) {};
  
  void setup( int& value ) {};
  void construct( int& value ) { value = 0; };

  PropertyPtr propertizeConstructor() { return buildProperty( "value", _name ); };
  ListPropertyPtr propertize( int& value ) 
  { 
    return buildListProperty( "constructed",
			      buildProperty( _name, value )); 
  };

private:
  std::string _name;

};

TEST_F( PropertyVisitorTest, ConstructorPropertyReading )
{
  setInput( "<cons value=\"myproperty\" myproperty=42/>" );

  int value;
  reader.read( buildConstructorProperty( "cons", value, NameConstructor( "myproperty" ) ) );

  EXPECT_EQ( value, 42 );
}


