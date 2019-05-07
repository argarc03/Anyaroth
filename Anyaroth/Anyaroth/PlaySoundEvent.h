#pragma once
#include "Event.h"
#include "Game.h"

class PlaySoundEvent : public Event
{
private:
	Game* _game;
	string _sound;

public:
	PlaySoundEvent(Game* game, string sound) : _game(game), _sound(sound) {}

	virtual void update(const double& deltaTime) {}
	virtual void play();
};