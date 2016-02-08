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

  // GET MESSAGES HERE FROM HLA
  // init temporary variables
  InteractionRoot::SP interactionRootSP;

  while ( (interactionRootSP = getNextInteraction() ) != 0 )
    {
      boost::shared_ptr<GridlabDOutput> gldoSP( boost::static_pointer_cast<GridlabDOutput>( interactionRootSP ) );
      std::string recv_objectName = gldoSP->get_ObjectName();
      std::string recv_parameterName = gldoSP->get_Parameter();
      double recv_value = gldoSP->get_Value();
      std::string recv_units = gldoSP->get_Units();
      bool recv_setValue = gldoSP->get_SetValue();
      std::cout <<
	"GLDFederate: Received GridlabDOutput interaction: " << 
	recv_objectName << "/" << recv_parameterName << ": " << recv_value << recv_units << ": " << recv_setValue <<
	std::endl;

      if (!strcmp(recv_objectName.c_str(), "Market_1"))
	{
	  if (recv_value > 100)
	    {
	      value = 60;
	    }
	  else
	    {
	      value = 70;
	    }
	}

      // SEND DATA TO GLD; GET DATA FROM GLD
      std::cout <<
	"DemandController: Sending  interaction: " << 
	object << "/" << parameter << ": " << value << units << std::endl;

      GridlabDInputSP gldiSP = create_GridlabDInput();
      gldiSP->set_ObjectName( object );
      gldiSP->set_Parameter( parameter );
      gldiSP->set_Value( value );
      gldiSP->set_Units( units );
      gldiSP->set_SetValue( true );
      gldiSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );
    }

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
