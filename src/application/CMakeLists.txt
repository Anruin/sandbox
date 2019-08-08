# Configure additional required libraries.
set(REQUIRED_LIBRARIES
		${SDL2_LIBRARY}
		SandboxCommon
		SandboxConfiguration)

if (UNIX)
	# Add dl for dynamic loaded libraries on UNIX systems
	set(REQUIRED_LIBRARIES ${REQUIRED_LIBRARIES} c)
endif ()

# Application source files.
set(SOURCE_FILES
		main.c
		application.c configuration.c application.h)

# Configure application.
add_executable(SandboxApp ${SOURCE_FILES})
target_link_libraries(SandboxApp PUBLIC ${REQUIRED_LIBRARIES})

# Make application export symbols for dynamic libraries loading.
set_target_properties(SandboxApp PROPERTIES ENABLE_EXPORTS TRUE)
if (MSVC)
	set_target_properties(SandboxApp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin")
endif ()

# Copy assets to build directory.
#file(GLOB Assets ${CMAKE_SOURCE_DIR}/assets)
#add_custom_target(AssetsCopy)
#add_custom_command(TARGET AssetsCopy PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${Assets} ${BINARY_DIR}/assets)
#add_dependencies(SandboxApp AssetsCopy)