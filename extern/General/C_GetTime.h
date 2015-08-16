//---------------------------------------------------------------------------
/*-----------------------------------------------------------------------------
 Unit Name  : C_GetTime
 Author     : DWK
 Since Date : 2001-01-01
 UpDate     : 2007-05-21
              2008-03-19
              2008-06-27
              2009-02-21
              2009-05-08
 Purpose    : get performance time, limit 1us
 History    : ver 2.2
              2008-03-19 add timeStartTime()
              2008-06-27 modify
              2008-12-11 change C_GetTime(void) to C_GetTime( SCALE::TIME_SCALE unit = SCALE::TIME_1MS );
              2009-02-21 change timeDevEnd(), add processMessage();
              2009-04-17 change namespace SCALE to EX_SCALE.
              ver 2.3
              2009-04-21 change C_GetTime( SCALE::TIME_SCALE unit = SCALE::TIME_1MS ) to
                                C_GetTime( SCALE::TIME_SCALE unit = SCALE::TIME_1MS, const bool slice = false );
              2009-05-08 add waitTimeOutFuncClosure<>()
 Manual     : get performance time, limit 1us
-----------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef C_GetTimeH
#define C_GetTimeH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------

namespace EX_SCALE
{
  enum TIME_SCALE
  {
    TIME_1S    =       1,
    TIME_100MS =      10,
    TIME_10MS  =     100,
    TIME_1MS   =    1000,
    TIME_100US =   10000,
    TIME_10US  =  100000,
    TIME_1US   = 1000000,
  };
};
//---------------------------------------------------------------------------

class C_GetTime
{
  public:
          void setTimeUnit( EX_SCALE::TIME_SCALE unit );        // unit: 1S, 100ms, 10ms, 1ms, 100us, 10us, 1us
    const long getTimeUnit( void ) { return _TIME_UNIT; }

    void setTimeSlice( const bool src ) { _timeSlice = src; }   // true: free, false: hold
    bool getTimeSlice( void ) { return _timeSlice; }

    const unsigned __int64 time ( void );                       // time = counter / frequency
    void                   delay( const unsigned tm );

    void timeStart( const unsigned long tm );       // set time up time
    bool timeUp( void );                            // time up return 1
    void waitFor( void );                           // wait for time up
    const unsigned __int64 timeStartTime( void );   // get time up active time

    void                   timeDevStart( void ) { _timeDevStart = time(); }  // start deviation time
    const unsigned __int64 timeDevEnd  ( void );                             // get deviation time,  return = End Time - Start Time

    export template<typename TRet>
      int  waitTimeOutFuncClosure( bool chk, int timeout, TRet ( __closure *func )(void) );  // return 0:ok, -1:timeout

  public:
//     C_GetTime();
     C_GetTime( EX_SCALE::TIME_SCALE unit = EX_SCALE::TIME_1MS, const bool slice = false );
    ~C_GetTime();

  private:
    void processMessage( void );

  private:
    long _TIME_UNIT;
    bool _timeSlice;

    LARGE_INTEGER _counter;
    unsigned __int64 _frequency;

    unsigned long _timeRange;
    unsigned __int64 _timeGet;
    bool _timeUp;

    unsigned __int64 _timeDevStart;

};
//---------------------------------------------------------------------------

template<typename TRet>
int C_GetTime::waitTimeOutFuncClosure( bool chk, int timeout, TRet ( __closure *func )(void) )
{
  if ( func == NULL ) return -1;

  __int64 tmBack = time();
  do
  {
    if ( (bool)func() == chk ) return 0;
    processMessage();
  } while( (time()-tmBack) < timeout );
  return -1;
}
//---------------------------------------------------------------------------
#endif
