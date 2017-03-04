register_gamestate("loading")
register_gamestate("dosowisko")
register_gamestate("empty")

option(WITH_RUST "build gamestates written in Rust" false)

if(WITH_RUST)
  add_subdirectory("rust")
endif(WITH_RUST)
