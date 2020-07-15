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

	virtual ~MyFontManager() {}
};

class MyTextureManager : public TextureManager {

public:
	MyTextureManager(bool autoSmooth = false)
		: TextureManager(autoSmooth)
	{
		this->registerPath(AssetNames::t_background, "Assets\\Sprites\\backdrop.jpg");
		this->registerPath(AssetNames::t_board, "Assets\\Sprites\\board.png");
		this->registerPath(AssetNames::t_pieces, "Assets\\Sprites\\pieces.png");
	}
};


class MyAudioManager : public AudioManager {

public:
	MyAudioManager()
		: AudioManager()
	{
		this->registerPath(AssetNames::s_piece_castle, "Assets\\Sounds\\piece_castle.ogg");
		this->registerPath(AssetNames::s_piece_check, "Assets\\Sounds\\piece_check.ogg");
		this->registerPath(AssetNames::s_piece_move, "Assets\\Sounds\\piece_move.ogg");
		this->registerPath(AssetNames::s_piece_take, "Assets\\Sounds\\piece_take.ogg");
		this->registerPath(AssetNames::s_button_click, "Assets\\Sounds\\button_click.ogg");
		this->registerPath(AssetNames::s_button_hover, "Assets\\Sounds\\button_hover.ogg");
	}
};

class ThemeManager : public ResourceManager<ThemeManager, tgui::Theme> {

public:
	ThemeManager()
		:ResourceManager()
	{
		this->registerPath(AssetNames::theme_default, "Assets//Themes//default.theme");
	}

private:
	virtual std::shared_ptr<tgui::Theme> loadResource(const std::string & path) const override {
		auto theme = std::make_shared<tgui::Theme>();
		theme->load(path);

		return theme;
	}
};