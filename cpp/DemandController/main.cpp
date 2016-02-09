#include "main.hpp"

double value = 70;
std::string object = "house15";
std::string parameter = "heating_setpoint";
std::string units = "degF";

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

  // SEND DATA TO GLD; GET DATA FROM GLD
  std::cout <<
    "DemandController: sending GridlabDInput interaction: " << 
    object << "/" << parameter << ": " << value << units << ": " << 1 << std::endl;

  GridlabDInputSP gldiSP = create_GridlabDInput();
  gldiSP->set_ObjectName( object );
  gldiSP->set_Parameter( parameter );
  gldiSP->set_Value( value );
  gldiSP->set_Units( units );
  gldiSP->set_Operation( 1 );
  gldiSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );

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
      int recv_operation = gldoSP->get_Operation();
      /*
      std::cout <<
	"DemandController: Received GridlabDOutput interaction: " << 
	recv_objectName << "/" << recv_parameterName << ": " << recv_value << recv_units << ": " << recv_operation <<
	std::endl;
      */
      if (!strcmp(recv_objectName.c_str(), "Market_1"))
	{
	  //std::cout << "Received current market price: " << recv_value << std::endl;
	  if (recv_value > 100 and value == 70)
	    {
	      std::cout << "Market price has increased too much, lowering demand." << std::endl;
	      value = 60;
	    }
	  else if (recv_value <= 100 and value == 60)
	    {
	      std::cout << "Market price has decreased enough, increasing demand." << std::endl;
	      value = 70;
	    }
	}
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
