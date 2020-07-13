#pragma once
#include "FontManager.hpp"
#include "TextureManager.hpp"
#include "AudioManager.hpp"


class MyFontManager : public FontManager {

public:
	MyFontManager()
		: FontManager()
	{
		this->registerPath(AssetNames::f_opensans_reg, "Assets\\Fonts\\OpenSans-Regular.ttf");
	}

	virtual ~MyFontManager(){}
};

class MyTextureManager : public TextureManager {

public:
	MyTextureManager(bool autoSmooth = false)
		: TextureManager(autoSmooth)
	{
		this->registerPath(AssetNames::t_background, "Assets\\Sprites\\backdrop.jpg");
		this->registerPath(AssetNames::t_board, "Assets\\Sprites\\Board.png");
		this->registerPath(AssetNames::t_pieces, "Assets\\Sprites\\Pieces.png");
	}
};


class MyAudioManager :public  AudioManager {

public:
	MyAudioManager()
		: AudioManager()
	{
		this->registerPath(AssetNames::s_piece_castle, "Assets\\Sounds\\piece_castle.ogg");
		this->registerPath(AssetNames::s_piece_check, "Assets\\Sounds\\piece_check.ogg");
		this->registerPath(AssetNames::s_piece_move, "Assets\\Sounds\\piece_move.ogg");
		this->registerPath(AssetNames::s_piece_take, "Assets\\Sounds\\piece_take.ogg");
	}
};