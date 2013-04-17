#include <sstream>
#include <gtest/gtest.h>
#include <Profiling/Timestamp.hpp>

using namespace rtrt;
using namespace rtrt::logging;

class TimestampTest : public testing::Test
{
public:
  void SetUp();
  
  Timestamp next( const Timestamp& stamp );
};

void TimestampTest::SetUp()
{
}

Timestamp TimestampTest::next( const Timestamp& stamp )
{
  Timestamp current( Timestamp::current() );

  while( current == stamp )
    current = Timestamp::current();

  return current;
}

TEST_F( TimestampTest, TimestampNext )
{
  Timestamp stamp1( Timestamp::current() );
  Timestamp stamp2( next( stamp1 ));

  EXPECT_FALSE( stamp1 < stamp1 );
  EXPECT_TRUE( stamp1 < stamp2 );
  EXPECT_FALSE( stamp2 < stamp1 );
  EXPECT_FALSE( stamp2 < stamp2 );
  
  EXPECT_TRUE( stamp1 <= stamp1 );
  EXPECT_TRUE( stamp1 <= stamp2 );
  EXPECT_FALSE( stamp2 <= stamp1 );
  EXPECT_TRUE( stamp2 <= stamp2 );

  EXPECT_TRUE( stamp1 == stamp1 );
  EXPECT_FALSE( stamp1 == stamp2 );
  EXPECT_TRUE( stamp2 == stamp2 );

  EXPECT_FALSE( stamp1 != stamp1 );
  EXPECT_TRUE( stamp1 != stamp2 );
  EXPECT_FALSE( stamp2 != stamp2 );

  EXPECT_TRUE( stamp1 >= stamp1 );
  EXPECT_FALSE( stamp1 >= stamp2 );
  EXPECT_TRUE( stamp2 >= stamp1 );
  EXPECT_TRUE( stamp2 >= stamp2 );

  EXPECT_FALSE( stamp1 > stamp1 );
  EXPECT_FALSE( stamp1 > stamp2 );
  EXPECT_TRUE( stamp2 >= stamp1 );
  EXPECT_FALSE( stamp2 > stamp2 );
}

TEST_F( TimestampTest, TimestampOutput )
{
  Timestamp stamp( 123456, 123456789 );

  std::stringstream stream;
  stream << stamp;

  EXPECT_EQ( stream.str(), "123456123456789" );
}

TEST_F( TimestampTest, TimestampOutputWithLeadingZeros )
{
  Timestamp stamp( 123456, 123456 );

  std::stringstream stream;
  stream << stamp;

  EXPECT_EQ( stream.str(), "123456000123456" );
}

