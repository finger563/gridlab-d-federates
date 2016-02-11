#include "main.hpp"

double value = 50;
std::string object = "Market_1";
std::string parameter = "fixed_price";
std::string units = "MW";

bool setValue = true;

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

  int operation = (int)setValue;

  // GET MESSAGES HERE FROM HLA
  InteractionRoot::SP interactionRootSP;
  std::cout <<
    "MarketController: sending GridlabDInput interaction: " << 
    object << "/" << parameter << ": " << value << units << ": " << operation << std::endl;

  GridlabDInputSP gldiSP = create_GridlabDInput();
  gldiSP->set_ObjectName( object );
  gldiSP->set_Parameter( parameter );
  gldiSP->set_Value( value );
  gldiSP->set_Units( units );
  gldiSP->set_Operation( operation );
  gldiSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );

  while ( (interactionRootSP = getNextInteraction() ) != 0 )
    {
      boost::shared_ptr<GridlabDOutput> gldoSP( boost::static_pointer_cast<GridlabDOutput>( interactionRootSP ) );
      std::string recv_objectName = gldoSP->get_ObjectName();
      std::string recv_parameterName = gldoSP->get_Parameter();
      double recv_value = gldoSP->get_Value();
      std::string recv_units = gldoSP->get_Units();
      int recv_operation = gldoSP->get_Operation();

      if (!strcmp(recv_objectName.c_str(), "house15") and setValue)
	{
	  std::cout << "Received current demand: " << recv_value << std::endl;
	  if (recv_value > 65 and value == 50)
	    {
	      std::cout << "Demand has increased too much, increasing price." << std::endl;
	      value = 150;
	    }
	  else if (recv_value < 65 and value == 150)
	    {
	      std::cout << "Demand has decreased enough, lowering price." << std::endl;
	      value = 50;
	    }
	}
      else if (!strcmp(recv_objectName.c_str(), object.c_str()) and !setValue)
	{
	  std::cout << "Received current price: " << recv_value << std::endl;
	}
    }

  // Advance Time
  MarketControllerATRCallback gldfedATRcb( *this );
  putAdvanceTimeRequest( _currentTime, gldfedATRcb );
}

int main(int argc, char** argv)
{
  for (int i=0; i<argc; i++)
    {
      if (!strcmp(argv[i],"--monitor"))
	{
	  setValue = false;
	}
    }

  if (setValue)
    std::cout << "MarketController entering Controller mode." << std::endl;
  else
    std::cout << "MarketController entering Monitor mode." << std::endl;

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
