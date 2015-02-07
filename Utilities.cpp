#include "Utilities.h"

Timer::Timer()
{
    _iStartTicks = 0;
    _iPausedTicks = 0;
    _bPaused = false;
    _bStarted = false;    
}

void Timer::start()
{
    _bStarted = true;
    _bPaused = false;

    _iStartTicks = SDL_GetTicks();    
}

void Timer::stop()
{
    _bStarted = false;
    _bPaused = false;    
}

int Timer::get_ticks()
{
    if( _bStarted == true )
    {
        if( _bPaused == true )
        {
            return _iPausedTicks;
        }
        else
        {
			int tempTicks = SDL_GetTicks() - _iStartTicks;
            return SDL_GetTicks() - _iStartTicks;
        }    
    }
    return 0;    
}

void Timer::pause()
{
    if( ( _bStarted == true ) && ( _bPaused == false ) )
	{
        _bPaused = true;
        _iPausedTicks = SDL_GetTicks() - _iStartTicks;
    }
}

void Timer::unpause()
{
    if( _bPaused == true )
    {
        _bPaused = false;
        _iStartTicks = SDL_GetTicks() - _iPausedTicks;
        _iPausedTicks = 0;
    }
}

bool Timer::is_started()
{
    return _bStarted;    
}

bool Timer::is_paused()
{
    return _bPaused;    
}

// Fps Meter

FpsMeter::FpsMeter()
{
	fps = 0;
	fpsTimer.start();
}

void FpsMeter::Start()
{
	fpsTimer.start();
}

int FpsMeter::GetReading()
{
	return fps;
}

bool FpsMeter::UpdateFrame()
{
	frame++;
	if( fpsTimer.get_ticks() >= 1000 )
	{
		fps = frame;
		frame = 0;
		fpsTimer.start();
		return true;
	}
	return false;
}
