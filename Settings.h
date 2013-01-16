#pragma once

struct Settings {
	Settings() 
	{
		fullscreen = false;
		smoothShadows = true;
		castShadows = true;
	}

	bool	fullscreen;
	bool	smoothShadows;
	bool	castShadows;
};

extern Settings globalSettings;