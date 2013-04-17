#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <Convert/TableSceneOStream.hpp>

using std::string;
using std::stringstream;
using std::tr1::shared_ptr;
using boost::lexical_cast;
using namespace rtrt;
using namespace rtrt::convert;

class TableSceneOStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< stringstream > stream;
  TableSceneOStreamPtr sceneStream;
  
  template< class T >
  static string toBinary( T* data );
};

void TableSceneOStreamTest::SetUp()
{
  stream.reset( new stringstream() );
  sceneStream.reset( new TableSceneOStream( stream ));
}

template< class T >
string TableSceneOStreamTest::toBinary( T* data )
{
  char* value = reinterpret_cast< char* >( data );
  return string( value, sizeof( T ));
}

TEST_F( TableSceneOStreamTest, SectionTest )
{
  uint32_t nodeId = sceneStream->beginNode();
  sceneStream->endNode();
  
  uint32_t size( 1 );
  uint32_t offset( 0 );
  string expected( toBinary( &size ) +
                   toBinary( &offset ));
  
  sceneStream->flush();
  
  EXPECT_EQ( nodeId, 0 );
  EXPECT_EQ( stream->str(), expected );
}

TEST_F( TableSceneOStreamTest, UniqueId )
{
  uint32_t node1 = sceneStream->beginNode();
  string sha1 = sceneStream->endNode();
  
  uint32_t node2 = sceneStream->beginNode();
  string sha2 = sceneStream->endNode();
  
  EXPECT_NE( node1, node2 );
  EXPECT_EQ( sha1, sha2 );
}

TEST_F( TableSceneOStreamTest, Write )
{
  uint32_t node = sceneStream->beginNode();
  
  uint32_t size( 1 );
  uint32_t offset( 0 );
  string expected( toBinary( &size ) + 
                   toBinary( &offset ) );
  
  int value = 42;
  expected += toBinary( &value );
  
  sceneStream->baseStream()->write( value, "value" );
  sceneStream->endNode();
  sceneStream->flush();
  
  EXPECT_EQ( node, 0 );
  EXPECT_EQ( stream->str(), expected );
}

string printNode( uint32_t id, uint32_t offset )
{
  string ret( "<node id=" );
  ret += lexical_cast< string >( id );
  ret += " offset=";
  ret += lexical_cast< string >( offset );
  ret += "/>\n";
  
  return ret;
}

TEST_F( TableSceneOStreamTest, TwoNodes )
{
  uint32_t value( 42 );
  uint32_t size( 2 );
  uint32_t offsets[] = { uint32_t( 0 ), uint32_t( sizeof( uint32_t )) };
  
  string expected( toBinary( &size ) + 
                   toBinary( &offsets[ 0 ]) + 
		   toBinary( &offsets[ 1 ]) +
		   toBinary( &value ));
  
  sceneStream->beginNode();
  sceneStream->baseStream()->write( value, "size" );
  string sha1 = sceneStream->endNode();
  
  sceneStream->beginNode();
  string sha2 = sceneStream->endNode();
  sceneStream->flush();
  
  EXPECT_EQ( stream->str(), expected );
  EXPECT_NE( sha1, sha2 );
}
