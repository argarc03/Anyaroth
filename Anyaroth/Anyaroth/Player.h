#pragma once
#include "GameObject.h"
#include "TransformComponent.h"
#include "CustomAnimatedSpriteComponent.h"
#include "BodyComponent.h"
#include "PlayerArm.h"
#include "Gun.h"
#include "Money.h"
#include "Life.h"
#include "PlayerPanel.h"
#include "BulletPool.h"
#include "Melee.h"

class Game;

class Player : public GameObject
{
private:
	Game* _game = nullptr;

	//Componentes
	TransformComponent* _transform = nullptr;
	CustomAnimatedSpriteComponent* _anim = nullptr;
	BodyComponent* _body = nullptr;
	Melee* _melee = nullptr;

	//Propiedades
	Life _life = Life(100);
	Money* _money = nullptr;
	PlayerPanel* _playerPanel = nullptr;
	BulletPool* _playerBulletPool = nullptr;

	//Hijos
	PlayerArm* _playerArm = nullptr;

	//Variable auxiliares
	int _dashCD = 3000, _maxDash = 2, _numDash = _maxDash, dashDur = 250;
	bool _isDashing = false, _isReloading = false, _isShooting = false, _isMeleeing = false, _onDash = false, dashDown = false;
	int _floorCount = 0;
	float _timeToJump = 100.f;

	Gun* _currentGun = nullptr;
	Gun* _otherGun = nullptr;

	inline bool dashIsAble() const { return _numDash > 0 && _isDashing; }
	void checkMovement(const Uint8* keyboard);
	void handleAnimations();

	void refreshCooldowns(const double& deltaTime);
	void refreshDashCoolDown(const double& deltaTime);
	void dashTimer(const double& deltaTime);
	void refreshGunCadence(const double& deltaTime);
	inline void setGrounded(bool grounded) { grounded ? _timeToJump = 100.f : _floorCount = grounded; }

	bool canReload();
	void checkMelee();

public:
	Player(Game* g, int xPos, int yPos);
	~Player();

	bool handleEvent(const SDL_Event& event);
	void update(const double& deltaTime);

	virtual void beginCollision(GameObject* other, b2Contact* contact);
	virtual void endCollision(GameObject* other, b2Contact* contact);

	void die();
	void revive();
	void subLife(int damage);

	void swapGun();
	inline void changeCurrentGun(Gun* gun) { _currentGun = gun; }
	inline void changeOtherGun(Gun* gun) { _otherGun = gun; }
	inline Gun* getCurrentGun() const { return _currentGun; }
	inline Gun* getOtherGun() const { return _otherGun; }

	void move(const Vector2D& dir, const double& speed);
	void dash(const Vector2D& dir);
	void dashOff();
	void jump();

	void melee();
	void shoot();
	void reload();

	void setPlayerPanel(PlayerPanel* p);
	inline void setPlayerBulletPool(BulletPool* pool) { _playerBulletPool = pool; }
	inline void setPlayerPosition(Vector2D pos) { _body->getBody()->SetTransform(b2Vec2(pos.getX(), pos.getY()), 0); }
	
	void changeMelee(Melee* newMelee);

	inline bool isGrounded() const { return _floorCount; }
	bool isDashing() const;
	bool isMeleeing() const;
	bool isReloading() const;
	bool isJumping() const;
	bool isFalling() const;
};