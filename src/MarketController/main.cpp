#include "main.hpp"

void MarketController::initialize( void )
{
  MarketControllerATRCallback gldfedATRCb( *this );
  putAdvanceTimeRequest(  _currentTime, gldfedATRCb );

  readyToPopulate();
  readyToRun();
}

void MarketController::execute( void )
{
  // update class variables
  _currentTime += 1;

  // init temporary variables
  std::string object, parameter, units;
  double value = 100;

  object = "Market_1";
  parameter = "fixed_price";
  units = "MW";

  std::cout <<
    "MarketController:  interaction: " << 
    object << "/" << parameter << ": " << value << units << std::endl;

  GridlabDInputSP gldSP = create_GridlabDInput();
  gldSP->set_ObjectName( object );
  gldSP->set_Parameter( parameter );
  gldSP->set_Value( value );
  gldSP->set_Units( units );
  gldSP->set_SetValue( true );
  gldSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );

  // GET MESSAGES HERE FROM HLA

  // Advance Time
  MarketControllerATRCallback gldfedATRcb( *this );
  putAdvanceTimeRequest( _currentTime, gldfedATRcb );
}

int main(int argc, char** argv)
{
  std::cout << "Creating MarketController Object" << std::endl;
  MarketController gldfed( argc, argv );
  std::cout << "MarketController created" << std::endl;

  std::cout << "Initializing MarketController" << std::endl;
  gldfed.initialize();
  std::cout << "MarketController initialized" << std::endl;

  std::cout << "Running MarketController" << std::endl;
  gldfed.run();

  return 0;
}
