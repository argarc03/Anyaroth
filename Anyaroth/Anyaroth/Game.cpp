#include "Game.h"
#include <ctime>
#include "AnyarothError.h"
#include <json.hpp>

using namespace nlohmann;

void Game::createTextures()
{
	ifstream input;
	input.open(INFO_PATH + "assets.json");
	if (input.is_open())
	{
		json j;
		input >> j;
		j = j["textures"];
		int numTextures = j.size();
		for (int i = 0; i < numTextures; i++)
		{
			string id, name;
			int fil, col;
			id = j[i][0].get<string>();
			name = j[i][1].get<string>();
			fil = j[i][2];
			col = j[i][3];

			_textures.insert(pair <string, Texture*>(id, new Texture(_renderer, SPRITE_PATH + name, fil, col)));
		}
	}
	else
		throw AnyarothError("No se ha encontrado el archivo introducido");

	input.close();
}

void Game::createFonts()
{
	ifstream input;
	input.open(INFO_PATH + "assets.json");
	if (input.is_open())
	{
		json j;
		input >> j;
		j = j["fonts"];
		int numFonts = j.size();
		string id, name;
		int size;
		for (int i = 0; i < numFonts; i++)
		{
			id = j[i][0].get<string>();
			name = j[i][1].get<string>();
			size = j[i][2];
			
			_fonts.insert(pair <string, Font*>(id, new Font(FONTS_PATH + name, size)));
		}
	}
	else
		throw AnyarothError("No se ha encontrado el archivo introducido");

	input.close();
}

void Game::createSounds()
{

	_soundManager->addMusic("bgMusic", SOUNDS_PATH + "bgMusic.wav");
	_soundManager->addMusic("shop", SOUNDS_PATH + "shop.mp3");
	_soundManager->addSFX("example1", SOUNDS_PATH + "example1.wav");

	//UI SOUNDS
		//Next Text (CAMBIAR)
	_soundManager->addSFX("example", SOUNDS_PATH + "example.wav");
		//Dialogue
	_soundManager->addSFX("openDialogue", SOUNDS_PATH + "openDialogue.wav");
	_soundManager->addSFX("closeDialogue", SOUNDS_PATH + "closeDialogue.wav");

	//VOICES
		//Example
	_soundManager->addSFX("exampleVoice", SOUNDS_PATH + "exampleVoice.wav");
		//Boss
	_soundManager->addSFX("bossVoice", SOUNDS_PATH + "bossVoice.wav");
}

void Game::createDialogues()
{
	ifstream input;
	input.open(INFO_PATH + "dialogues.json");
	if (input.is_open())
	{
		json j;
		input >> j;
		j = j["dialogues"];
		for (json::iterator it = j.begin(); it != j.end(); ++it) 
		{
			string id = it.key();
			json aux = it.value();
			cout << id<<endl;
			int numDialogues = j.size();
			string face, voice, name;
			vector<string> conversation;
			vector<int> faces;
			vector<string> sounds;

			face = aux["face"].get<string>();
			name = aux["name"].get<string>();
			voice = aux["voice"].get<string>();
			conversation = aux["dialogue"].get<vector<string>>();
			faces = aux["faces"].get<vector<int>>();
			sounds = aux["sounds"].get<vector<string>>();

			_dialogues.insert(pair <string, Dialogue>(id, Dialogue{ getTexture(face),voice,name, conversation,faces,sounds }));
		}
	}
	else
		throw AnyarothError("No se ha encontrado el archivo introducido");

	input.close();
}

void Game::toggleFullscreen()
{
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP; //fake fullscreen (windowed mode)
	bool IsFullscreen = SDL_GetWindowFlags(_window) & FullscreenFlag;
	SDL_SetWindowFullscreen(_window, IsFullscreen ? 0 : FullscreenFlag);
}

Game::Game()
{
	srand(time(NULL));//random seed

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	TTF_Init(); //Ventana del tamaño de la pantalla de cada dispositivo
	SDL_DisplayMode monitor;
	SDL_GetCurrentDisplayMode(0, &monitor);
	auto win_width = monitor.w - 50;
	auto win_height = monitor.h - 80;

	_window = SDL_CreateWindow("Anyaroth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(_renderer, GAME_RESOLUTION_X, GAME_RESOLUTION_Y);

	//Icon
	SDL_Surface* icon = IMG_Load((SPRITE_PATH + "icon.png").c_str());
	SDL_SetWindowIcon(_window, icon);

	//Show cursor
	SDL_ShowCursor(true);

	//---Create textures
	createTextures();
	//---Create fonts
	createFonts();
	//---Create sounds
	_soundManager = new SoundManager();
	createSounds();
	//---Create dialogues
	createDialogues();
	//---Create world
	_world = new b2World(b2Vec2(0.0, 9.8));
	//---Create states
	_stateMachine->pushState(new MenuState(this));
}

Game::~Game()
{
	//delete textures
	for (auto it = _textures.begin(); it != _textures.end(); it++)
		delete (*it).second;

	//delete fonts
	for (auto it = _fonts.begin(); it != _fonts.end(); it++)
		delete (*it).second;

	delete _stateMachine;
	delete _world;
	delete _soundManager;
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Game::run()
{
	Uint32 deltaTime = FRAME_RATE;
	Uint32 startTime = SDL_GetTicks();

	while (!_exit)
	{
		handleEvents();
		deltaTime = SDL_GetTicks() - startTime;
		if (deltaTime >= FRAME_RATE)
		{
			_world->Step((float32)deltaTime / 1000.f, 8, 3);
			update(deltaTime);
			render();
			startTime = SDL_GetTicks();
		}
	}
}

void Game::update(const double& deltaTime)
{
	_stateMachine->currentState()->update(deltaTime);
	_stateMachine->currentState()->post_update();
}

void Game::render() const
{
	SDL_RenderClear(_renderer);
	_stateMachine->currentState()->render();
	_world->DrawDebugData();
	SDL_RenderPresent(_renderer);
}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) && !_exit)
	{
		if (event.type == SDL_QUIT)
			_exit = true;
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_F11)
				toggleFullscreen();
			else if (event.key.keysym.sym == SDLK_1)
				_soundManager->playSFX("example");
			else if (event.key.keysym.sym == SDLK_2)
				_soundManager->playSFX("example1");
		}

		_stateMachine->currentState()->handleEvent(event);
	}
}