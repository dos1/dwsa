/*! \file common.c
 *  \brief Common stuff that can be used by all gamestates.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "common.h"
#include <libsuperderpy.h>

struct CommonResources* CreateGameData(struct Game *game) {
	struct CommonResources *data = calloc(1, sizeof(struct CommonResources));

	data->aftervideo = strdup("opening");
	data->videoname = strdup("window.ogv");
	return data;
}

void HighlightCharacter(struct Game *game, struct Character *character, float alpha) {
	alpha *= 0.2;
	MoveCharacter(game, character, 2*8, 2*8, 0);
	DrawCharacter(game, character, al_map_rgba(alpha*255,alpha*255,0,alpha*255), 0);
	MoveCharacter(game, character, -4*8, -4*8, 0);
	DrawCharacter(game, character, al_map_rgba(alpha*255,alpha*255,0,alpha*255), 0);
	MoveCharacter(game, character, 2*8, 0, 0);
	DrawCharacter(game, character, al_map_rgba(alpha*255,alpha*255,0,alpha*255), 0);
	MoveCharacter(game, character, 0, 4*8, 0);
	DrawCharacter(game, character, al_map_rgba(alpha*255,alpha*255,0,alpha*255), 0);
	MoveCharacter(game, character, 0, -2*8, 0);
	alpha *= 3;
	DrawCharacter(game, character, al_map_rgba(255,255,255,255), 0);
	DrawCharacter(game, character, al_map_rgba(alpha*255,alpha*255,0,alpha*255), 0);
}

void DestroyGameData(struct Game *game, struct CommonResources *data) {
	free(data);
}

