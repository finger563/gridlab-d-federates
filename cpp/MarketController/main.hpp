#ifndef MAIN_HPP
#define MAIN_HPP

#include "MarketControllerBase.hpp"

// Needed for communicating with GLD
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class MarketController : public MarketControllerBase
{
private:
  double _currentTime;
public:
  typedef MarketControllerBase Super;
  MarketController( int argc, char *argv[] ) 
    : Super( argc, argv ), _currentTime( 0 ) { }
  virtual ~ MarketController( void )
    throw (RTI::FederateInternalError) { }

  class MarketControllerATRCallback : public ATRCallback 
  {
  private:
    MarketController &_gldfed;
  public:
    MarketControllerATRCallback( MarketController &gldfed ) : _gldfed( gldfed ) { }
    virtual void execute( void ) { _gldfed.execute(); }
    virtual SP clone( void ) { return SP( new MarketControllerATRCallback( *this ) ); }
  };

  void initialize( void );
  void execute( void );
};

#endif
