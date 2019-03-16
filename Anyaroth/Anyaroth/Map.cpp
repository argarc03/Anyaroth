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

Map::Map(string filename, Texture* tileset, Game* game, PlayState* playstate, Player* player) : _game(game), _playState(playstate), _player(player)
{
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

					_mapObjects.push_back(_layers[*it]);
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

	for (auto it = _mapObjects.begin(); it != _mapObjects.end(); it++)
		addChild(*it);
}

Map::~Map()
{
	int tam = _objectsNames.size();
	for (int i = 0; i < tam; i++)
		delete _objects[_objectsNames[i]];

	_objects.clear();
}

void Map::createObjects()
{
	for (int i = 0; i < _objectsNames.size(); i++)
	{
		string name = _objectsNames[i];
		vector<Vector2D> pos = _objects[name]->getObjectsPositions();

		for (int j = 0; j < pos.size(); j++)
		{
			if (name == "Melee")
			{
				_mapObjects.push_back(new MeleeEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name));
			}
			else if (name == "Martyr")
			{
				_mapObjects.push_back(new MartyrEnemy(_player, _game, _playState, _game->getTexture("EnemyMartyr"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name));
			}
			else if (name == "DistanceEstatic")
			{
				_mapObjects.push_back(new DistanceStaticEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name, BasicEnemyGun));
			}
			else if (name == "DistanceDynamic")
			{
				_mapObjects.push_back(new DistanceDynamicEnemy(_player, _game, _playState, _game->getTexture("EnemyMelee"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE * 2), name, BasicEnemyGun));
			}
			else if (name == "Coin")
			{
				Coin* coin = new Coin(_playState, _game, _game->getTexture("Coin"), Vector2D(pos[j].getX(), pos[j].getY() - TILES_SIZE), _coinValue);
				_mapObjects.push_back(coin);
				auto itFR = --(_mapObjects.end());
				coin->setItList(itFR);
			}
		}
	}
}