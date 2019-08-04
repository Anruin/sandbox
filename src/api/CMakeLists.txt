# Application source files.
set(SOURCE_FILES
        application.h
        module.h
        platform.h
        api.c)

# Configure application.
add_library(SandboxAPI ${SOURCE_FILES})