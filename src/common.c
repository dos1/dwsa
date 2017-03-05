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
	data->videoname = strdup("mart.ogv");

	data->text = NULL; //strdup("Oh... uhm, yeah, elevator... that explains a lot. But, okay! If you made it this far, you must be already aware of my masterful plan of building The Evil Comic Sans-o-nator 3000. As you know, we are inside of it right now, it's already initiated and there's nothing you can do to stop me from firing it! Then my deathly beam will reach the Earth and turn every single font in every single internet browser into Comic Sans. Muahahaha! But well, you sure already know that this is not everything. Actually, there will be one browser left intact: the wonderful Internet Explorer. Frustrated inhabitants of planet Earth will turn their hopes into their most hated browser as the only way to keep away from Comic Sans... and as we all know, Internet Explorer will make them so vulnerable I'll take over their PCs in no time, which means taking over their lifes, and in turn, taking over the whole world! Muhahahaaha! How do you feel now, knowing that you're absolutely powerless against my malevolent plans? You thwarted my operations way too many times already, now it's time to get my sweet revenge! Feel the power of The Evil Comic Sans-o-nator 3000, hahahahahahahahahahahahauahahuahahahahahaa(...)ahahahahahahahahahahahahahahahahahuahuaah!");
	data->font = al_load_font(GetDataFilePath(game, "fonts/DroidSans.ttf"), 42, 0);

	data->faceb = al_load_bitmap(GetDataFilePath(game, "faceb.png"));
	data->faceg = al_load_bitmap(GetDataFilePath(game, "faceg.png"));
	data->face = NULL;

	data->skip = false;
	data->timeline = TM_Init(game, "speech");

	return data;
}

bool Speak(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
//	struct GamestateResources *data = TM_GetArg(action->arguments, 0);
	ALLEGRO_AUDIO_STREAM *stream = TM_GetArg(action->arguments, 0);
	char *text = TM_GetArg(action->arguments, 1);
	ALLEGRO_BITMAP *face = TM_GetArg(action->arguments, 2);

	if (state == TM_ACTIONSTATE_INIT) {
		al_set_audio_stream_playing(stream, false);
		al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_ONCE);
	}

	if (state == TM_ACTIONSTATE_START) {
		game->data->skip = false;
		//al_rewind_audio_stream(stream);
		al_attach_audio_stream_to_mixer(stream, game->audio.voice);
		al_set_audio_stream_playing(stream, true);

		game->data->text = text;
		game->data->face = face;
	}

	if (state == TM_ACTIONSTATE_RUNNING) {
		return !al_get_audio_stream_playing(stream) || game->data->skip;
	}

	if (state == TM_ACTIONSTATE_DESTROY) {
		//if (game->data->skip) {
		  al_destroy_audio_stream(stream);
		//}
		game->data->text = NULL;
		game->data->face = NULL;
	}
	return false;
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

void DrawDialogs(struct Game *game) {
	if (game->data->text) {
		al_draw_filled_rectangle(0, 1080-180, 1920, 1080, al_map_rgba(32,32,32,32));
		al_draw_filled_rectangle(0, 1080-180, 1920, 1080, al_map_rgba(0,0,0,164));

		DrawWrappedTextWithShadow(game->data->font, al_map_rgb(255,255,255), 200, 1080-160, 1650, ALLEGRO_ALIGN_LEFT, game->data->text);
		if (game->data->face) {
			al_draw_bitmap(game->data->face, 0, 1080-al_get_bitmap_height(game->data->face), 0);
		}
	}
}

void LogicDialogs(struct Game *game) {
	TM_Process(game->data->timeline);
}

void EventDialogs(struct Game *game, ALLEGRO_EVENT *ev) {
	TM_HandleEvent(game->data->timeline, ev);
}

void SayDialog(struct Game *game, ALLEGRO_BITMAP* who, char* what, char* file) {
	char buffer[255];
	snprintf(buffer, 255, "voices/%s.flac", file);
	TM_AddAction(game->data->timeline, Speak, TM_AddToArgs(NULL, 3, al_load_audio_stream(GetDataFilePath(game, buffer), 4, 1024),
	                                                 what, who), "speak");
}


