#pragma once
#include "FramedImageUI.h"


class ButtonUI : public FramedImageUI
{
	private:
		typedef void Callback(Game*);
		struct Frames { uint onOut = 0; uint onOver = 0; uint onDown = 0; uint onUp = 0; };

		enum ButtonState { None, Over, Out, Down, Up };
		ButtonState _positionState = Out;	//Over o Out
		ButtonState _pressState = None;		//Up o Down

		Callback* _onOutCallback = nullptr;
		Callback* _onOverCallback = nullptr;
		Callback* _onDownCallback = nullptr;
		Callback* _onUpCallback = nullptr;

		uint _onOutFrame = 0;
		uint _onOverFrame = 0;
		uint _onDownFrame = 0;
		uint _onUpFrame = 0;

		bool mouseIsOver();

	public:
		ButtonUI(Game* game, Texture* image, Callback* callback = nullptr, Frames frames = { 0, 0, 0, 0 });
		virtual ~ButtonUI() {};

		virtual void update(const double& deltaTime);
		virtual void handleEvent(const SDL_Event& event);

		inline void onUp(Callback* callback) { _onUpCallback = callback; }
		inline void onDown(Callback* callback) { _onDownCallback = callback; }
		inline void onOver(Callback* callback) { _onOverCallback = callback; }
		inline void onOut(Callback* callback) { _onOutCallback = callback; }

		void setFrames(Frames frames);
		virtual void setVisible(bool a);
};
