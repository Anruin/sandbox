# Configure additional required libraries.
set(REQUIRED_LIBRARIES
		SandboxCommon)

set(SOURCE_FILES
		configuration.h
		configuration.c)

add_library(SandboxConfiguration ${SOURCE_FILES})

target_link_libraries(SandboxConfiguration PUBLIC ${REQUIRED_LIBRARIES})
