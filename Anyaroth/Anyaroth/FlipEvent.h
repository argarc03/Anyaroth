#pragma once
#include "Event.h"
#include "Player.h"

class FlipEvent : public Event
{
private:
	Player* _player = nullptr;

public:
	FlipEvent(Player* player) : _player(player) {}

	virtual void update(const double& deltaTime) {}
	virtual void play();
};