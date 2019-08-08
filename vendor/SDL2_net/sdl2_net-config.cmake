set(SDL2_NET_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_NET_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2_net.lib")
else ()
    set(SDL2_NET_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2_net.lib")
endif ()

string(STRIP "${SDL2_NET_LIBRARY}" SDL2_NET_LIBRARY)