#pragma once
#include "Boss.h"
#include "LaserHandler.h"

class Boss2 : public Boss
{
private:
	LaserHandler* _lasers = nullptr;
	Vector2D _velocity = { 20,0 }, _originalVelocity = { 20,0 };
	int _dir, _onFloor = 0, _stopRange = 103;
	double _shootRange = 120, _speedIncrement = 100;
	bool _particles = false;

	int _timeStartMelee = 400, _timeOnMelee = 0;

	int _timeToShoot = 700, _timeWaiting = 0;

	int _timeToJump = 100, _timeWaitingJump = 0;
	bool _jump = false, _realMelee = false;;
public:
	Boss2(Game* g, Player* player, Vector2D pos, BulletPool* pool);
	~Boss2();

	void Jump();
	void checkJump(const double& deltaTime);

	virtual void movement(const double& deltaTime);
	virtual void beginCollision(GameObject* other, b2Contact* contact);
	virtual void endCollision(GameObject * other, b2Contact* contact);

	virtual void meleeAttack();
	void endJump();
	virtual void checkMelee(const double& deltaTime);
	virtual int const  getDamage()
	{
		return _damage;
	}

	virtual void fase1(const double& deltaTime);
	virtual void fase2(const double& deltaTime);
	virtual void fase3(const double& deltaTime);
	virtual void beetwenFases(const double& deltaTime);
	virtual void manageLife(Life& l, int damage);

	virtual void die();
};

