#include <openssl/sha.h>
#include <boost/assign/std/vector.hpp>
#include <gtest/gtest.h>
#include <Convert/MixedSceneOStream.hpp>

using std::string;
using std::vector;
using std::stringstream;
using std::tr1::shared_ptr;
using namespace Eigen;
using namespace boost::assign;

using namespace rtrt;
using namespace rtrt::convert;

class MixedSceneOStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< stringstream > baseStream,
                             binaryStream;
			     
  shared_ptr< SceneOStream > stream;
};

void MixedSceneOStreamTest::SetUp()
{
  baseStream.reset( new stringstream() );
  binaryStream.reset( new stringstream() );
  
  stream.reset( new MixedSceneOStream( baseStream, binaryStream ));
}

template< class T > 
string toBinary( const T& value )
{
  const char* data = reinterpret_cast< const char* >( &value );
  return string( data, sizeof( T ));
}

TEST_F( MixedSceneOStreamTest, EmptyFlush )
{
  uint32_t size( 0 );
  stream->flush();
  
  EXPECT_EQ( baseStream->str(), "<xml/>" );
  EXPECT_EQ( binaryStream->str(), toBinary( size ));
}

char toHex( unsigned char ch )
{
  if( ch < 10 )
    return '0' + ch;
  else 
    return 'a' + ch - 10;
}
string sha( string source )
{
  const unsigned char* data( reinterpret_cast< const unsigned char* >( source.data() ));
  
  unsigned char sha[ 20 ];
  SHA1( data, source.size(), sha );
  
  string ret;
  for( int i=0; i<20; i++ )
  {
    ret += toHex( sha[ i ]/16 );
    ret += toHex( sha[ i ]%16 );
  } 
  return ret;
}

TEST_F( MixedSceneOStreamTest, ArrayWrite )
{
  vector< int > array;
  array += 1, 2, 4, 7, 15;

  uint32_t size( 1 );
  uint32_t offset( 0 );
  
  string arrayExpected( toBinary( 1 ) + toBinary( 2 ) + toBinary( 4 ) + toBinary( 7 ) + toBinary( 15 ) );
			
  string binaryExpected( toBinary( size ) +
			 toBinary( offset ) +
			 arrayExpected );
  
  string baseExpected( "<xml>\n"\
                       "  <array id=0 sha=\"" + sha( arrayExpected ) + "\"/>\n"\
                       "</xml>" );
  
  stream->enterSection( "array" );
  bool isWriten = stream->writeRawData( array.data(), array.size() * sizeof( int ) );
  stream->leaveSection();

  stream->flush();


  ASSERT_TRUE( isWriten );
  
  EXPECT_EQ( baseStream->str(), baseExpected );
  EXPECT_EQ( binaryStream->str(), binaryExpected );
}

/*
TEST_F( MixedSceneOStreamTest, Array2fWrite )
{
  Array2f array;
  array += Vector2f( 1.0, 2.0 );
  uint32_t size( 1 );
  uint32_t offset( 0 );
  
  string arrayExpected( toBinary( static_cast< uint32_t >( array.size() )) +
                        toBinary( array[ 0 ].x() ) +
			toBinary( array[ 0 ].y() ));
			
  string binaryExpected( toBinary( size ) +
			 toBinary( offset ) +
			 arrayExpected );
  
  string baseExpected( "<xml>\n"\
                       "  <array id=0 sha=\"" + sha( arrayExpected ) + "\"/>\n"\
                       "</xml>" );
   
  stream->write( array, "array" );
  stream->flush();
  
  EXPECT_EQ( baseStream->str(), baseExpected );
  EXPECT_EQ( binaryStream->str(), binaryExpected );
}

TEST_F( MixedSceneOStreamTest, Array3fWrite )
{
  Array3f array;
  array += Vector3f( 1.0, 2.0, 3.0 ),
    Vector3f( 5.0, 6.0, 7.0 );
	
  uint32_t size( 1 );
  uint32_t offset( 0 );
  string binaryExpected( toBinary( size ) + 
                         toBinary( offset ) );
  
  string arrayExpected = toBinary( static_cast< uint32_t >( array.size() ));
  for( int i=0; i<2; i++ )
    arrayExpected += toBinary( array[ i ].x() ) +
      toBinary( array[ i ].y() ) +
      toBinary( array[ i ].z() );

  binaryExpected += arrayExpected;
  
  string baseExpected( "<xml>\n"\
                       "  <array id=0 sha=\"" + sha( arrayExpected ) +"\"/>\n"\
                       "</xml>" );
   
  stream->write( array, "array" );
  stream->flush();
  
  EXPECT_EQ( baseStream->str(), baseExpected );
  EXPECT_EQ( binaryStream->str(), binaryExpected );
}

TEST_F( MixedSceneOStreamTest, Array4fWrite )
{
  Array4f array;
  array += Vector4f( 1.0, 2.0, 3.0, 4. ),
    Vector4f( 5.0, 6.0, 7.0, 8. );
	
  uint32_t size( 1 );
  uint32_t offset( 0 );
  string binaryExpected( toBinary( size ) + 
                         toBinary( offset ) );
  
  string arrayExpected = toBinary( static_cast< uint32_t >( array.size() ));
  for( int i=0; i<2; i++ )
    arrayExpected += toBinary( array[ i ].x() ) +
      toBinary( array[ i ].y() ) +
      toBinary( array[ i ].z() ) +
      toBinary( array[ i ].w() );

  binaryExpected += arrayExpected;
  
  string baseExpected( "<xml>\n"\
                       "  <array id=0 sha=\"" + sha( arrayExpected ) +"\"/>\n"\
                       "</xml>" );
   
  stream->write( array, "array" );
  stream->flush();
  
  EXPECT_EQ( baseStream->str(), baseExpected );
  EXPECT_EQ( binaryStream->str(), binaryExpected );
}

TEST_F( MixedSceneOStreamTest, Array3iWrite )
{
  Array3i array;
  array += Vector3i( 1, 2, 3 );
  uint32_t size( 1 );
  uint32_t offset( 0 );
  
  string arrayExpected(	 toBinary( static_cast< uint32_t >( array.size() )) +
			 toBinary( array[ 0 ].x() ) +
			 toBinary( array[ 0 ].y() ) +
			 toBinary( array[ 0 ].z() ) );
	  
  string binaryExpected( toBinary( size ) +
			 toBinary( offset ) +
			 arrayExpected );
  
  string baseExpected( "<xml>\n"\
                       "  <array id=0 sha=\"" + sha( arrayExpected ) + "\"/>\n"\
                       "</xml>" );
    
  stream->write( array, "array" );
  stream->flush();
  
  EXPECT_EQ( baseStream->str(), baseExpected );
  EXPECT_EQ( binaryStream->str(), binaryExpected );
}
*/
