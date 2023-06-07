cmake_minimum_required(VERSION 3.14)
project(SFMLGame)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)


set(SOURCE_FILES
        src/detail/pugiconfig.hpp
        src/detail/pugixml.cpp
        src/detail/pugixml.hpp
        src/Objects/FadeInOut.cpp
        src/Objects/FadeInOut.h
        src/Objects/TextBox.cpp
        src/Objects/TextBox.h
        src/Scenes/Scene_Escapee.cpp
        src/Scenes/Scene_Escapee.h
        src/Scenes/Scene_Guard.cpp
        src/Scenes/Scene_Guard.h
        src/Scenes/Scene_MainMenu.cpp
        src/Scenes/Scene_MainMenu.h
        src/Scenes/Scene_Server.cpp
        src/Scenes/Scene_Server.h
        src/Scenes/Scene_Splash.cpp
        src/Scenes/Scene_Splash.h
        src/Scenes/SceneBase.h
        src/Scenes/SceneEnums.h
        src/Tools/Lerp.cpp
        src/Tools/Lerp.h
        src/Tools/UiEnumsCycle.cpp
        src/Tools/UiEnumsCycle.h
        src/Tools/Vector2.cpp
        src/Tools/Vector2.h
        src/Bot.cpp
        src/Bot.h
        src/Character.cpp
        src/Character.h
        src/Client.cpp
        src/Client.h
        src/Controller.cpp
        src/Controller.h
        src/FreeFuncs.cpp
        src/Game.cpp
        src/Game.h
        src/GameObject.cpp
        src/GameObject.h
        src/ImageLayer.cpp
        src/LayerGroup.cpp
        src/main.cpp
        src/Map.cpp
        src/miniz.c
        src/miniz.h
        src/Object.cpp
        src/ObjectGroup.cpp
        src/ObjectTypes.cpp
        src/PacketOverrides.cpp
        src/PacketOverrides.h
        src/Property.cpp
        src/Server.cpp
        src/Server.h
        src/Tile.cpp
        src/Tile.h
        src/TileLayer.cpp
        src/Tilemap.cpp
        src/Tilemap.h
        src/Tileset.cpp src/Objects/EndScreen.cpp src/Objects/EndScreen.h)

add_executable(SFMLGame ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

target_link_libraries (SFMLGame sfml-graphics sfml-window sfml-system sfml-network sfml-audio)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Data/)

