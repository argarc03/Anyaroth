#include "DistanceEnemy.h"
#include "Game.h"

DistanceEnemy::DistanceEnemy(Player* player, Game* g, PlayState* play, Texture* texture, Vector2D posIni, string tag, GunType type) : Enemy(player, g, play, texture, posIni, tag)
{
	_arm = new EnemyArm(g, this, player, { 35,30 });
	_arm->setTexture(g->getTexture("ArmPistol"));
	addChild(_arm);

	// ShooterInterface* sh = getGame()->gameGuns[type].shooter;
	// int mA = getGame()->gameGuns[type].maxAmmo;
	// int mC = getGame()->gameGuns[type].maxClip;
	// double c = getGame()->gameGuns[type].cadence;
	
	//_arm->setGun(new Gun(_arm, sh, bp, type, mA, mC, c));
}

DistanceEnemy::~DistanceEnemy()
{
	delete _arm->getCurrentGun();
}

void DistanceEnemy::RayCast()
{
	BodyComponent* target = _player->getComponent<BodyComponent>();

	b2Vec2 enemyPos = _body->getBody()->GetPosition();
	b2Vec2 targetPos = target->getBody()->GetPosition();

	b2RayCastInput rayInput;

	rayInput.maxFraction = 1;
	rayInput.p1 = { (float32)(enemyPos.x), (float32)(enemyPos.y - _body->getH()) };
	rayInput.p2 = { (float32)(targetPos.x), (float32)(targetPos.y) };

	b2RayCastOutput rayOutput;

	_armVision = true;

	for (b2Body* b = getWorld()->GetBodyList(); b && _armVision; b = b->GetNext())
		for (b2Fixture* f = b->GetFixtureList(); f && _armVision; f = f->GetNext())
			if (b->GetType() == b2_staticBody && f->RayCast(&rayOutput, rayInput, 0))
				_armVision = false;
}