#include <gtest/gtest.h>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <boost/assign/std/vector.hpp>
#include <Core/Types.hpp>
#include <Core/VectorUtils.hpp>
#include <Convert/BinarySceneOStream.hpp>

using namespace boost::assign;
using std::tr1::shared_ptr;
using std::string;
using std::stringstream;
using std::ios;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::convert;

class BinarySceneOStreamTest : public testing::Test
{
public:
  void SetUp();
  
protected:
  
  SceneOStreamPtr stream;
  shared_ptr< stringstream > stringStream;
};

void BinarySceneOStreamTest::SetUp()
{
  stringStream.reset( new stringstream( ios::in | ios::out ));
  stream.reset( new BinarySceneOStream( stringStream ));
}

template< class T >
string toBinary( const T& value )
{
  const char* data = reinterpret_cast< const char* >( &value );
  return string( data, sizeof( T ));
}

TEST_F( BinarySceneOStreamTest, IntWrite )
{
  int value = 123;
  string expected = toBinary( value );
  
  stream->write( value, "value" );
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, SizeTWrite )
{
  uint32_t value = 123456789;
  string expected = toBinary( value );
  
  stream->write( value, "size" );
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, StringWrite )
{
  string text="Hello, world!!!";
  uint32_t size = 15;
  string expected = toBinary( size ) + text;
  
  stream->write( text, "string" );
  
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, EnterSectionTest )
{
  string expected = "";
  
  stream->enterSection( "section" );
  
  EXPECT_EQ( stream->correct(), false );
  EXPECT_EQ( stream->errorLog(), "Section's end missing.\n" );
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, LeaveSectionTest )
{
  string expected = "";
  
  stream->leaveSection();
  
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, Correctness )
{
  stream->enterSection( "section" );
  stream->leaveSection();
  
  EXPECT_EQ( stream->correct(), true );
}

TEST_F( BinarySceneOStreamTest, FloatWrite )
{
  float value = 1.234;
  string expected( toBinary( value ));
  
  stream->write( value, "value" );
  
  EXPECT_EQ( stringStream->str(), expected );
}

TEST_F( BinarySceneOStreamTest, RawDataWriting )
{
  int data[] = { 1, 4, 16, 25 };
  string expected( toBinary( 1 ) + toBinary( 4 ) + toBinary( 16 ) + toBinary( 25 ) );

  bool result( stream->writeRawData( data, sizeof( int ) * 4 ) );

  ASSERT_TRUE( result );
  EXPECT_EQ( stringStream->str(), expected );
}

