#include <gtest/gtest.h>
#include <Convert/XmlSceneOStream.hpp>

using std::string;
using std::tr1::shared_ptr;
using namespace rtrt;
using namespace rtrt::convert;

class XmlSceneOStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< XmlSceneOStream > stream;
  XmlNodePtr node;
  
};

void XmlSceneOStreamTest::SetUp()
{
  node = XmlNode::create( "root" );
  stream.reset( new XmlSceneOStream( node ));
}

TEST_F( XmlSceneOStreamTest, SizeWrite )
{
  uint32_t size = 123456789;
  
  stream->write( size, "size" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( node->attr( "size" ), size );
}

TEST_F( XmlSceneOStreamTest, IntWrite )
{
  int value = 123;
  stream->write( value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_TRUE( node->attr( "value" ).isSet() );
  EXPECT_EQ( node->attr( "value" ), value );
}

TEST_F( XmlSceneOStreamTest, FloatWrite )
{
  float value = 132.5;
  stream->write( value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_TRUE( node->attr( "value" ).isSet() );
  EXPECT_EQ( node->attr( "value" ), value );
}

TEST_F( XmlSceneOStreamTest, StringWrite )
{
  string value( "some value" );
  stream->write( value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_TRUE( node->attr( "value" ).isSet() );
  EXPECT_EQ( node->attr( "value" ), value );
}

TEST_F( XmlSceneOStreamTest, NoRawWritting )
{
  int array[ 10 ];
  EXPECT_FALSE( stream->writeRawData( array, 10 * sizeof( int ) ));
}

TEST_F( XmlSceneOStreamTest, SectionTest )
{
  stream->enterSection( "leaf" );
  stream->leaveSection();
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( node->childs( "leaf" ).count(), 1 );
}

TEST_F( XmlSceneOStreamTest, SectionWriteTest )
{
  stream->enterSection( "leaf" );
  stream->leaveSection();
  
  int value = 42;
  stream->write( value, "value" );

  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( node->childs( "leaf" ).count(), 1 );
  EXPECT_TRUE( node->attr( "value" ).isSet() );
}

TEST_F( XmlSceneOStreamTest, SectionInnerWriteTest )
{
  stream->enterSection( "leaf" );
  int value = 42;
  stream->write( value, "value" );
  stream->leaveSection();
  
  ASSERT_EQ( stream->correct(), true );
  ASSERT_EQ( node->childs( "leaf" ).count(), 1 );
  NodeSet::iterator child( node->childs( "leaf" ).begin() );
  
  EXPECT_EQ( (*child)->attr( "value" ), value );
}

TEST_F( XmlSceneOStreamTest, UnclosedSection )
{
  stream->enterSection( "leaf" );
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Section's end missing.\n" );
}

TEST_F( XmlSceneOStreamTest, UnexistingSection )
{
  stream->leaveSection();
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Unexpected section's end.\n" );
}

TEST_F( XmlSceneOStreamTest, TwoUnexistingSections )
{
  stream->leaveSection();
  stream->leaveSection();
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Unexpected section's end.\n"\
                                 "Unexpected section's end.\n" );
}
