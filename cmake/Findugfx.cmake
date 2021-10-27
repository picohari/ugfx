# Define UGFX_ROOT
set(UGFX_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

# Ensure that CHIBIOS_ROOT is set
if(NOT UGFX_ROOT)
    message(FATAL_ERROR "No UGFX_ROOT specified")
endif()

# Assemble list of components
list(APPEND ugfx_COMPONENTS
    gadc
    gaudio
    gdisp
    gdriver
    gevent
    gfile
    ginput
    gmisc
    gos
    gqueue
    gtimer
    gtrans
    gwin
)

# Core sources
list(APPEND ugfx_SOURCES
    ${UGFX_ROOT}/src/gfx.c
)

# Core include directories
list(APPEND ugfx_INCLUDE_DIRS
    ${UGFX_ROOT}/src
)

# Include each component
foreach(component ${ugfx_COMPONENTS})
    include(${UGFX_ROOT}/src/${component}/${component}.cmake)
endforeach()

# Include target setup for ease-of-use
include(${CMAKE_CURRENT_LIST_DIR}/target_setup.cmake)

# Remove duplicates from non-cached variables
list(REMOVE_DUPLICATES ugfx_SOURCES)
list(REMOVE_DUPLICATES ugfx_INCLUDE_DIRS)

# Outsource heavy-lifting to cmake
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ugfx DEFAULT_MSG ugfx_SOURCES ugfx_INCLUDE_DIRS)
