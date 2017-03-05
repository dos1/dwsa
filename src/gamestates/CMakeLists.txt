register_gamestate("loading")
register_gamestate("dosowisko")
register_gamestate("opening")
register_gamestate("video")
register_gamestate("plans")
register_gamestate("winda")
register_gamestate("evil")
register_gamestate("machine")
register_gamestate("corridor")

target_link_libraries("libsuperderpy-${LIBSUPERDERPY_GAMENAME}-opening" ${ALLEGRO5_VIDEO_LIBRARIES})
target_link_libraries("libsuperderpy-${LIBSUPERDERPY_GAMENAME}-video" ${ALLEGRO5_VIDEO_LIBRARIES})

