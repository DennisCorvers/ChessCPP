#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>

namespace Utils {
#ifdef RUNNING_WINDOWS

#include <windows.h>
#include <Shlwapi.h>


	inline std::string GetWorkingDirectory() {
		HMODULE hModule = GetModuleHandle(nullptr);
		if (hModule) {
			char path[256];
			GetModuleFileName(hModule, path, sizeof(path));
			PathRemoveFileSpec(path);
			strcat_s(path, "\\");
			return std::string(path);
		}
		return "";
	}

	inline std::string GetWorkingDirectory(std::string filename) {
		return GetWorkingDirectory() + filename;
	}
#elif defined RUNNING_LINUX
#include <unistd.h>
	inline std::string GetWorkingDirectory() {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != nullptr) {
			return std::string(cwd) + std::string("/");
		}
		return "";
	}

	inline std::string GetWorkingDirectory(std::string filename) {
		return GetWorkingDirectory() + filename;
	}
#endif
}

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics.hpp>

namespace Graphics {

	class ResourceLoader {
	public:

		static sf::Font* loadFont(const std::string& path) {

			sf::Font* font = new sf::Font();
			if (!font->loadFromFile(path))
			{
				delete font;
				font = nullptr;
				std::cerr << "! Failed to load font: " << path << std::endl;
			}
			return font;
		}
		static sf::Texture* loadTexture(const std::string& path) {

			sf::Texture* texture = new sf::Texture();
			if (!texture->loadFromFile(path))
			{
				delete texture;
				texture = nullptr;
				std::cerr << "! Failed to load texture: " << path << std::endl;
			}
			return texture;
		}
	};
}