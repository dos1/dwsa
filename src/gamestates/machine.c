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
		ALLEGRO_BITMAP *bg, *arrow;
		float rot;
		float speed;

		float time;
};

int Gamestate_ProgressCount = 2; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.

	data->time += 0.0015;
	data->rot-= (data->speed / 120) + (data->rot / 500);

	bool cel = false;
	if (data->rot < 0.5) {
		cel = true;
	}
	if (data->rot > ALLEGRO_PI - 0.5) {
		cel = true;
	}

	if (cel) {
		al_play_sample_instance(game->data->target);
	} else {
		al_stop_sample_instance(game->data->target);
	}


	data->speed += data->rot / 500;
	if (data->speed > ALLEGRO_PI / 1.75) {
		data->speed = ALLEGRO_PI / 1.75;
	}
	if (data->rot<0) data->rot=0;
	if (data->rot>ALLEGRO_PI) data->rot=ALLEGRO_PI;

	if (data->time >= 1) {
		TM_CleanQueue(game->data->timeline);
		StopGamestate(game, "evil");
		SwitchCurrentGamestate(game, "video");
		al_set_audio_stream_playing(game->data->music, false);

		if (data->rot <= ALLEGRO_PI - 0.5) {
			game->data->videoname = strdup("endlose.ogv");
			game->data->aftervideo = NULL;
		} else {
			game->data->videoname = strdup("endwin.ogv");
			game->data->aftervideo = NULL;
			game->data->winner = true;
		}

	}
}

void Gamestate_Draw(struct Game *game, struct GamestateResources* data) {
	// Called as soon as possible, but no sooner than next Gamestate_Logic call.
	// Draw everything to the screen here.
al_draw_bitmap(data->bg, 0, 0, 0);

bool cel = false;
if (data->rot < 0.5) {
	cel = true;
}
if (data->rot > ALLEGRO_PI - 0.5) {
	cel = true;
}

al_draw_tinted_rotated_bitmap(data->arrow, cel ? al_map_rgb(255,32,32) : al_map_rgb(255,255,255),
                              226, 158, 715+226, 425+158, data->rot, 0);

al_draw_filled_rectangle(200, 840, 200+(1920-440)*data->time, 880, al_map_rgb(255,255,255));
DrawDialogs(game);
}

void Gamestate_ProcessEvent(struct Game *game, struct GamestateResources* data, ALLEGRO_EVENT *ev) {
	// Called for each event in Allegro event queue.
	// Here you can handle user input, expiring timers etc.
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE)) {
		UnloadCurrentGamestate(game); // mark this gamestate to be stopped and unloaded
		// When there are no active gamestates, the engine will quit.
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_LEFT)) {
		data->rot += 0.2;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_RIGHT)) {
		data->rot -= 0.2;
	}

}

void* Gamestate_Load(struct Game *game, void (*progress)(struct Game*)) {
	// Called once, when the gamestate library is being loaded.
	// Good place for allocating memory, loading bitmaps etc.
	struct GamestateResources *data = malloc(sizeof(struct GamestateResources));

	data->bg = al_load_bitmap(GetDataFilePath(game, "bg.png"));
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar
	data->arrow = al_load_bitmap(GetDataFilePath(game, "arrow.png"));
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	game->data->sample = al_load_sample(GetDataFilePath(game, "voices/ta.flac"));
	game->data->target = al_create_sample_instance(game->data->sample);
	al_attach_sample_instance_to_mixer(game->data->target, game->audio.fx);

	al_set_sample_instance_gain(game->data->target, 0.8);

	return data;
}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_bitmap(data->bg);
	al_destroy_bitmap(data->arrow);
	al_destroy_sample_instance(game->data->target);
	al_destroy_sample(game->data->sample);
	free(data);
}

void Gamestate_Start(struct Game *game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	data->rot = 0;
	data->speed = 1;
	data->time = 0;

	TM_CleanQueue(game->data->timeline);
	SayDialog(game, game->data->faceg, "Looks like he added new security measures since the last time I defeated him.", "security");

	for (int i=0; i<8; i++) {
		SayDialog(game, game->data->faceb, "Muahhahahahahahahahahahahahahahaha!", "l9");
		SayDialog(game, game->data->faceb, "Hahahahhahaahahahahahahahahaaha!", "l10");
		SayDialog(game, game->data->faceb, "Muahhahahahahahahahahahahah!", "l11");
		SayDialog(game, game->data->faceb, "Haha ha ha hhahaha phew haha!", "l12");
		SayDialog(game, game->data->faceb, "Hahhhahahahahahahahahhhahh!", "l13");
		SayDialog(game, game->data->faceb, "Hahahahahahahahahahahha!", "l14");
		SayDialog(game, game->data->faceb, "Muahahahahahahahahahahahaha!", "l1");
		SayDialog(game, game->data->faceb, "Ahahahhhahahaha! Muoahohohohoaohaoo!", "l2");
		SayDialog(game, game->data->faceb, "Muaahahahaha hhhahahahahaahahahahaha!", "l3");
		SayDialog(game, game->data->faceb, "Ahahahah hahahaahahahahahahahahahahah!", "l4");
		SayDialog(game, game->data->faceb, "Muahahahahahahahahahhahahahhaha!", "l5");
		SayDialog(game, game->data->faceb, "Hahahahahahahahahahahahahahaa!", "l6");
		SayDialog(game, game->data->faceb, "Ohohoahahahahahahahahahahahhohaha!", "l7");
		SayDialog(game, game->data->faceb, "Hahahahahahahahhahahaahaha!", "l8");
}
	SayDialog(game, game->data->faceb, "Haha ha ha hhahaha phew haha!", "l12");

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
