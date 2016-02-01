#ifndef MAIN_HPP
#define MAIN_HPP

#include "DemandControllerBase.hpp"

// Needed for communicating with GLD
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class DemandController : public DemandControllerBase
{
private:
  double _currentTime;
public:
  typedef DemandControllerBase Super;
  DemandController( int argc, char *argv[] ) 
    : Super( argc, argv ), _currentTime( 0 ) { }
  virtual ~ DemandController( void )
    throw (RTI::FederateInternalError) { }

  class DemandControllerATRCallback : public ATRCallback 
  {
  private:
    DemandController &_gldfed;
  public:
    DemandControllerATRCallback( DemandController &gldfed ) : _gldfed( gldfed ) { }
    virtual void execute( void ) { _gldfed.execute(); }
    virtual SP clone( void ) { return SP( new DemandControllerATRCallback( *this ) ); }
  };

  void initialize( void );
  void execute( void );
};

#endif
