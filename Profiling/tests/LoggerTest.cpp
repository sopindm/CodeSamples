#include <sstream>
#include <gtest/gtest.h>
#include <Profiling/Logger.hpp>
#include <Profiling/SimpleListener.hpp>
#include <Profiling/Timestamp.hpp>

using std::stringstream;
using std::string;
using std::vector;
using namespace rtrt;
using namespace rtrt::logging;

class LoggerTest : public testing::Test
{
public:
  void SetUp();
  
  Logger logger;
};

void LoggerTest::SetUp()
{
}

TEST_F( LoggerTest, CStringLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "a test message" );

  ASSERT_EQ( listener.logs().size(), 1 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "a test message" );
  EXPECT_EQ( listener.log( 0 ).values().size(), 0 );
}

TEST_F( LoggerTest, StringLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", string( "an other message" ) );

  ASSERT_EQ( listener.logs().size(), 1 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "an other message" );
}

TEST_F( LoggerTest, IntLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "int test", 123 );

  ASSERT_EQ( listener.logs().size(), 1 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "int test" );

  EXPECT_EQ( listener.log( 0 ).values().size(), 1 );

  const int32_t* value = boost::get< const int32_t >( &(listener.log( 0 ).values()[ 0 ]) );
  ASSERT_TRUE( value != NULL );

  EXPECT_EQ( *value, 123 );
}

TEST_F( LoggerTest, UIntLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "unsigned int test", static_cast< uint32_t >( 123456 ) );

  ASSERT_EQ( listener.logs().size(), 1 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "unsigned int test" );

  const uint32_t* value = boost::get< const uint32_t >( &(listener.log( 0 ).values()[ 0 ] ) );
  ASSERT_TRUE( value != NULL );

  EXPECT_EQ( *value, 123456 );
}

TEST_F( LoggerTest, FloatLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "float log test", 1.234 );
  logger.log( "test", "another float test", 5.678F );

  ASSERT_EQ( listener.logs().size(), 2 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "float log test" );

  const float* value = boost::get< float >( &listener.log( 0 ).values()[ 0 ] );
  ASSERT_TRUE( value != NULL );

  EXPECT_FLOAT_EQ( *value, 1.234 );

  EXPECT_EQ( listener.log( 1 ).stream(), "test" );
  EXPECT_EQ( listener.log( 1 ).message(), "another float test" );

  value = boost::get< float >( &listener.log( 1 ).values()[ 0 ] );
  ASSERT_TRUE( value != NULL );

  EXPECT_FLOAT_EQ( *value, 5.678 );
}

TEST_F( LoggerTest, BoolLogging )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "bool log test", true );
  logger.log( "test", "another bool test", false );

  ASSERT_EQ( listener.logs().size(), 2 );

  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "bool log test" );

  const bool* value = boost::get< bool >( &listener.log( 0 ).values()[ 0 ] );
  ASSERT_TRUE( value != NULL );

  EXPECT_TRUE( *value );

  EXPECT_EQ( listener.log( 1 ).stream(), "test" );
  EXPECT_EQ( listener.log( 1 ).message(), "another bool test" );

  value = boost::get< bool >( &listener.log( 1 ).values()[ 0 ] );
  ASSERT_TRUE( value != NULL );

  EXPECT_FALSE( *value );
}

TEST_F( LoggerTest, SeveralLogs )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "test", "string log 1" );
  logger.log( "test", "int log", 123 );
  logger.log( "test", "float log 1", 1.23 );
  logger.log( "test", "string log 2" );
  logger.log( "test", "float log 2", 4.56 );

  ASSERT_EQ( listener.logs().size(), 5 );
  
  EXPECT_EQ( listener.log( 0 ).stream(), "test" );
  EXPECT_EQ( listener.log( 0 ).message(), "string log 1" );

  EXPECT_EQ( listener.log( 1 ).stream(), "test" );
  EXPECT_EQ( listener.log( 1 ).message(), "int log" );

  EXPECT_EQ( listener.log( 2 ).stream(), "test" );
  EXPECT_EQ( listener.log( 2 ).message(), "float log 1" );

  EXPECT_EQ( listener.log( 3 ).stream(), "test" );
  EXPECT_EQ( listener.log( 3 ).message(), "string log 2" );

  EXPECT_EQ( listener.log( 4 ).stream(), "test" );
  EXPECT_EQ( listener.log( 4 ).message(), "float log 2" );
}

TEST_F( LoggerTest, SeveralListeners )
{
  SimpleListener l1, l2, l3;
  
  logger += l1;
  logger += l2;
  logger += l3;

  logger.log( "test", "string" );
  logger.log( "test", "int", 123 );

  ASSERT_EQ( l1.logs().size(), 2 );

  EXPECT_EQ( l1.log( 0 ).stream(), "test" );
  EXPECT_EQ( l1.log( 0 ).message(), "string" );

  ASSERT_EQ( l2.logs().size(), 2 );

  EXPECT_EQ( l2.log( 0 ).stream(), "test" );
  EXPECT_EQ( l2.log( 0 ).message(), "string" );

  ASSERT_EQ( l3.logs().size(), 2 );

  EXPECT_EQ( l3.log( 0 ).stream(), "test" );
  EXPECT_EQ( l3.log( 0 ).message(), "string" );

  EXPECT_EQ( l1.log( 1 ).stream(), "test" );
  EXPECT_EQ( l1.log( 1 ).message(), "int" );

  EXPECT_EQ( l2.log( 1 ).stream(), "test" );
  EXPECT_EQ( l2.log( 1 ).message(), "int" );

  EXPECT_EQ( l3.log( 1 ).stream(), "test" );
  EXPECT_EQ( l3.log( 1 ).message(), "int" );
}

TEST_F( LoggerTest, RemovingListeners )
{
  SimpleListener l1, l2;

  logger += l1;
  logger += l2;

  logger.log( "test", "string" );

  logger -= l2;

  logger.log( "test", "int", 123 );

  logger -= l1;

  logger.log( "test", "bool", false );

  ASSERT_EQ( l1.logs().size(), 2 );

  EXPECT_EQ( l1.log( 0 ).stream(), "test" );
  EXPECT_EQ( l1.log( 0 ).message(), "string" );

  ASSERT_EQ( l2.logs().size(), 1 );

  EXPECT_EQ( l2.log( 0 ).stream(), "test" );
  EXPECT_EQ( l2.log( 0 ).message(), "string" );

  EXPECT_EQ( l1.log( 1 ).stream(), "test" );
  EXPECT_EQ( l1.log( 1 ).message(), "int" );
}

TEST_F( LoggerTest, LoggingTime )
{
  SimpleListener listener;

  logger += listener;

  logger.log( "time", "event" );

  ASSERT_EQ( listener.logs().size(), 1 );

  Timestamp stamp( listener.log( 0 ).timestamp() );

  EXPECT_TRUE( stamp <= Timestamp::current() );

  Timestamp next( Timestamp::current() );
  while( next == stamp )
    next = Timestamp::current();

  logger.log( "newTime", "newEvent" );

  ASSERT_EQ( listener.logs().size(), 2 );

  Timestamp newStamp( listener.log( 1 ).timestamp() );

  EXPECT_TRUE( newStamp > stamp );
}

TEST_F( LoggerTest, LoggingStringValues )
{
  SimpleListener listener;
  logger += listener;

  logger.log( "time", "event", "value" );

  ASSERT_EQ( listener.logs().size(), 1 );

  EXPECT_EQ( listener.log( 0 ).stream(), "time" );
  EXPECT_EQ( listener.log( 0 ).message(), "event" );

  ASSERT_EQ( listener.log( 0 ).values().size(), 1 );

  const string* value = boost::get< string >( &listener.log( 0 ).values()[ 0 ] );

  ASSERT_TRUE( value != NULL );
  EXPECT_EQ( *value, "value" );
}

