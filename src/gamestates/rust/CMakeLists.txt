project(libsuperderpy-gamestate-rust NONE)

find_package(rustc)
find_package(rustdoc)
find_package(cargo)
include(Rust)

#cargo_dependency(cargo
#                 PACKAGE_NAMES url
#		 PACKAGE_VERSIONS =0.2.31)

#set(RUSTC_FLAGS -L ${CMAKE_BINARY_DIR}/lib -L ${CMAKE_BINARY_DIR}/cargo/target/debug/deps)
#set(RUSTDOC_FLAGS -L ${CMAKE_BINARY_DIR}/lib -L ${CMAKE_BINARY_DIR}/cargo/target/debug/deps)

# Get the dependencies of all the crates
get_rust_deps(rust.rs TESTLIB_DEPS)

# Build the library
rust_crate(rust.rs
           ALL
           TARGET_NAME TESTLIB
           DESTINATION lib
           DEPENDS "${TESTLIB_DEPS}"
           OTHER_RUSTC_FLAGS --crate-type dylib --crate-type rlib)

add_custom_target(library_target
                  ALL
                  DEPENDS ${TESTLIB_FULL_TARGET})


install(FILES ${TESTLIB_ARTIFACTS}
DESTINATION lib)

#register_gamestate("rust")
