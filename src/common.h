#define LIBSUPERDERPY_DATA_TYPE struct CommonResources
#include <libsuperderpy.h>

struct CommonResources {
		// Fill in with common data accessible from all gamestates.
		char* videoname;
		char* aftervideo;
};
void HighlightCharacter(struct Game *game, struct Character *character, float alpha);

struct CommonResources* CreateGameData(struct Game *game);
void DestroyGameData(struct Game *game, struct CommonResources *data);
