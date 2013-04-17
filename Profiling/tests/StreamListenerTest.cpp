#include <sstream>
#include <gtest/gtest.h>
#include <boost/lexical_cast.hpp>
#include <Profiling/Logger.hpp>
#include <Profiling/SimpleListener.hpp>
#include <Profiling/StreamListener.hpp>

using boost::lexical_cast;
using std::stringstream;
using std::string;
using boost::shared_ptr;
using namespace rtrt;
using namespace rtrt::logging;

class StreamListenerTest : public testing::Test
{
public:
  void SetUp();
  
  std::string message( const std::string& stream, const std::string& message, const Timestamp& time )
  {
    return string( "\"" ) + stream + "\" \"" + message + "\" " + lexical_cast< string >( time ) + '\n';
  }

  template< class T >
  std::string message( const std::string& stream, const std::string& message, T value, const Timestamp& time )
  {
    return string( "\"" ) + stream + "\" \"" 
      + message + "\" "
      + lexical_cast< string >( value ) + " "
      + lexical_cast< string >( time ) + "\n";
  }

  Logger logger;
  shared_ptr< StreamListener > listener;
  SimpleListener simpleListener;
  shared_ptr< stringstream > stream;
};

void StreamListenerTest::SetUp()
{
  stream.reset( new stringstream() );
  listener.reset( new StreamListener( stream ));

  logger += *listener;
  logger += simpleListener;
}

TEST_F( StreamListenerTest, StringLogging )
{
  logger.log( "test stream", "a test message" );

  ASSERT_EQ( simpleListener.logs().size(), 1 );

  EXPECT_EQ( stream->str(), message( "test stream",  "a test message", 
				     simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, StringLoggingWithQuotes )
{
  logger.log( "a test", "\"other message with \"" );

  EXPECT_EQ( stream->str(), message( "a test", "\\\"other message with \\\"", 
				     simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, LoggingStringWithSlash )
{
  logger.log( "a test", "a \\ message" );

  EXPECT_EQ( stream->str(), message( "a test", "a \\\\ message", simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, LoggingStringWithNewlines )
{
  stringstream tmpStream;
  tmpStream << "message with" << std::endl << " newlines.";

  logger.log( "a test", tmpStream.str() );

  EXPECT_EQ( stream->str(), message( "a test", "message with\\n newlines.", simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, IntLoggign )
{
  logger.log( "other test", "a message", 123 );

  EXPECT_EQ( stream->str(), message( "other test", "a message", 123, simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, UIntLoggign )
{
  logger.log( "other test", "a message", static_cast< uint32_t >( 123 ) );

  EXPECT_EQ( stream->str(), message( "other test", "a message", 123, simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, FloatLogging )
{
  logger.log( "other test", "a message", 1.25 );

  EXPECT_EQ( stream->str(), message( "other test", "a message", 1.25, simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, BoolLogging )
{
  logger.log( "bool", "message 1", false );
  logger.log( "bool", "message 2", true );

  EXPECT_EQ( stream->str(), 
	     message( "bool", "message 1", "false", simpleListener.log( 0 ).timestamp() ) +
	     message( "bool", "message 2", "true", simpleListener.log( 1 ).timestamp() ) );
}

TEST_F( StreamListenerTest, LoggingStringArgs )
{
  logger.log( "test", "message", "value" );

  EXPECT_EQ( stream->str(),
	     message( "test", "message", "\"value\"", simpleListener.log( 0 ).timestamp() ) );
}

TEST_F( StreamListenerTest, LoggingSeveralValues )
{
  logger.log( "test", "message" , "a string", 123 , "test" );

  EXPECT_EQ( stream->str(),
	     message( "test", "message", "\"a string\" 123 \"test\"", simpleListener.log( 0 ).timestamp() ) );
}

