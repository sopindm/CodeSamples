#include <gtest/gtest.h>
#include <boost/assign/std/vector.hpp>
#include <Property/AtomProperty.hpp>
#include <Property/ListProperty.hpp>
#include <Property/PropertyBuilder.hpp>
#include "SimpleUpdateHandler.hpp"

using std::vector;
using std::string;
using namespace rtrt::property;
using namespace boost::assign;

class ListPropertyTest : public testing::Test
{
};

TEST_F( ListPropertyTest, EmptyList )
{
  vector< PropertyPtr > props;
  ListPropertyPtr prop( ListProperty::create( "prop", props ));

  EXPECT_EQ( prop->size(), 0 );
  EXPECT_EQ( prop->name(), "prop" );

  EXPECT_EQ( prop->link(), prop );
}

TEST_F( ListPropertyTest, AccessTest )
{
  int intValue( 42 );
  string stringValue( "blabla" );
  bool boolValue( false );

  vector< PropertyPtr > props;
  props += buildProperty( "someProperty", intValue ),
    buildProperty( "otherOne", stringValue ),
    buildProperty( "andBoolean", boolValue );

  ListPropertyPtr lProp( ListProperty::create( "list", props ));

  ASSERT_EQ( lProp->size(), 3 );

  EXPECT_EQ( lProp->at( 0 )->name(), "someProperty" );
  EXPECT_EQ( lProp->at( 1 )->name(), "otherOne" );
  EXPECT_EQ( lProp->at( 2 )->name(), "andBoolean" );
}

TEST_F( ListPropertyTest, GettingPropertyByName )
{
  ListPropertyPtr property( buildListProperty( "list",
					       buildProperty( "anInt", 1 ),
					       buildProperty< string >( "aString", "abcdef" ),
					       buildProperty( "aFloat", 1.234F )));

  IntPropertyPtr intProperty( Property::cast< IntProperty >( property->at( "anInt" )) );
  EXPECT_EQ( intProperty->get(), 1 );
  
  StringPropertyPtr stringProperty( Property::cast< StringProperty >( property->at( "aString" )) );
  EXPECT_EQ( stringProperty->get(), "abcdef" );

  FloatPropertyPtr floatProperty( Property::cast< FloatProperty >( property->at( "aFloat" ) ));
  EXPECT_FLOAT_EQ( floatProperty->get(), 1.234 );
}

TEST_F( ListPropertyTest, UpdateTest )
{
  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );

  int iVal;
  float fVal;

  ListPropertyPtr lProp( buildListProperty( "list", 
					    buildProperty( "anInt", iVal ),
					    buildProperty( "aFloat", fVal ) ));

  lProp->connect( handler );
  
  FloatPropertyPtr fProp( Property::cast< FloatProperty >( lProp->at( "aFloat" ) ));
  fProp->set( 10 );

  EXPECT_TRUE( handler->isUpdated() );
}
