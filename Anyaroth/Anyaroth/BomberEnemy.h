#pragma once
#include "Enemy.h"
#include "GameComponent.h"
#include "AnimatedSpriteComponent.h"
#include "HurtRenderComponent.h"
#include "Player.h"
#include "BomberGun.h"


class BomberEnemy : public Enemy
{
private:
	bool _spawning = false, _activated = false, _bloqueDer = false, _bloqueIzq = false, _move = false;
	double _spawnTime = 1000;
	int _range = 600, _dir = 8;

	BulletPool* _myBulletPool = nullptr;
	BomberGun* _gun = nullptr;
	//ExplosiveBulletEffect _effect = ExplosiveBulletEffect(100);
	Texture* _bulletTexture = nullptr;

public:
	BomberEnemy(Player* player, Game* g, PlayState* play, Texture* texture, Vector2D posIni, string tag, BulletPool* pool);
	virtual ~BomberEnemy();

	virtual void update(double time);

	virtual void subLife(int damage);

	virtual void beginCollision(GameComponent* other, b2Contact* contact);

	void throwBomb(const Vector2D& position, const double& angle, const string& tag);
};