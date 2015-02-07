/***

Utilities: Objects that preform useful operations; yet are not specific enough for their own file

***/

#ifndef UTILITIES_H
#define UTILITIES_H

#include "SDL.h"

class Timer
{
private:
    int _iStartTicks;
    int _iPausedTicks;
    bool _bPaused;
    bool _bStarted; 
    
public:
    Timer();

	// Starts the Timer
    void start();

	// Stops the Timer
    void stop();

	// Pauses the Timer
    void pause();

	// Unpauses the Timer
    void unpause();
    
	// Postcondition: Returns the time elapsed in milliseconds
    int get_ticks();

    bool is_started();
    bool is_paused();    
};

// Fps Meter: Defines an object that will return the amount of 'ticks' per second
class FpsMeter
{
private:
	Timer fpsTimer;
	int frame;
	int fps;
public:
	FpsMeter();

	void Start();

	// Gets the most current reading
	int GetReading();

	// Returns true if the reading has changed
	bool UpdateFrame();
};

#endif // !UTILITIES_H