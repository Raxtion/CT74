//---------------------------------------------------------------------------

#pragma hdrstop

#include "C_GetTime.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

C_GetTime::C_GetTime( EX_SCALE::TIME_SCALE unit, const bool slice )
:_timeUp( true )
{
  setTimeSlice( slice );
  setTimeUnit( unit );
}
//---------------------------------------------------------------------------

C_GetTime::~C_GetTime( void )
{
}
//---------------------------------------------------------------------------

void C_GetTime::setTimeUnit( EX_SCALE::TIME_SCALE unit )
{
  LARGE_INTEGER freq;
  QueryPerformanceFrequency( &freq );
  _frequency = freq.QuadPart / unit;

  _TIME_UNIT = unit/1000;
}
//---------------------------------------------------------------------------

const unsigned __int64 C_GetTime::time( void )
{
  QueryPerformanceCounter( &_counter );
  return ( (unsigned __int64)( _counter.QuadPart / _frequency ) );
}
//---------------------------------------------------------------------------

void C_GetTime::delay( const unsigned tm )
{
  unsigned __int64 temp;
  temp = time();
  do
  {
    if( _timeSlice == true ) processMessage();
  } while( ( time() - temp ) < tm );
}
//---------------------------------------------------------------------------

void C_GetTime::timeStart( const unsigned long tm )
{
  _timeGet = time();
  _timeRange = tm;
}
//---------------------------------------------------------------------------

bool C_GetTime::timeUp( void )
{
  if( _timeSlice == true ) processMessage();
  return ( ( time() - _timeGet ) >= _timeRange );
}
//---------------------------------------------------------------------------

void C_GetTime::waitFor( void )
{
  do
  {
    if( _timeSlice == true ) processMessage();
  } while( !timeUp() );
}
//---------------------------------------------------------------------------

const unsigned __int64 C_GetTime::timeStartTime( void )
{
  return ( time() - _timeGet );
}
//---------------------------------------------------------------------------

const unsigned __int64 C_GetTime::timeDevEnd( void )
{
  if( _timeSlice == true ) processMessage();
  return time()-_timeDevStart;
}
//---------------------------------------------------------------------------

void C_GetTime::processMessage( void )
{
  MSG msg;
  PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );

//  if ( msg.message != 0 ) GetMessage( &msg, NULL, 0, 0 );   // ·|¥d¦í
  DispatchMessage( &msg );
  TranslateMessage( &msg );

  Sleep( 0 );
}
//---------------------------------------------------------------------------

