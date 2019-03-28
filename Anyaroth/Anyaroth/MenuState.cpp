#include "MenuState.h"
#include "PlayState.h"
#include "MenuButton.h"
#include "BackGround.h"
#include "ShopCanvas.h"

MenuState::MenuState(Game* g) : GameState(g)
{
	//Show cursor
	SDL_ShowCursor(true);

	int buttonH = g->getTexture("Play")->getH()*BUTTON_SCALE;
	int buttonW = g->getTexture("Play")->getW()*BUTTON_SCALE;

	_stages.push_back(new MenuButton(Vector2D(CAMERA_RESOLUTION_X / 2 - buttonW / 2, CAMERA_RESOLUTION_Y / 2 - buttonH + 50), g->getTexture("Play"), g, startGame));
	_stages.push_back(new MenuButton(Vector2D(CAMERA_RESOLUTION_X / 2 - buttonW / 2, CAMERA_RESOLUTION_Y / 2 + buttonH + 50), g->getTexture("Exit"), g, exitGame));

	//Camera BackGound
	_mainCamera->setBackGround(new BackGround(g->getTexture("BgMenu"), _mainCamera));

	//g->getSoundManager()->playMusic("bgMusic", true);
	setCanvas(new ShopCanvas(g));
}

void MenuState::startGame(Game * g)
{
	g->changeState(new PlayState(g));
}

void MenuState::exitGame(Game * g)
{
	g->setExit(true);
}