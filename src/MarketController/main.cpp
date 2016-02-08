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

  // GET MESSAGES HERE FROM HLA
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
	"MarketController: Received GridlabDOutput interaction: " << 
	recv_objectName << "/" << recv_parameterName << ": " << recv_value << recv_units << ": " << recv_setValue <<
	std::endl;

      if (!strcmp(recv_objectName.c_str(), "house15"))
	{
	  if (recv_value > 65)
	    {
	      value = 150;
	    }
	  else
	    {
	      value = 50;
	    }
	}

      // SEND DATA TO GLD; GET DATA FROM GLD
      std::cout <<
	"MarketController:  interaction: " << 
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
