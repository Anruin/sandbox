# Application source files.
set(SOURCE_FILES
        common.h
        errors.h
        generic/object.c
        containers/vector.c
        string/string.c
        platform/platform.c
        debug.c
        containers/buffer.c
        memory/allocate.h
        network/packet.h
        network/peer.h)

# Configure application.
add_library(SandboxCommon ${SOURCE_FILES})

if (UNIX)
    target_link_libraries(SandboxCommon pthread)
endif (UNIX)