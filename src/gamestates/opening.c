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
#include <allegro5/allegro_video.h>
#include <math.h>

struct GamestateResources {
		// This struct is for every resource allocated and used by your gamestate.
		// It gets created on load and then gets passed around to all other function calls.
		ALLEGRO_FONT *font;
		int counter;

		ALLEGRO_VIDEO *video;
		ALLEGRO_BITMAP *bg, *closed, *open, *security;

		struct Character *zzz, *ego;

		float alpha;

		bool mirror;

		bool highlight;
		bool gotit;
		bool goback;

		bool videoended;
		int videodelay;

		bool videoend, moveleft, moveup, movedown, moveright;
};

int Gamestate_ProgressCount = 1; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	AnimateCharacter(game, data->zzz, 1);
	AnimateCharacter(game, data->ego, 1);
	data->counter++;
	if (data->goback) {
		if (GetCharacterX(game, data->ego) < 50) {
			MoveCharacter(game, data->ego, 7, 0, 0);
		} else {
			data->goback = false;
		}

	} else {

	if (data->videoend) {
		if (data->movedown) {
			MoveCharacter(game, data->ego, 0, 3, 0);
		}
		if (data->moveup) {
			MoveCharacter(game, data->ego, 0, -3, 0);
		}
		if (data->moveleft) {
			MoveCharacter(game, data->ego, -7, 0, 0);
			data->mirror = true;
		}
		if (data->moveright) {
			MoveCharacter(game, data->ego, 7, 0, 0);
			data->mirror = false;
		}
		data->alpha-=0.05;
		if (data->alpha < 0) data->alpha = 0;
	}

	if (data->videoended) {
		if (data->videodelay > 0) data->videodelay--;
		if (data->videodelay == 0) {
			data->videoended = false;
			data->videoend = true;

			SayDialog(game, game->data->faceg, "Hmm, he actually might be useful.", "s1");
		}
	}

	if (GetCharacterY(game, data->ego) < 200) {
		SetCharacterPosition(game, data->ego, GetCharacterX(game, data->ego), 200, 0);
	}
	if (GetCharacterY(game, data->ego) > 380) {
		SetCharacterPosition(game, data->ego, GetCharacterX(game, data->ego), 380, 0);
	}
	if (GetCharacterX(game, data->ego) > 730) {
		SetCharacterPosition(game, data->ego, 730, GetCharacterY(game, data->ego), 0);
	}
	if (GetCharacterX(game, data->ego) < -375) {
		if (data->gotit) {
			SwitchCurrentGamestate(game, "video");
			LoadGamestate(game, "winda");
			game->data->videoname = strdup("winda.ogv");
			game->data->aftervideo = strdup("winda");
		} else {
			data->goback = true;
			SayDialog(game, NULL, "UNAUTHORIZED PERSONEL", "up");
			SayDialog(game, game->data->faceg, "The door won't open until I get some disguise.", "s3");

		}
	}

	}

	if (!data->gotit) {
		data->highlight = GetCharacterX(game, data->ego) > 600;
	}

	LogicDialogs(game);
}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
	ALLEGRO_BITMAP *frame = al_get_video_frame(data->video);
	//al_draw_bitmap(data->bg, 0, 0, 0);
	al_draw_bitmap(data->closed, 0, 0, 0);

	float scale = 1.5 + ((GetCharacterY(game, data->ego) - 800) / 200.0) * 0.2;


	if (data->videoend) {
		float alpha = 1 - data->alpha;
		al_draw_tinted_bitmap(data->open, al_map_rgba(255*alpha,255*alpha,255*alpha,255*alpha), 0, 0, 0);
	}

	al_draw_bitmap(data->security, 1370, 0, 0);

	  if (frame) {
			al_draw_tinted_scaled_rotated_bitmap_region(frame, 175, 15, 1118, 558,
			                                            al_map_rgba(255*data->alpha,255*data->alpha,255*data->alpha,255*data->alpha),
			                                            0, 0, 171, 8, 1.008, 1.015, 0, 0);
			al_draw_tinted_scaled_rotated_bitmap_region(frame, 175, 15, 1118, 558,
			                                            al_map_rgba(255*data->alpha,255*data->alpha,255*data->alpha,255*data->alpha),
			                                            0, 0, 171, 8, 1.008, 1.015, 0, 0);
			al_draw_tinted_scaled_rotated_bitmap_region(frame, 175, 15, 1118, 558,
			                                            al_map_rgba(255*data->alpha,255*data->alpha,255*data->alpha,255*data->alpha),
			                                            0, 0, 171, 8, 1.008, 1.015, 0, 0);
			al_draw_tinted_scaled_rotated_bitmap_region(frame, 175, 15, 1118, 558,
			                                            al_map_rgba(255*data->alpha,255*data->alpha,255*data->alpha,255*data->alpha),
			                                            0, 0, 171, 8, 1.008, 1.015, 0, 0);
		}
		if (data->videoend) {
			float alpha = 1 - data->alpha;
			DrawScaledCharacter(game, data->ego, al_map_rgba(255*alpha,255*alpha,255*alpha,255*alpha), 1*scale, 1*scale, data->moveleft ? ALLEGRO_FLIP_HORIZONTAL : 0);
		}

	if (data->highlight) {
		HighlightCharacter(game, data->zzz, fabs(sin(data->counter / 40.0)) / 2.0 + 0.5);
	} else {
		DrawCharacter(game, data->zzz, al_map_rgb(255,255,255), 0);
	}

	DrawDialogs(game);

}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		if (data->videoend) {
			UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
			// When there are no active gamestates, the engine will quit.
		} else {
			al_set_video_playing(data->video, false);
			data->videoend = true;
		}
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
		SelectSpritesheet(game, data->ego, "top");
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
		left = false;
		SelectSpritesheet(game, data->ego, data->gotit ? "standkrawat": "stand");
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_DOWN)) {
		data->movedown = false;
	}

	if (!left && (data->moveleft || data->moveright)) {
		SelectSpritesheet(game, data->ego, data->gotit ? "walkkrawat": "walk");
	}
	if (left && !(data->moveleft || data->moveright)) {
		if (data->moveup) {
			SelectSpritesheet(game, data->ego, "top");
		} else {
			SelectSpritesheet(game, data->ego, data->gotit ? "standkrawat": "stand");
		}
	}

	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_SPACE)) {
		if (data->highlight) {
			SelectSpritesheet(game, data->ego, "standkrawat");
			SelectSpritesheet(game, data->zzz, "zzz2");
			data->gotit = true;
			data->highlight = false;
			SayDialog(game, game->data->faceg, "Now I should be able to get through von Wissenschäftler's guards unnoticed.", "s4");
		}
	}

	if (ev->type==ALLEGRO_EVENT_VIDEO_FINISHED) {
		data->videoended = true;
	}

	if (ev->type==ALLEGRO_EVENT_KEY_DOWN) {

		if (ev->keyboard.keycode == ALLEGRO_KEY_FULLSTOP) {
			if (game->data->text) {
				game->data->skip = true;
			}
		}

	}
}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));
	data->font = al_create_builtin_font();
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->video = al_open_video(GetDataFilePath(game, "window.ogv"));
	data->bg = al_load_bitmap(GetDataFilePath(game, "sleepbg.png"));
	data->closed = al_load_bitmap(GetDataFilePath(game, "sleepclosed.png"));
	data->open = al_load_bitmap(GetDataFilePath(game, "sleepopen.png"));
	data->security = al_load_bitmap(GetDataFilePath(game, "security.png"));

	data->zzz = CreateCharacter(game, "zzz");
	RegisterSpritesheet(game, data->zzz, "zzz");
	RegisterSpritesheet(game, data->zzz, "zzz2");
	LoadSpritesheets(game, data->zzz);

	data->ego = CreateCharacter(game, "ego");
	RegisterSpritesheet(game, data->ego, "stand");
	RegisterSpritesheet(game, data->ego, "standkrawat");
	RegisterSpritesheet(game, data->ego, "walk");
	RegisterSpritesheet(game, data->ego, "walkkrawat");
	RegisterSpritesheet(game, data->ego, "top");
	LoadSpritesheets(game, data->ego);

	al_register_event_source(game->_priv.event_queue, al_get_video_event_source(data->video));

	return data;
}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_font(data->font);
	free(data);
}

void Gamestate_Start(struct Game *game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	data->videodelay = 50;
	data->videoended = false;
	data->highlight = false;
	data->gotit = false;
	data->goback = false;
	SelectSpritesheet(game, data->zzz, "zzz");
	SelectSpritesheet(game, data->ego, "stand");

	SetCharacterPosition(game, data->ego, 582, 281, 0);
	SetCharacterPosition(game, data->zzz, 1141, 323, 0);

	data->videoend = false;
	al_start_video(data->video, game->audio.fx);

	data->alpha = 1;
	data->moveleft = false;
	data->movedown = false;
	data->moveright = false;
	data->moveup = false;
	data->counter = 0;
}

void Gamestate_Stop(struct Game *game, struct GamestateResources* data) {
	// Called when gamestate gets stopped. Stop timers, music etc. here.
	al_set_video_playing(data->video, false);
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
