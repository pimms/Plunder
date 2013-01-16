#include "Pim.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Settings.h"

#include <fstream>

using namespace Pim;
using namespace std;

// Global variable. I cannot be arsed to do otherwise, fuck teh system.
Settings globalSettings;

void loadSettings()
{
	ifstream file("settings.init");
	if (file.is_open()) 
	{
		string line = "";
		while (getline(file, line)) 
		{
			if (line[0] == '#') continue;

			if (strstr(line.c_str(), "smoothshadows"))
			{
				globalSettings.smoothShadows = bool(strstr(line.c_str(), "true"));
			}

			if (strstr(line.c_str(), "fullscreen"))
			{
				globalSettings.fullscreen = bool(strstr(line.c_str(), "true"));
			}

			if (strstr(line.c_str(), "castshadows"))
			{
				globalSettings.castShadows = bool(strstr(line.c_str(), "true"));
			}
		}

		file.close();
	}
}

int main()
{
	{
		loadSettings();

		WinStyle::CreationData cd("Plunder (NM i Gameplay build)", 1280, 720, WinStyle::WINDOWED);
		cd.forcedAspectRatio = true;
		cd.aspectRatio = 1280.f / 720.f;
		cd.renderResolution = Vec2(1280.f, 720.f);

		if (globalSettings.fullscreen) 
		{
			cd.winStyle = WinStyle::BORDERLESS_WINDOWED;
		}

		GameControl *gc = new GameControl;
		gc->go(new MainMenuScene, cd, false);
		delete gc;
	}

	_CrtDumpMemoryLeaks();
}