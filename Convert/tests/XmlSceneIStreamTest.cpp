#include <gtest/gtest.h>
#include <Convert/XmlSceneIStream.hpp>

using std::string;
using std::tr1::shared_ptr;
using namespace rtrt;
using namespace rtrt::convert;

class XmlSceneIStreamTest : public testing::Test
{
public:
  void SetUp();
  
  shared_ptr< XmlSceneIStream > stream;
  XmlNodePtr node;
};

void XmlSceneIStreamTest::SetUp()
{
  node = XmlNode::create( "root" );
  stream.reset( new XmlSceneIStream( node ));
}

TEST_F( XmlSceneIStreamTest, SizeRead )
{
  size_t expected = 123456789;
  node->attr( "size" ) = expected;
  
  uint32_t size;
  stream->read( &size, "size" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( expected, size );
}

TEST_F( XmlSceneIStreamTest, IntRead )
{
  int expected = 123;
  node->attr( "value" ) = expected;
  
  int value;
  stream->read( &value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( value, expected );
}

TEST_F( XmlSceneIStreamTest, FloatRead )
{
  float expected = 123.5;
  node->attr( "value" ) = expected;
  
  float value;
  stream->read( &value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( value, expected );
}

TEST_F( XmlSceneIStreamTest, StringRead )
{
  string expected = "some string";
  node->attr( "value" ) = expected;
  
  string value;
  stream->read( &value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( value, expected );
}

TEST_F( XmlSceneIStreamTest, NoRawReading )
{
  int array[ 10 ];
  EXPECT_FALSE( stream->readRawData( array, 10 * sizeof( int ) ));
}

TEST_F( XmlSceneIStreamTest, SectionOuterReadTest )
{
  int expected = 42;
  
  XmlNodePtr newNode( XmlNode::create( "leaf" ));
  node->childs() += newNode;
  node->attr( "value" ) = expected;
  
  stream->enterSection( "leaf" );
  stream->leaveSection();
  
  int value;
  stream->read( &value, "value" );
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( value, expected );
}

TEST_F( XmlSceneIStreamTest, SectionInnerReadTest )
{
  int expected = 42;
  
  XmlNodePtr newNode( XmlNode::create( "leaf" ));
  newNode->attr( "value" ) = expected;
  node->childs() += newNode;
  
  stream->enterSection( "leaf" );
  
  int value;
  stream->read( &value, "value" );
  
  stream->leaveSection();
  
  EXPECT_TRUE( stream->correct() );
  EXPECT_EQ( value, expected );
}

TEST_F( XmlSceneIStreamTest, DoubleSectionTest )
{
  XmlNodePtr node1( XmlNode::create( "leaf" ));
  node1->attr( "value" ) = 123;
  
  XmlNodePtr node2( XmlNode::create( "leaf" ));
  node2->attr( "value" ) = 234;
  
  node->childs() += node1;
  node->childs() += node2;
  
  stream->enterSection( "leaf" );
  int value1;
  stream->read( &value1, "value" );
  stream->leaveSection();
  stream->enterSection( "leaf" );
  int value2;
  stream->read( &value2, "value" );
  stream->leaveSection();
  
  EXPECT_EQ( value1, 123 );
  EXPECT_EQ( value2, 234 );
}

TEST_F( XmlSceneIStreamTest, UnexistingSection )
{
  stream->enterSection( "leaf" );
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Current node has not enought childs with name leaf.\n" );
}

TEST_F( XmlSceneIStreamTest, UnclosedSection )
{
  node->childs() += XmlNode::create( "leaf" );
  
  stream->enterSection( "leaf" );
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Section's end missing.\n" );
}

TEST_F( XmlSceneIStreamTest, UnexistingRead )
{
  int value;
  stream->read( &value, "value" );
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Attribute value not set.\n" );
}

TEST_F( XmlSceneIStreamTest, UnexpectingLeave )
{
  stream->leaveSection();
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Unexpected section's end.\n" );
}

TEST_F( XmlSceneIStreamTest, TwoUnexistingSections )
{
  stream->leaveSection();
  stream->leaveSection();
  
  EXPECT_FALSE( stream->correct() );
  EXPECT_EQ( stream->errorLog(), "Unexpected section's end.\n"\
                                 "Unexpected section's end.\n" );
}
