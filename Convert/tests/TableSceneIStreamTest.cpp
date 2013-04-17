#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <Convert/TableSceneIStream.hpp>

using std::string;
using std::stringstream;
using std::tr1::shared_ptr;
using boost::lexical_cast;
using namespace rtrt;
using namespace rtrt::convert;

class TableSceneIStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< stringstream > stream;
  TableSceneIStreamPtr sceneStream;
  
  template< class T >
  static string toBinary( T* data );
};

void TableSceneIStreamTest::SetUp()
{
  stream.reset( new stringstream() );
  sceneStream.reset( new TableSceneIStream( stream ));
}

template< class T >
string TableSceneIStreamTest::toBinary( T* data )
{
  char* value = reinterpret_cast< char* >( data );
  return string( value, sizeof( T ));
}

TEST_F( TableSceneIStreamTest, SimpleSections )
{
  uint32_t nodes( 1 );
  uint32_t offset( 0 );
  
  float expected( 123.4 );
  stream->str( toBinary( &nodes ) + 
               toBinary( &offset ) + 
	       toBinary( &expected ));
  sceneStream->initOffsets();
  
  sceneStream->beginNode( 0 );
  float value;
  sceneStream->baseStream()->read( &value, "value" );
  
  EXPECT_EQ( value, expected );
}

TEST_F( TableSceneIStreamTest, MultipleSections )
{
  uint32_t nodes( 3 );
  uint32_t offsets[] = { 0, sizeof( float ), sizeof( float ) + sizeof( int ) };
  
  float expected1 = 123.4;
  int expected2 = 123;
  uint32_t expected3 = 123456789;
  
  stream->str( toBinary( &nodes ) +
               toBinary( &offsets[ 0 ] ) +
               toBinary( &offsets[ 1 ] ) +
               toBinary( &offsets[ 2 ] ) +
	       toBinary( &expected1 ) +
	       toBinary( &expected2 ) +
	       toBinary( &expected3 ) );
  sceneStream->initOffsets();
  
  sceneStream->beginNode( 0 );
  float value1;
  sceneStream->baseStream()->read( &value1, "value" );
  
  sceneStream->beginNode( 2 );
  uint32_t value3;
  sceneStream->baseStream()->read( &value3, "size" );
  
  sceneStream->beginNode( 1 );
  int value2;
  sceneStream->baseStream()->read( &value2, "value" );
  
  EXPECT_EQ( value1, expected1 );
  EXPECT_EQ( value2, expected2 );
  EXPECT_EQ( value3, expected3 );
}
