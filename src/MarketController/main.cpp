#include "main.hpp"

double value = 100;
std::string object = "Market_1";
std::string parameter = "fixed_price";
std::string units = "MW";

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

  // GET MESSAGES HERE FROM HLA
  InteractionRoot::SP interactionRootSP;
  std::cout <<
    "MarketController: sending GridlabDInput interaction: " << 
    object << "/" << parameter << ": " << value << units << ": " << 1 << std::endl;

  GridlabDInputSP gldiSP = create_GridlabDInput();
  gldiSP->set_ObjectName( object );
  gldiSP->set_Parameter( parameter );
  gldiSP->set_Value( value );
  gldiSP->set_Units( units );
  gldiSP->set_Operation( 1 );
  gldiSP->sendInteraction( getRTI(), _currentTime + getLookAhead() );

  while ( (interactionRootSP = getNextInteraction() ) != 0 )
    {
      boost::shared_ptr<GridlabDOutput> gldoSP( boost::static_pointer_cast<GridlabDOutput>( interactionRootSP ) );
      std::string recv_objectName = gldoSP->get_ObjectName();
      std::string recv_parameterName = gldoSP->get_Parameter();
      double recv_value = gldoSP->get_Value();
      std::string recv_units = gldoSP->get_Units();
      int recv_operation = gldoSP->get_Operation();
      std::cout <<
	"MarketController: Received GridlabDOutput interaction: " << 
	recv_objectName << "/" << recv_parameterName << ": " << recv_value << recv_units << ": " << recv_operation <<
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
