#pragma once

//#ifndef H_UTILITIES
//#define H_UTILITIES

// Macro for releasing COM objects easily
#define RELEASE_COM(x) { if(x) { x->Release(); x = 0; } }
#define SAFE_DELETE(x) { if(x) { delete x; x = 0; } }

static bool RectanglesTouching(RECT& rect1, RECT& rect2)
{
	if(rect1.bottom < rect2.top)
		return false;
	if(rect1.top > rect2.bottom)
		return false;
	if(rect1.left > rect2.right)
		return false;
	if(rect1.right < rect2.left)
		return false;
	return true;
}

// Simple directional enum
enum Directions
{
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
};

//#endif