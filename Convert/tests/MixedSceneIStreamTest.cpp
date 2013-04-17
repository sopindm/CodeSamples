#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <Convert/MixedSceneIStream.hpp>

using std::string;
using std::vector;
using std::stringstream;
using std::tr1::shared_ptr;
using namespace Eigen;
using namespace boost::assign;
using namespace rtrt;
using namespace rtrt::convert;

class MixedSceneIStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< stringstream > baseStream,
                             binaryStream;
			     
  shared_ptr< MixedSceneIStream > stream;
};

void MixedSceneIStreamTest::SetUp()
{
  baseStream.reset( new stringstream() );
  baseStream->unsetf(std::ios::skipws);
  binaryStream.reset( new stringstream() );
  
  stream.reset( new MixedSceneIStream( baseStream, binaryStream ));
}

template< class T > 
string toBinary( const T& value )
{
  const char* data = reinterpret_cast< const char* >( &value );
  return string( data, sizeof( T ));
}

template< class T >
string toBinary( const vector< T >& array )
{
  string ret( "" );
  
  for( size_t i=0; i<array.size(); i++ )
    ret += toBinary( array[ i ]);
	   
  return ret;
}

TEST_F( MixedSceneIStreamTest, RawDataReading )
{
  baseStream->str( "<xml>\n"\
                   "  <array id=0/>\n"\
                   "</xml>" );

  uint32_t nodes( 1 );
  uint32_t offset( 0 );
  
  vector< int > array;
  array += 0, 1, 3, 7, 12, 15, 27;

  binaryStream->str( toBinary( nodes ) +
                     toBinary( offset ) +
		     toBinary( array ) );
  stream->init();
	
  stream->enterSection( "array" );

  vector< int > result( array.size() );
  bool isRead = stream->readRawData( result.data(), sizeof( int ) * array.size() );

  ASSERT_TRUE( isRead );

  EXPECT_FLOAT_EQ( result[ 0 ], 0 );
  EXPECT_FLOAT_EQ( result[ 1 ], 1 );
  EXPECT_FLOAT_EQ( result[ 2 ], 3 );
  EXPECT_FLOAT_EQ( result[ 3 ], 7 );
  EXPECT_FLOAT_EQ( result[ 4 ], 12 );
  EXPECT_FLOAT_EQ( result[ 5 ], 15 );
  EXPECT_FLOAT_EQ( result[ 6 ], 27 );
}


