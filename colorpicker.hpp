#pragma once

#include <ncurses.h>
#include <vector>

short id = 1;
std::vector<short> createdcolors;

/**
 * Uses the ncurses library to change the draw color of the terminal.
 * This function can only create 254 colors. 0 is reserved for default.
 */
void fromRGB(int r, int g, int b)
{
	// RGB in ncurse is from 0-1000. Normalize to 0-255
	r *= 1000/255;
	g *= 1000/255;
	b *= 1000/255;
	
	// Try to reuse a color
	short red, green, blue, tempid1, tempid2;
	for (int i = 0; i < createdcolors.size(); i++) {
		pair_content(createdcolors[i], &tempid1, &tempid2);
		color_content(tempid2, &red, &green, &blue);
		if (red == r && green == g && blue == b) {
			attron(COLOR_PAIR(tempid2));
			return;
		}
	}

	// Otherwise create a new one
	init_color(id, r, g, b);
	init_pair(id, COLOR_WHITE, id);
	attron(COLOR_PAIR(id));
	createdcolors.emplace_back(id);
	id++;	
}
