cmake_minimum_required(VERSION 3.5)

set(SOURCE_FILES module.c)

add_library(SandboxTrackerModule MODULE ${SOURCE_FILES})

set(REQUIRED_LIBRARIES
		${SDL2_LIBRARY}
		${SDL2_NET_LIBRARY})

target_link_libraries(SandboxTrackerModule PUBLIC ${REQUIRED_LIBRARIES})

# Tell our linker that we are building module so it must export required symbols.
target_compile_definitions(SandboxTrackerModule PRIVATE SANDBOX_MODULE)

# Enable PIC.
set_property(TARGET SandboxTrackerModule PROPERTY POSITION_INDEPENDENT_CODE ON)