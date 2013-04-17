#include <gtest/gtest.h>
#include <CommandLine/Argument.hpp>

using std::vector;
using namespace rtrt::commandLine;

TEST( ValueTest, FailedCheck )
{
  Flag flag( "flag", FalseCheck< impl::EmptyType >() );
  
  const char* argv[] = {};
  Iterator iter( 0, argv );
  
  EXPECT_EXIT( flag.parse( iter ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST( ValueTest, VectorCheck )
{
  const char* argv[] = { "1", "2", "3", "4" };
  Iterator iter( 4, argv );
  
  Key< vector< int > > key( "key", LessCheck< int >( 5 ) );
  key.parse( iter );
}

TEST( ValueTest, VectorFailedCheck )
{
  const char* argv[] = { "1", "2", "3", "4", "5" };
  Iterator iter( 5, argv );
  
  Key< vector< int > > key( "key", LessCheck< int >( 5 ) );
  EXPECT_EXIT( key.parse( iter ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}