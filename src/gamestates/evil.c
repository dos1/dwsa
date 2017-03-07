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
		bool moveleft, moveup, movedown, moveright, mirror, highlight1;
		bool init;
		bool unlock;
int counter;
    struct Character *drzwi;

};

int Gamestate_ProgressCount = 3; // number of loading steps as reported by Gamestate_Load

void Gamestate_Logic(struct Game *game, struct GamestateResources* data) {
	// Called 60 times per second. Here you should do all your game logic.
	AnimateCharacter(game, data->ego, 1);
	data->counter++;

	if (data->init) {
		data->moveup = true;
		if (GetCharacterY(game, data->ego) < 680) {
			data->moveup = false;
			data->init = false;
			SelectSpritesheet(game, data->ego, "top1");

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
	if (GetCharacterX(game, data->ego) > 1700) {
		SetCharacterPosition(game, data->ego, 1700, GetCharacterY(game, data->ego), 0);
	}
	if (GetCharacterX(game, data->ego) < 10) {
		SetCharacterPosition(game, data->ego, 10, GetCharacterY(game, data->ego), 0);
	}

if (data->unlock) {
	  data->highlight1 = (GetCharacterX(game, data->ego) > 1100);
} else {
	data->highlight1 = false;
}

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

	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_LEFT)) {
		data->moveleft = false;
	}
	if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode == ALLEGRO_KEY_RIGHT)) {
		data->moveright = false;
	}


	if (!left && (data->moveleft || data->moveright)) {
		SelectSpritesheet(game, data->ego, "walkkrawat");
	}
	if (left && !(data->moveleft || data->moveright)) {
		SelectSpritesheet(game, data->ego,"top1");
	}

	if ((ev->type==ALLEGRO_EVENT_KEY_DOWN) && (ev->keyboard.keycode == ALLEGRO_KEY_SPACE)) {
		if (data->highlight1) {
			StartGamestate(game, "machine");
		}
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

	data->bg = al_load_bitmap(GetDataFilePath(game, "evil.png"));
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->ego = CreateCharacter(game, "ego");
	RegisterSpritesheet(game, data->ego, "top1");
	RegisterSpritesheet(game, data->ego, "walkkrawat");
	RegisterSpritesheet(game, data->ego, "top");
	LoadSpritesheets(game, data->ego);
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	data->drzwi = CreateCharacter(game, "activator");
	RegisterSpritesheet(game, data->drzwi, "maszyna");
	LoadSpritesheets(game, data->drzwi);
	progress(game); // report that we progressed with the loading, so the engine can draw a progress bar

	return data;
}

void Gamestate_Unload(struct Game *game, struct GamestateResources* data) {
	// Called when the gamestate library is being unloaded.
	// Good place for freeing all allocated memory and resources.
	al_destroy_bitmap(data->bg);
	DestroyCharacter(game, data->ego);
	DestroyCharacter(game, data->drzwi);
	free(data);
}


bool StartPlans(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
//	struct GamestateResources *data = TM_GetArg(action->arguments, 0);
	struct GamestateResources *data = TM_GetArg(action->arguments, 0);
	if (state == TM_ACTIONSTATE_START) {
		StartGamestate(game, "plans");
		data->unlock = false;
	}
	return true;
}
bool StopPlans(struct Game *game, struct TM_Action *action, enum TM_ActionState state) {
	struct GamestateResources *data = TM_GetArg(action->arguments, 0);
	if (state == TM_ACTIONSTATE_START) {
		StopGamestate(game, "plans");
		data->unlock = true;
	}
	return true;
}



void Gamestate_Start(struct Game *game, struct GamestateResources* data) {
	// Called when this gamestate gets control. Good place for initializing state,
	// playing music etc.
	SelectSpritesheet(game, data->ego, "top");
	SetCharacterPosition(game, data->ego, 1100, 950, 0);
data->init = true;
data->unlock = true;
if (game->data->skipevil) {
	data->unlock = true;
}
  data->moveleft = false;
	data->movedown = false;
	data->moveright = false;
	data->moveup = false;
	data->highlight1 = false;
data->counter = 0;
  SelectSpritesheet(game, data->drzwi, "maszyna");
	SetCharacterPosition(game, data->drzwi, 1396, 410, 0);

	if (!game->data->skipevil) {
	SayDialog(game, game->data->faceb, "Oh, there you are. I was awaiting you.", "evil1");
	SayDialog(game, game->data->faceb, "And well, I'm impressed, I must say.", "evil2");
	SayDialog(game, game->data->faceb, "I thought my evil maze of dangerous traps will keep you engaged for a little bit longer.", "evil3");

	SayDialog(game, game->data->faceg, "Evil maze? I took an elevator.", "maze");

	SayDialog(game, game->data->faceb, "Oh... uhm, yeah, elevator... that explains a lot.", "evil4");

	SayDialog(game, game->data->faceb, "But, okay! If you made it this far, you must be already aware of my masterful plan of building The Evil Comic Sans-o-nator 3000.", "evil5");
	SayDialog(game, game->data->faceb, "As you know, we are inside of it right now, it's already initiated and there's nothing you can do to stop me from firing it!", "evil6");

	TM_AddAction(game->data->timeline, StartPlans, TM_AddToArgs(NULL, 1, data), "plans");

	SayDialog(game, game->data->faceb, "Then my deathly beam will reach the Earth and turn every single font in every single internet browser into Comic Sans.", "evil7");
	SayDialog(game, game->data->faceb, "Muahahaha!", "evil8");
	SayDialog(game, game->data->faceb, "But well, you sure already know that this is not everything.", "evil9");
	SayDialog(game, game->data->faceb, "Actually, there will be one browser left intact: the wonderful Internet Explorer.", "evil10");
	SayDialog(game, game->data->faceb, "Frustrated inhabitants of planet Earth will turn their hopes into their most hated browser as their only way to keep away from Comic Sans...", "evil11");
	SayDialog(game, game->data->faceb, "...and as we all know, Internet Explorer will make them so vulnerable I'll take over their PCs in no time, which means taking over their lifes, and in turn, taking over the whole world!", "evil12");
	SayDialog(game, game->data->faceb, "Muhahahaaha!", "evil13");
	TM_AddAction(game->data->timeline, StopPlans, TM_AddToArgs(NULL, 1, data), "plans");
	SayDialog(game, game->data->faceb, "How do you feel now, knowing that you're absolutely powerless against my malevolent plans?", "evil14");
	SayDialog(game, game->data->faceb, "You thwarted my operations way too many times already, now it's time to get my sweet revenge!", "evil15");
}
	SayDialog(game, game->data->faceb, "Feel the power of The Evil Comic Sans-o-nator 3000!", "evil16");

	for (int i=0; i<8; i++) {
		SayDialog(game, game->data->faceb, "Muahahahahahahahahahahahaha!", "l1");
		SayDialog(game, game->data->faceb, "Ahahahhhahahaha! Muoahohohohoaohaoo!", "l2");
		SayDialog(game, game->data->faceb, "Muaahahahaha hhhahahahahaahahahahaha!", "l3");
		SayDialog(game, game->data->faceb, "Ahahahah hahahaahahahahahahahahahahah!", "l4");
		SayDialog(game, game->data->faceb, "Muahahahahahahahahahhahahahhaha!", "l5");
		SayDialog(game, game->data->faceb, "Hahahahahahahahahahahahahahaa!", "l6");
		SayDialog(game, game->data->faceb, "Ohohoahahahahahahahahahahahhohaha!", "l7");
		SayDialog(game, game->data->faceb, "Hahahahahahahahhahahaahaha!", "l8");
		SayDialog(game, game->data->faceb, "Muahhahahahahahahahahahahahahahaha!", "l9");
		SayDialog(game, game->data->faceb, "Hahahahhahaahahahahahahahahaaha!", "l10");
		SayDialog(game, game->data->faceb, "Muahhahahahahahahahahahahah!", "l11");
		SayDialog(game, game->data->faceb, "Haha ha ha hhahaha phew haha!", "l12");
		SayDialog(game, game->data->faceb, "Hahhhahahahahahahahahhhahh!", "l13");
		SayDialog(game, game->data->faceb, "Hahahahahahahahahahahha!", "l14");
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
