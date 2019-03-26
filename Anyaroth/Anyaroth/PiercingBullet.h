#pragma once
#include "Bullet.h"
class PiercingBullet :
	public Bullet
{
private:
	bool _destroy = false; //true si colisiona con suelo
public:
	PiercingBullet(Game* game);
	virtual ~PiercingBullet();

	virtual void beginCollision(GameComponent* other, b2Contact* contact);

	virtual void update(double time);
};
