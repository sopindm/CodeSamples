/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O P E R T Y   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
                             
   Copyright (c) 2010 - 2012  Dmitry Sopin ( sopindm @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

namespace rtrt
{
  namespace property
  {
    template< class Value, class Constructor >
    SimpleConstructorProperty< Value, Constructor >::SimpleConstructorProperty(
      const std::string& name, Value& value, Constructor constructor ):
      ConstructorProperty( name ),
      _value( value ),
      _constructor( constructor )
    {
      _constructor.setup( _value );
    }

    template< class V, class C >
    ConstructorPropertyPtr SimpleConstructorProperty< V, C >::create( const std::string& name,
								      V& value,
								      C constructor )
    {
      boost::shared_ptr< SimpleConstructorProperty< V, C > > ret( 
	new SimpleConstructorProperty( name, value, constructor ));

      ret->setSelfLink( ret );

      return ret;
    }

    template< class V, class Constructor >
    void SimpleConstructorProperty< V, Constructor >::construct()
    {
      _constructor.construct( _value );
    }

    class ConstructorProperty::ConstructorUpdateHandler: public UpdateHandler
    {
    public:
      ConstructorUpdateHandler( const ConstructorPropertyPtr& constructor ):
	_constructor( constructor )
      {
      }

      void update()
      {
	_constructor->construct();
      }

    private:
      ConstructorPropertyPtr _constructor;

    };

    template< class V, class Constructor >
    PropertyPtr SimpleConstructorProperty< V, Constructor >::constructor()
    {

      PropertyPtr ret( _constructor.propertizeConstructor() );
      ret->setParent( *this );

      ret->connect( UpdateHandlerPtr( new ConstructorUpdateHandler( 
					boost::dynamic_pointer_cast< ConstructorProperty >( link() ) )));
      return ret;
    }

    template< class V, class Constructor >
    ListPropertyPtr SimpleConstructorProperty< V, Constructor >::constructed()
    {
      ListPropertyPtr ret( _constructor.propertize( _value ));
      ret->setParent( *this );

      return ret;
    }
  }
}
