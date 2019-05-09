#include "MenuState.h"
#include "PlayState.h"
#include "MenuButton.h"
#include "BackGround.h"

MenuState::MenuState(Game* g) : GameState(g)
{

}

void MenuState::start()
{
	//Show cursor
	SDL_ShowCursor(true);

	//HUD
	_menuHud = new MenuStateHUD(_gameptr);
	setCanvas(_menuHud);

	//Camera BackGound
	_mainCamera->setBackGround(new BackGround(_gameptr->getTexture("BgMenu"), _mainCamera));

	g->getSoundManager()->playMusic("mainMenu", true);
}