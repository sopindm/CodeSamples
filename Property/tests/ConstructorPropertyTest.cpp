#include <gtest/gtest.h>
#include <Property/AtomProperty.hpp>
#include <Property/ConstructorProperty.hpp>
#include <Property/PropertyBuilder.hpp>
#include "SimpleUpdateHandler.hpp"

using std::string;
using namespace rtrt::property;

class MyConstructor
{
public:
  MyConstructor(): _length( 0 ) {}

  void setup( const string& value ) { _length = value.length(); };
  void construct( string& value ) { value.resize( _length ); };

  PropertyPtr propertizeConstructor()
  {
    return buildProperty( "length", _length );
  }

  static ListPropertyPtr propertize( string& value )
  {
    return buildListProperty( "", buildProperty( "string", value ) );
  }

private:
  int _length;

};

class ConstructorPropertyTest : public testing::Test
{
};

TEST_F( ConstructorPropertyTest, Initialization )
{
  string value( "abc" );

  ConstructorPropertyPtr property( SimpleConstructorProperty< string, MyConstructor >::create("property", value ));

  EXPECT_EQ( property->link(), property );

  EXPECT_EQ( property->name(), "property" );

  IntPropertyPtr constructor = Property::cast< IntProperty >( property->constructor() );
  EXPECT_EQ( constructor->name(), "length" );
  EXPECT_EQ( constructor->get(), 3 );

  ASSERT_EQ( property->constructed()->size(), 1 );

  StringPropertyPtr constructed = Property::cast< StringProperty >( property->constructed()->at( 0 ) );
  EXPECT_EQ( constructed->name(), "string" );
  EXPECT_EQ( constructed->get(), "abc" );
}

TEST_F( ConstructorPropertyTest, Constructing )
{
  string value ( "abcd" );

  ConstructorPropertyPtr property( SimpleConstructorProperty< string, MyConstructor >::create( "property", value ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler());
  property->connect( handler );

  IntPropertyPtr constructor = Property::cast< IntProperty >( property->constructor() );

  constructor->set( 2 );

  EXPECT_TRUE( handler->isUpdated() );

  ASSERT_EQ( property->constructed()->size(), 1 );

  StringPropertyPtr constructed = Property::cast< StringProperty >( property->constructed()->at( 0 ) );

  EXPECT_EQ( value, "ab" );
  EXPECT_EQ( constructed->get(), "ab" );
}

TEST_F( ConstructorPropertyTest, UpdatingConstructed )
{
  string value ( "abcd" );

  ConstructorPropertyPtr property( SimpleConstructorProperty< string, MyConstructor >::create( "property", value ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  property->connect( handler );

  ASSERT_EQ( property->constructed()->size(), 1 );

  StringPropertyPtr constructed = Property::cast< StringProperty >( property->constructed()->at( 0 ) );
  constructed->set( "abc" );

  EXPECT_TRUE( handler->isUpdated() );
}


