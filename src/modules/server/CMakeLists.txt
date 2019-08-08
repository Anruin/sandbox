cmake_minimum_required(VERSION 3.5)

set(SOURCE_FILES module.c)

add_library(SandboxServerModule MODULE ${SOURCE_FILES})

# Tell our linker that we are building module so it must export required symbols.
target_compile_definitions(SandboxServerModule PRIVATE SANDBOX_MODULE)

# Enable PIC.
set_property(TARGET SandboxServerModule PROPERTY POSITION_INDEPENDENT_CODE ON)