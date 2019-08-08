cmake_minimum_required(VERSION 3.5)

set(SOURCE_FILES module.c)

add_library(SandboxClientModule MODULE ${SOURCE_FILES})

# Tell our linker that we are building module so it must export required symbols.
target_compile_definitions(SandboxClientModule PRIVATE SANDBOX_MODULE)

# Enable PIC.
set_property(TARGET SandboxClientModule PROPERTY POSITION_INDEPENDENT_CODE ON)