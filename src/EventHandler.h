#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SDL3/SDL.h>

class EventHandler
{
public:
	EventHandler() = default;
	virtual void handleEvents(const SDL_Event& event) = 0;
};

#endif // EVENTHANDLER_H
