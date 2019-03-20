#include "Map.h"
#include "BodyComponent.h"
#include "MeleeEnemy.h"
#include "MartyrEnemy.h"
#include "DistanceStaticEnemy.h"
#include "DistanceDynamicEnemy.h"
#include "Coin.h"
#include "GunType_def.h"
#include <json.hpp>

using namespace nlohmann;

Map::Map(string filename, Game* game, PlayState* playstate, Texture* tileset, int coinValue) : _game(game), _playState(playstate), _coinValue(coinValue)
{
	_player = _playState->getPlayer();

	json j;
	fstream file;
	file.open(filename);
	if (file.is_open())
	{
		file >> j;
		j = j["layers"];
		for (int i = 0; i < j.size(); i++)
		{
			auto it = j[i].find("name");
			if (it != j[i].end())
			{
				if (*it == "Map" || *it == "Ground")
				{
					_layers[*it] = new Layer(filename, *it, tileset, game, *it);
					_layersNames.push_back(*it);

					if(*it=="Ground")
						_layers[*it]->addComponent<BodyComponent>();

					_levelObjects.push_back(_layers[*it]);
				}
				else
				{
					_objects[*it] = new ObjectLayer(filename, *it);
					_objectsNames.push_back(*it);
				}
			}
		}
		file.close();
	}
	else
		throw AnyarothError("No se ha encontrado el archivo introducido");

	createObjects();

	setActive(true);
}

Map::~Map()
{
	for (int i = 0; i < _objectsNames.size(); i++)
		delete _objects[_objectsNames[i]];

	_objects.clear();

	for (auto it = _levelObjects.begin(); it != _levelObjects.end(); ++it)
		delete *it;

	_levelObjects.clear();
}

void Map::createObjects()
{
	for (int i = 0; i < _objectsNames.size(); i++)
	{
		string name = _objectsNames[i];
		vector<Vector2D> pos = _objects[name]->getObjectsPositions();

		for (int j = 0; j < pos.size(); j++)
		{
			if (name == "Player")
			{
				_player->setPlayerPosition(Vector2D(pos[j].getX() / M_TO_PIXEL, (pos[j].getY() - TILES_SIZE * 2) / M_TO_PIXEL));
			}
			else if (name == "Melee")
			{
				_levelObjects.push_back(new MeleeEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name));
			}
			else if (name == "Martyr")
			{
				_levelObjects.push_back(new MartyrEnemy(_player, _game, _playState, _game->getTexture("EnemyMartyr"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name));
			}
			else if (name == "DistanceEstatic")
			{
				_levelObjects.push_back(new DistanceStaticEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name, BasicEnemyGun));
			}
			else if (name == "DistanceDynamic")
			{
				_levelObjects.push_back(new DistanceDynamicEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name, BasicEnemyGun));
			}
			else if (name == "Coin")
			{
				_levelObjects.push_back(new Coin(_game, _game->getTexture("Coin"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE), _coinValue));
			}
		}
	}
}

bool Map::handleInput(const SDL_Event & event)
{
	GameComponent::handleInput(event);

	for (GameComponent* o : _levelObjects)
		if (o->isActive())
			o->handleInput(event);

	return false;
}

void Map::update(double time)
{
	GameComponent::update(time);

	for (GameComponent* o : _levelObjects)
		if (o->isActive())
			o->update(time);
}

void Map::render(Camera * c) const
{
	GameComponent::render(c);

	for (GameComponent* o : _levelObjects)
		if (o->isActive())
			o->render(c);
}