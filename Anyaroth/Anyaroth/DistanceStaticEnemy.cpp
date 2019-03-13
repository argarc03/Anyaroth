﻿#include "DistanceStaticEnemy.h"
#include "GameComponent.h"
#include "AnimatedSpriteComponent.h"
#include "Player.h"

DistanceStaticEnemy::DistanceStaticEnemy(Player* player, Game* g, PlayState* play, Texture* texture, Vector2D posIni, string tag, GunType type) : DistanceEnemy(player, g, play, texture, posIni, tag, type)
{
	_vision = 700;
	_attackRange = _vision; //No se puede poner mas pequeño que la velocidad
	_attackTime = 1300; //La animacion tarda unos 450
	_life = 50;

	_anim->addAnim(AnimatedSpriteComponent::EnemyIdle, 13, true);
	_anim->addAnim(AnimatedSpriteComponent::EnemyDie, 18, false);

	_anim->playAnim(AnimatedSpriteComponent::EnemyIdle);
	_body->addCricleShape(b2Vec2(0, _body->getH() + _body->getH() / 20), _body->getW() - _body->getW() / 20, ENEMIES, FLOOR);
}

void DistanceStaticEnemy::update(double time)
{
	if (!_dead && inCamera())
	{
		Enemy::update(time);
		BodyComponent* _playerBody = _player->getComponent<BodyComponent>();

		b2Vec2 enemyPos = _body->getBody()->GetPosition(), playerPos = _playerBody->getBody()->GetPosition();

		double x = playerPos.x * 8 - enemyPos.x * 8, y = playerPos.y * 8 - enemyPos.y * 8;

		if (x < _vision && x > -_vision && y < _vision && y > -_vision) //Jugador en el rango
		{
			RayCast();

			if (_armVision) //Si vemos al jugador
			{
				if (x > 0) //Derecha
					_anim->unFlip();
				else if (x < 0) //Izquierda
					_anim->flip();

				//_arm->shoot();
			}
		}
	}
}