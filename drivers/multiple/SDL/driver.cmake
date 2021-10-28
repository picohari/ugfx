set(ROOT_PATH ${UGFX_ROOT}/drivers/multiple/SDL)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gdisp_lld_SDL.c
)

list(APPEND ugfx_DEFS
    GFX_OS_PRE_INIT_FUNCTION=sdl_driver_init
)

list(APPEND ugfx_LIBS
	SDL2::SDL2
)

# Find SDL2
find_package(
    SDL2
    REQUIRED
)

