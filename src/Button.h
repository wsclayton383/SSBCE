#pragma once
#include <graphx.h>
#include "gfx/gfx.h"
#include "gfx/menu_palette.h"
#include <TINYSTL/vector.h>
#include <string.h>

using namespace tinystl;

struct Button
{
	int xpos, ypos, scale;
	int player;
	int menuLink;
	char* text;

	Button(int x, int y, char* bText, int bPlayer, int bLink, int bScale = 1)
	{
		xpos = x;
		ypos = y;
		text = bText;
		player = bPlayer;
		menuLink = bLink;
		scale = bScale;
	}
};