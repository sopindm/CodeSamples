#include <gtest/gtest.h>
#include <Scenegraph/Time.hpp>

using namespace rtrt::scenegraph;

class TimeTest : public testing::Test
{
public:

  static Time oldest() { return Time( Time::oldest ); };
  static Time low() { return Time( Time::lowest ); };
  static Time middle() { return Time( Time::low ); };
  static Time high() { return Time( Time::high ); };
  static Time newest() { return Time( Time::newest ); };

  void testSettingOldLess( Time oldTime, Time newTime );  
  void testSettingOldMore( Time oldTime, Time newTime );
  void testSettingOld( Time oldTime, Time newTime );
};

TEST_F( TimeTest, OrderTest )
{
  EXPECT_TRUE( oldest() < low() );
  EXPECT_TRUE( oldest() < middle() );
  EXPECT_TRUE( oldest() < high() );
  EXPECT_TRUE( oldest() < newest() );

  EXPECT_FALSE( oldest() > low() );
  EXPECT_FALSE( oldest() > middle() );
  EXPECT_FALSE( oldest() > high() );
  EXPECT_FALSE( oldest() > newest() );

  EXPECT_TRUE( low() > oldest() );
  EXPECT_TRUE( low() < middle() );
  EXPECT_TRUE( low() > high() );
  EXPECT_TRUE( low() < newest() );

  EXPECT_FALSE( low() < oldest() );
  EXPECT_FALSE( low() > middle() );
  EXPECT_FALSE( low() < high() );
  EXPECT_FALSE( low() > newest() );

  EXPECT_TRUE( middle() > oldest() );
  EXPECT_TRUE( middle() > low() );
  EXPECT_TRUE( middle() < high() );
  EXPECT_TRUE( middle() < newest() );

  EXPECT_FALSE( middle() < oldest() );
  EXPECT_FALSE( middle() < low() );
  EXPECT_FALSE( middle() > high() );
  EXPECT_FALSE( middle() > newest() );

  EXPECT_TRUE( high() > oldest() );
  EXPECT_TRUE( high() < low() );
  EXPECT_TRUE( high() > middle() );
  EXPECT_TRUE( high() < newest() );

  EXPECT_FALSE( high() < oldest() );
  EXPECT_FALSE( high() > low() );
  EXPECT_FALSE( high() < middle() );
  EXPECT_FALSE( high() > newest() );
}

TEST_F( TimeTest, IncrementTest )
{
  Time veryOld( oldest() );
  veryOld++;

  EXPECT_TRUE( veryOld == oldest() );

  Time high( Time::highest );
  high++;

  EXPECT_TRUE( high == low() );

  Time veryNew( newest() );
  veryNew++;

  EXPECT_TRUE( veryNew == newest() );
}

void TimeTest::testSettingOldLess( Time oldTime, Time newTime )
{
  EXPECT_TRUE( newTime > oldTime );
  newTime++;

  EXPECT_TRUE( newTime > oldTime );
}

void TimeTest::testSettingOldMore( Time oldTime, Time newTime )
{
  EXPECT_FALSE( newTime < oldTime );
  newTime++;

  EXPECT_FALSE( newTime < oldTime );
}

void TimeTest::testSettingOld( Time oldTime, Time newTime )
{
  testSettingOldLess( oldTime, newTime );
  testSettingOldMore( oldTime, newTime );
}

TEST_F( TimeTest, SettingOld )
{
  testSettingOld( low(), Time( Time::high - 1 ));
  testSettingOld( middle(), Time( Time::highest ) );
  testSettingOld( high(), Time( Time::low - 1 ));
}

