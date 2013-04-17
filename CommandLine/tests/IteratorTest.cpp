#include <gtest/gtest.h>
#include <CommandLine/Iterator.hpp>

using namespace rtrt::commandLine;

TEST( IteratorTest, EmptryLine )
{
  const char* argv[] = {};
  
  Iterator iter( 0, argv );
  
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Next )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  ASSERT_FALSE( iter.atEnd() );
  
  iter++;
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Prev )
{
  const char* argv[] = { "aaa", "bbb" };
  
  Iterator iter( 2, argv, 1 );
  EXPECT_EQ( *iter, "bbb" );
  iter--;
  EXPECT_EQ( *iter, "aaa" );
}

TEST( IteratorTest, PrefixNext )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  ASSERT_FALSE( iter.atEnd() );
  
  ++iter;
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Access )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  
  EXPECT_EQ( *iter, "aaa" );
}

TEST( IteratorTest, FailedAccess )
{
  const char* argv[] = {};
  
  Iterator iter( 0, argv );
  EXPECT_EXIT( *iter, testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST( IteratorTest, AtFlag )
{
  const char* argv[] = { "--html" };
  
  Iterator iter( 1, argv );
  
  EXPECT_TRUE( iter.atArgument() );
}

TEST( IteratorTest, AtShortFlag )
{
  const char* argv[] = { "-a" };
  
  Iterator iter( 1, argv );
  
  EXPECT_TRUE( iter.atArgument() );
}