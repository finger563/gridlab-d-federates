#include "main.hpp"

void DemandController::initialize( void )
{
  DemandControllerATRCallback gldfedATRCb( *this );
  putAdvanceTimeRequest(  _currentTime, gldfedATRCb );

  readyToPopulate();
  readyToRun();
}

void DemandController::execute( void )
{
  // update class variables
  _currentTime += 1;

  // init temporary variables
  std::string object, parameter, units;
  double value = 70;

  object = "house15";
  parameter = "heating_setpoint";
  units = "degF";

  std::cout <<
    "DemandController: Sending  interaction: " << 
    object << "/" << parameter << ": " << value << units << std::endl;

  GridlabDMessageSP gldSP = create_GridlabDMessage();
  gldSP->set_ObjectName( object );
  gldSP->set_Parameter( parameter );
  gldSP->set_Value( value );
  gldSP->set_Units( units );
  gldSP->set_SetValue( true );
  gldSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );

  // GET MESSAGES HERE FROM HLA

  // Advance Time
  DemandControllerATRCallback gldfedATRcb( *this );
  putAdvanceTimeRequest( _currentTime, gldfedATRcb );
}

int main(int argc, char** argv)
{
  std::cout << "Creating DemandController Object" << std::endl;
  DemandController gldfed( argc, argv );
  std::cout << "DemandController created" << std::endl;

  std::cout << "Initializing DemandController" << std::endl;
  gldfed.initialize();
  std::cout << "DemandController initialized" << std::endl;

  std::cout << "Running DemandController" << std::endl;
  gldfed.run();

  return 0;
}
