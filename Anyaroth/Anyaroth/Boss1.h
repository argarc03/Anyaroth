#pragma once
#include "DistanceEnemy.h"
#include "Axe.h"

class ExplosiveBulletPool;
class Boss1 : public DistanceEnemy
{
	private:

		bool _fase1 = true, _fase2 = false, _fase3 = false, _beetwenFase = false, move= true;
		Vector2D _bodyPos, _playerPos, _originalPos, _amplitude = Vector2D(250,25), _velocity = Vector2D(0.5, 0.5), _dir = Vector2D(1,0);
		double  _damage = 50, _angularFrequency = 0.05, _k = _angularFrequency / _velocity.distance(Vector2D());

		BodyComponent* _playerBody;
		//Vida

		Life _life1 = 200, _life2 = 200, _life3 = 200;

		//Cosas para el ataque bombardero
		ExplosiveBulletPool* _myExplosivePool = nullptr;
		Texture* _bombTexture = nullptr;
		bool _bomberAttacking = false;
		int _bomberAttackTime = 2500, _timeOnBomberAttack = 0, _timeBeetwenBombs = 0,_bombRange = 500;

		//Cosas Melee
		Melee* _melee;

	public:
		Boss1(Player* player, Game* g, PlayState* play, Texture* texture, Vector2D posIni, string tag, BulletPool* pool, ExplosiveBulletPool* explosivePool);
		virtual ~Boss1() {};
		virtual void update(double time);


		virtual void subLife(int damage);

		void movement(double time);
		void bomberAttack(double time);
		void meleeAttack();
		bool inline const isMeleeing() { return ((_anim->getCurrentAnim() == AnimatedSpriteComponent::EnemyAttack) && !_anim->animationFinished()); }
		void checkMelee();

		void Fase1(double time);
		void Fase2(double time);
		void Fase3(double time);
		void beetwenFases(double time);


		void throwBomb();

};
