#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>

struct CommonResources {
		// Fill in with common data accessible from all gamestates.
		char* videoname;
		char* aftervideo;

		bool skip;

		ALLEGRO_FONT *font;
	bool skipevil;
	  char* text;
		ALLEGRO_BITMAP *text_icon;

		ALLEGRO_BITMAP *faceg, *faceb, *face;

		ALLEGRO_AUDIO_STREAM *music;
bool winner;
    struct Timeline *timeline;
};
void HighlightCharacter(struct Game *game, struct Character *character, float alpha);

void DrawDialogs(struct Game *game);
void LogicDialogs(struct Game *game);
void SayDialog(struct Game *game, ALLEGRO_BITMAP* who, char* what, char* file);
bool Speak(struct Game *game, struct TM_Action *action, enum TM_ActionState state);

struct CommonResources* CreateGameData(struct Game *game);
void DestroyGameData(struct Game *game, struct CommonResources *data);
