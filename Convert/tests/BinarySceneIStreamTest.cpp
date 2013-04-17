#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <Convert/BinarySceneIStream.hpp>

using std::string;
using std::vector;
using std::stringstream;
using std::tr1::shared_ptr;
using namespace boost::assign;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::convert;

class BinarySceneIStreamTest : public testing::Test
{
public:
  SceneIStreamPtr stream;
  shared_ptr< stringstream > stringStream;
  
protected:
  void SetUp();
};

void BinarySceneIStreamTest::SetUp()
{
  stringStream.reset( new stringstream );
  stream.reset( new BinarySceneIStream( stringStream ));
}

template< class T >
string toBinary( const T& value )
{
  const char* data( reinterpret_cast< const char* >( &value ));
  return string( data, sizeof( T ));
}

TEST_F( BinarySceneIStreamTest, IntRead )
{
  int expected = 123;
  stringStream->str( toBinary( expected ));
  
  int value( 0 );
  stream->read( &value, "value" );
  
  EXPECT_EQ( stream->correct(), true );
  EXPECT_EQ( value, expected );
}

TEST_F( BinarySceneIStreamTest, SizeTRead )
{
  uint32_t expected = 123456789;
  stringStream->str( toBinary( expected ));
  
  uint32_t value( 0 );
  stream->read( &value, "size" );
  
  EXPECT_EQ( stream->correct(), true );
  EXPECT_EQ( value, expected );
}

TEST_F( BinarySceneIStreamTest, FloatRead )
{
  float expected = 123.5;
  stringStream->str( toBinary( expected ));
  
  float value( 0 );
  stream->read( &value, "value" );
  
  EXPECT_EQ( stream->correct(), true );
  EXPECT_EQ( value, expected );
}

TEST_F( BinarySceneIStreamTest, StringRead )
{
  string expected( "6bA !!\"" );
  uint32_t expectedSize( 7 );
  stringStream->str( toBinary( expectedSize ) + expected );
  
  string value;
  stream->read( &value, "value" );
  
  EXPECT_EQ( stream->correct(), true );
  EXPECT_EQ( value, expected );
}

TEST_F( BinarySceneIStreamTest, TooBigSizeStringRead )
{
  size_t size( 10 );
  stringStream->str( toBinary( size ) + "aaa" );
  
  string value;
  stream->read( &value, "value" );
  
  EXPECT_EQ( stream->correct(), false );
  EXPECT_EQ( stream->errorLog(), "Base stream bad.\n" );
}

TEST_F( BinarySceneIStreamTest, RawDataReading )
{
  vector< int > expected( 5 );
  expected += 0, 4, 17, 23, 24;

  string streamString( "" );
  streamString += toBinary( 0 ) + toBinary( 4 ) + toBinary( 17 ) + toBinary( 23 ) + toBinary( 24 );

  stringStream->str( streamString );

  vector< int > result( 5 );
  bool isRead( stream->readRawData( result.data(), sizeof( int ) * 5 ) );

  ASSERT_TRUE( isRead );

  EXPECT_EQ( result[ 0 ], 0 );
  EXPECT_EQ( result[ 1 ], 4 );
  EXPECT_EQ( result[ 2 ], 17 );
  EXPECT_EQ( result[ 3 ], 23 );
  EXPECT_EQ( result[ 4 ], 24 );
}

