#ifndef RTRT_PROPERTY_TESTS_SIMPLEUPDATEHANDLER_HPP
#define RTRT_PROPERTY_TESTS_SIMPLEUPDATEHANDLER_HPP

#include <Property/Property.hpp>

class SimpleUpdateHandler: public rtrt::property::UpdateHandler
{
public:
  SimpleUpdateHandler(): _isUpdated( false )
  {
  }
  
  void update() 
  { 
    _isUpdated = true; 
  };

  bool isUpdated() const { return _isUpdated; };

private:
  bool _isUpdated;
  
};
typedef boost::shared_ptr< SimpleUpdateHandler > SimpleUpdateHandlerPtr;

#endif
