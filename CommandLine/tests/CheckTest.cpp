#include <gtest/gtest.h>
#include <CommandLine/Check.hpp>

using namespace rtrt::commandLine;

TEST( CheckTest, TrueCheck )
{
  TrueCheck< int > check;
  
  EXPECT_TRUE( check.check( -100 ) );
  EXPECT_TRUE( check.check( 0 ) );
  EXPECT_TRUE( check.check( 100 ) );
}

TEST( CheckTest, TrueCheckClone )
{
  CheckPtr< int >::Type check( TrueCheck< int >().clone() );
  
  EXPECT_TRUE( check->check( -100 ) );
  EXPECT_TRUE( check->check( 0 ) );
  EXPECT_TRUE( check->check( 100 ) );
}

TEST( CheckTest, FalseCheck )
{
  FalseCheck< int > check;
  
  EXPECT_FALSE( check.check( -100 ) );
  EXPECT_FALSE( check.check( 0 ) );
  EXPECT_FALSE( check.check( 100 ) );
}

TEST( ChechTest, FalseCheckClone )
{
  CheckPtr< int >::Type check( FalseCheck< int >().clone() );
  
  EXPECT_FALSE( check->check( -100 ) );
  EXPECT_FALSE( check->check( 0 ) );
  EXPECT_FALSE( check->check( 100 ) );
}

TEST( CheckTest, AndCheckTest )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( AndCheck< int >( falseCheck, falseCheck ).check( 42 ) );
  EXPECT_FALSE( AndCheck< int >( falseCheck, trueCheck ).check( 42 ) );
  EXPECT_FALSE( AndCheck< int >( trueCheck, falseCheck ).check( 42 ) );
  EXPECT_TRUE( AndCheck< int >( trueCheck, trueCheck ).check( 42 ) );
}

TEST( CheckTest, OrCheckTest )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( OrCheck< int >( falseCheck, falseCheck ).check( 42 ) );
  EXPECT_TRUE( OrCheck< int >( falseCheck, trueCheck ).check( 42 ) );
  EXPECT_TRUE( OrCheck< int >( trueCheck, falseCheck ).check( 42 ) );
  EXPECT_TRUE( OrCheck< int >( trueCheck, trueCheck ).check( 42 ) );
}

TEST( CheckTest, AndOperator )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( ( falseCheck && trueCheck ).check( 42 ) );
  EXPECT_TRUE( ( trueCheck && trueCheck ).check( 42 ) );
}

TEST( CheckTest, OrOperator )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( ( falseCheck || falseCheck ).check( 42 ) );
  EXPECT_TRUE( ( trueCheck || falseCheck ).check( 42 ) );
}

TEST( CheckTest, NotCheck )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( NotCheck< int >( trueCheck ).check( 42 ) );
  EXPECT_TRUE( NotCheck< int >( falseCheck ).check( 42 ) );
}

TEST( CheckTest, NotOperator )
{
  TrueCheck< int > trueCheck;
  FalseCheck< int > falseCheck;
  
  EXPECT_FALSE( (!trueCheck).check( 42 ) );
  EXPECT_TRUE( (!falseCheck).check( 42 ) );
}

TEST( CheckTest, Equal )
{
  EqualCheck< int > check( 42 );
  
  EXPECT_TRUE( check.check( 42 ) );
  EXPECT_FALSE( check.check( 24 ) );
}

TEST( CheckTest, NotEqual )
{
  NotEqualCheck< int > check( 42 );
  
  EXPECT_FALSE( check.check( 42 ) );
  EXPECT_TRUE( check.check( 24 ) );
}

TEST( CheckTest, Less )
{
  LessCheck< int > check( 42 );
  
  EXPECT_FALSE( check.check( 42 ) );
  EXPECT_TRUE( check.check( 24 ) );
  EXPECT_FALSE( check.check( 242 ) );
}

TEST( CheckTest, More )
{
  MoreCheck< int > check( 42 );
  
  EXPECT_FALSE( check.check( 42 ) );
  EXPECT_FALSE( check.check( 24 ) );
  EXPECT_TRUE( check.check( 242 ) );
}

TEST( CheckTest, LessEqual )
{
  LessEqualCheck< int > check( 42 );
  
  EXPECT_TRUE( check.check( 42 ) );
  EXPECT_TRUE( check.check( 24 ) );
  EXPECT_FALSE( check.check( 242 ) );
}

TEST( CheckTest, MoreEqual )
{
  MoreEqualCheck< int > check( 42 );
  
  EXPECT_TRUE( check.check( 42 ) );
  EXPECT_FALSE( check.check( 24 ) );
  EXPECT_TRUE( check.check( 242 ) );
}