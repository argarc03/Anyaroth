#pragma once
#include "Collectable.h"



class Coin : public Collectable
{
public:
	Coin(Game* g, Texture* texture, Vector2D iniPos, int value);
	~Coin() {}
};