#include "MeleeEnemy.h"
#include "GameComponent.h"
#include "AnimatedSpriteComponent.h"
#include "Player.h"


MeleeEnemy::MeleeEnemy(Player* player, Game* g, PlayState* play,Texture* texture, Vector2D posIni, string tag) : Enemy(player, g, play,texture, posIni, tag)
{
	_vision = 300;
	_attackRange = 25; //No se puede poner mas peque�o que la velocidad
	_attackTime = 1300; //La animacion tarda unos 450
	_life = 50;


	_anim->addAnim(AnimatedSpriteComponent::EnemyIdle, 13, true);
	_anim->addAnim(AnimatedSpriteComponent::EnemyWalk, 8, true);
	_anim->addAnim(AnimatedSpriteComponent::EnemyAttack, 11, false);

	_anim->playAnim(AnimatedSpriteComponent::EnemyIdle);
}

void MeleeEnemy::update()
{
	Enemy::update();

	Vector2D enemy, player;

	enemy = _transform->getPosition();
	player = _playerTransform->getPosition();

	double x, y;

	x = player.getX() - enemy.getX();
	y = player.getY() - enemy.getY();

	if (!_attacking && x < _vision && x > -_vision)
	{
		if (x > 0)
		{
			_anim->unFlip();

			if (x > _attackRange)
			{
				_movement->changeDir(1, 0);
				_anim->playAnim(AnimatedSpriteComponent::EnemyWalk);
			}
			else
			{
				_movement->changeDir(0, 0);
				_anim->playAnim(AnimatedSpriteComponent::EnemyAttack); //Llamas a animacion de ataque
				_time = SDL_GetTicks();
				_attacking = true;
			}
		}
		else if (x < 0)
		{
			_anim->flip();

			if (x < -_attackRange)
			{
				_movement->changeDir(-1, 0);
				_anim->playAnim(AnimatedSpriteComponent::EnemyWalk);
			}
			else
			{
				_movement->changeDir(0, 0);
				_anim->playAnim(AnimatedSpriteComponent::EnemyAttack); //Llamas a animacion de ataque
				_time = SDL_GetTicks();
				_attacking = true;
			}
		}
	}
	else if(_attacking)
	{ 
		if (SDL_GetTicks() > _time + _attackTime)
		{
			_attacking = false;
			_anim->playAnim(AnimatedSpriteComponent::EnemyIdle);
			//if(colision de arma con jugador)
			//	llamas da�o al jugador
		}
	}
	else
	{
		_movement->changeDir(0, 0);
		_anim->playAnim(AnimatedSpriteComponent::EnemyIdle);
	}
}