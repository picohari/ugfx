function(ugfx_target_setup TARGET)

    target_include_directories(
        ${TARGET}
        PRIVATE
            ${ugfx_INCLUDE_DIRS}
    )

    target_sources(
        ${TARGET}
        PRIVATE
            ${ugfx_SOURCES}
    )

    target_compile_definitions(
        ${TARGET}
        PRIVATE
            ${ugfx_DEFS}
    )

endfunction()
