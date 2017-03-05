/*! \file empty.c
 *  \brief Empty gamestate.
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

#include "../common.h"
#include <libsuperderpy.h>
#include <math.h>

struct GamestateResources {
		// This struct is for every resource allocated and used by your gamestate.
		// It gets created on load and then gets passed around to all other function calls.
		ALLEGRO_BITMAP *bg;
		struct Character *ego;
		bool moveleft, moveup, movedown, moveright, mirror, highlight1, highlight2;
		bool init;
int counter;
    struct Character *drzwi, *winda;

};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	AnimateCharacter(game, data->ego, 1);
	data->counter++;

	if (data->init) {
		data->moveup = true;
		if (GetCharacterY(game, data->ego) < 666) {
			data->moveup = false;
			data->init = false;
		}
	}

	if (data->movedown) {
		MoveCharacter(game, data->ego, 0, 1 * ((GetCharacterY(game, data->ego) > 700) ? 2 : 1), 0);
		if (GetCharacterX(game, data->ego) > 800) {
			MoveCharacter(game, data->ego, 1, 0, 0);
		} else {
			MoveCharacter(game, data->ego, -1, 0, 0);
		}
	}
	if (data->moveup) {
		MoveCharacter(game, data->ego, 0, -1 * ((GetCharacterY(game, data->ego) > 700) ? 2 : 1), 0);
		if (GetCharacterX(game, data->ego) > 800) {
			MoveCharacter(game, data->ego, -1, 0, 0);
		} else {
			MoveCharacter(game, data->ego, 1, 0, 0);
		}
	}
	if (data->moveleft) {
		MoveCharacter(game, data->ego, -6, 0, 0);
		data->mirror = true;
	}
	if (data->moveright) {
		MoveCharacter(game, data->ego, 6, 0, 0);
		data->mirror = false;
	}

	if (GetCharacterY(game, data->ego) < 540) {
		SetCharacterPosition(game, data->ego, GetCharacterX(game, data->ego), 540, 0);
	}
	if (GetCharacterY(game, data->ego) > 950) {
		SetCharacterPosition(game, data->ego, GetCharacterX(game, data->ego), 950, 0);
	}
	if (GetCharacterX(game, data->ego) > 1300) {
		SetCharacterPosition(game, data->ego, 1300, GetCharacterY(game, data->ego), 0);
	}
	if (GetCharacterX(game, data->ego) < 350) {
		SetCharacterPosition(game, data->ego, 350, GetCharacterY(game, data->ego), 0);
	}


	  data->highlight1 = (GetCharacterX(game, data->ego) < 550) && (GetCharacterY(game, data->ego) < 580);
		data->highlight2 = (GetCharacterX(game, data->ego) > 550) && (GetCharacterX(game, data->ego) < 1000) && (GetCharacterY(game, data->ego) < 580);


	LogicDialogs(game);
}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
	al_draw_scaled_bitmap(data->bg, 0, 0, al_get_bitmap_width(data->bg), al_get_bitmap_height(data->bg), 0, 0, game->viewport.width, game->viewport.height, 0);
	//DrawScaledCharacter(game, data->doctor, al_map_rgb(255,255,255), 0.2,0.2,0);
	float scale = 0.2 + ((GetCharacterY(game, data->ego) - 520) / 280.0) * 0.8;

	if (data->highlight1) {
		HighlightCharacter(game, data->drzwi, fabs(sin(data->counter / 40.0)) / 2.0 + 0.5);
	} else {
		DrawCharacter(game, data->drzwi, al_map_rgb(255,255,255), 0);
	}
	if (data->highlight2) {
		HighlightCharacter(game, data->winda, fabs(sin(data->counter / 40.0)) / 2.0 + 0.5);
	} else {
		DrawCharacter(game, data->winda, al_map_rgb(255,255,255), 0);
	}

//	MoveCharacter(game, data->ego, 0, -200, 0);
	DrawScaledCharacter(game, data->ego, al_map_rgb(255,255,255), 1*scale, 1*scale, data->moveleft ? ALLEGRO_FLIP_HORIZONTAL : 0);
//	MoveCharacter(game, data->ego, 0, 200, 0);


	DrawDialogs(game);

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}

	bool left = data->moveleft || data->moveright;

	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_LEFT)) {
		data->moveleft = true;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_RIGHT)) {
		data->moveright = true;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_UP)) {
		data->moveup = true;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_DOWN)) {
		data->movedown = true;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_LEFT)) {
		data->moveleft = false;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_RIGHT)) {
		data->moveright = false;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_UP)) {
		data->moveup = false;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_DOWN)) {
		data->movedown = false;
	}

	if (!left && (data->moveleft || data->moveright)) {
		SelectSpritesheet(game, data->ego, "walkkrawat");
	}
	if (left && !(data->moveleft || data->moveright)) {
		SelectSpritesheet(game, data->ego,"standkrawat");
	}

	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_SPACE)) {
		if (data->highlight1) {
			UnloadAllGamestates(game);
		}
		if (data->highlight2) {
			SwitchCurrentGamestate(game, "plans");
		}
	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->bg = al_load_bitmap(GetDataFilePath(game, "winda.png"));

	data->ego = CreateCharacter(game, "ego");
	RegisterSpritesheet(game, data->ego, "standkrawat");
	RegisterSpritesheet(game, data->ego, "walkkrawat");
	LoadSpritesheets(game, data->ego);

	data->drzwi = CreateCharacter(game, "activator");
	RegisterSpritesheet(game, data->drzwi, "drzwi");
	LoadSpritesheets(game, data->drzwi);

	data->winda = CreateCharacter(game, "activator");
	RegisterSpritesheet(game, data->winda, "winda");
	LoadSpritesheets(game, data->winda);

	return data;
}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	free(data);
}

void Gamestate_Start(struct Game *game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	SelectSpritesheet(game, data->ego, "standkrawat");
	SetCharacterPosition(game, data->ego, 1100, 950, 0);
data->init = true;
  data->moveleft = false;
	data->movedown = false;
	data->moveright = false;
	data->moveup = false;
	data->highlight1 = false;
	data->highlight2 = false;
data->counter = 0;
  SelectSpritesheet(game, data->drzwi, "drzwi");
	SelectSpritesheet(game, data->winda, "winda");
	SetCharacterPosition(game, data->drzwi, 356, 525, 0);
	SetCharacterPosition(game, data->winda, 765, 467, 0);
}

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
}

void Gamestate_Pause(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets paused (so only Draw is being called, no Logic not ProcessEvent)
	// Pause your timers here.
}

void Gamestate_Resume(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets resumed. Resume your timers here.
}

// Ignore this for now.
// TODO: Check, comment, refine and/or remove:
void Gamestate_Reload(struct Game *game, struct GamestateResources* data) {}
