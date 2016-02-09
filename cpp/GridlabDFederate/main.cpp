#include "main.hpp"

void GridlabDFederate::initialize( void )
{
  GridlabDFederateATRCallback gldfedATRCb( *this );
  putAdvanceTimeRequest(  _currentTime, gldfedATRCb );

  readyToPopulate();
  readyToRun();
}

void GridlabDFederate::execute( void )
{
  // update class variables
  _currentTime += 1;

  // init temporary variables
  InteractionRoot::SP interactionRootSP;

  char tmpBuf[1024];
  std::string gld_url_base = "http://" + 
    _gld_ip + ":" + 
    _gld_port + "/";
  gld_obj object;
  int intf_retval = 0;
  std::string gld_url, date, time;

  // STEP GLD
  date = "2000-01-01 ";
  sprintf( tmpBuf,
	   "%02d:%02d:%02d",
	   (int)( ( _currentTime / 60 ) / 60 ),
	   (int)( _currentTime / 60 ),
	   (int)fmod(_currentTime, 60));
  time = std::string(tmpBuf);
  gld_url = gld_url_base + "control/pauseat=" + date + time;
  intf_retval = call_gld(gld_url, object);
  
  // GET MESSAGES HERE FROM HLA
  while ( (interactionRootSP = getNextInteraction() ) != 0 )
    {
      boost::shared_ptr<GridlabDInput> gldSP( boost::static_pointer_cast<GridlabDInput>( interactionRootSP ) );
      std::string objectName = gldSP->get_ObjectName();
      std::string parameterName = gldSP->get_Parameter();
      double value = gldSP->get_Value();
      std::string units = gldSP->get_Units();
      int operation = gldSP->get_Operation();
      
      // operation:
      //  0 : GET
      //  1 : SET
      //  ... : UNDEFINED

      std::cout <<
	"GLDFederate: Received GridlabDInput interaction: " << 
	objectName << "/" << parameterName << ": " << value << units << ": " << operation <<
	std::endl;

      // SEND DATA TO GLD; GET DATA FROM GLD
      memset(tmpBuf, 0, 1024);
      sprintf(tmpBuf, "%f", value);
      gld_url = gld_url_base + objectName + "/" + parameterName;
      if (operation == 1)
	gld_url += "=" + std::string(tmpBuf) + units;
      intf_retval = call_gld(gld_url, object);
      if (intf_retval) // everything went well
	{
	  if (object.has_data)
	    {
	      GridlabDOutputSP output = create_GridlabDOutput();
	      output->set_ObjectName( object.object );
	      output->set_Parameter( object.name );
	      // need to split the value into value and units
	      char *pEnd = 0;
	      value = strtod(object.value.c_str(), &pEnd);
	      output->set_Value( value );
	      if (pEnd)
		{
		  output->set_Units( std::string(pEnd) ); // there is always a space between the value and units
		}
	      output->set_Operation( 0 );
	      std::cout <<
		"GLDFederate: Sending GridlabDOutput interaction: " << 
		output->get_ObjectName() << "/" << 
		output->get_Parameter() << ": " << 
		output->get_Value() << 
		output->get_Units() << ": " << 
		output->get_Operation() <<
		std::endl;
	      output->sendInteraction( getRTI(), _currentTime + getLookAhead() );
	    }
	}
    }

  // Advance Time
  GridlabDFederateATRCallback gldfedATRcb( *this );
  putAdvanceTimeRequest( _currentTime, gldfedATRcb );
}

bool GridlabDFederate::call_gld(std::string gld_url,
				gld_obj& ret_obj)   // in-out param
{
  ret_obj.has_data = false;
  ret_obj.type = "";
  ret_obj.object = "";
  ret_obj.name = "";
  ret_obj.value = "";

  // std::cout << "Connecting to: " << gld_url << std::endl;
  using namespace boost::network;
  http::client client;
  http::client::request request(gld_url);
  request << header("Connection", "close");
  http::client::response response = client.get(request);

  std::string text = body(response);

  if (text.length())
    {
      rapidxml::xml_document<> doc;
      doc.parse<0>((char *)text.c_str());

      rapidxml::xml_node<> *_type = doc.first_node("property");
      rapidxml::xml_node<> *_name, *_val, *_object;

      if (_type)
	{
	  _object = _type->first_node("object");
	  _name = _type->first_node("name");
	  _val = _type->first_node("value");

	  ret_obj.has_data = true;
	  ret_obj.type = "property";
	  ret_obj.object = _object->value();
	  ret_obj.name = _name->value();
	  ret_obj.value = _val->value();
	}
      else
	{
	  _type = doc.first_node("globalvar");
	  _name = _type->first_node("name");
	  _val = _type->first_node("value");

	  ret_obj.has_data = true;
	  ret_obj.type = "globalvar";
	  ret_obj.name = _name->value();
	  ret_obj.value = _val->value();
	}
    }
  return true;
}

int main(int argc, char** argv)
{
  pid_t pID = fork();
  if (pID == 0)
    {
      char *args[4];

      char process[] = "/usr/local/bin/gridlabd";
      char model[] = "/home/c2wt/Projects/c2wt/examples/GridlabDHelloWorld/models/gridlab-d/IEEE_13_Node_With_Houses.glm";
      char option[] = "--server";
      args[0] = process;
      args[1] = model;
      args[2] = option;
      args[3] = 0;

      execv(args[0], args);
    }

  std::cout << "Creating GridlabDFederate Object" << std::endl;
  GridlabDFederate gldfed( argc, argv );
  std::cout << "GridlabDFederate created" << std::endl;

  std::cout << "Initializing GridlabDFederate" << std::endl;
  gldfed.initialize();
  std::cout << "GridlabDFederate initialized" << std::endl;

  std::cout << "Running GridlabDFederate" << std::endl;
  gldfed.run();

  kill(0, SIGKILL);

  return 0;
}
