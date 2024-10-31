set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    Main.cpp 
    pch.cpp 
    Sources/Game.cpp 
    Sources/Entities/ChessPieceEntity.cpp 
    Sources/Entities/Entity.cpp 
    Sources/Gui/GuiClientStart.cpp 
    Sources/Gui/GuiGameMessage.cpp 
    Sources/Gui/GuiInfoBox.cpp 
    Sources/Gui/GuiMainMenu.cpp 
    Sources/Gui/GuiPauseMenu.cpp 
    Sources/Gui/Base/GuiContainer.cpp 
    Sources/Gui/Base/GuiInputWindow.cpp 
    Sources/Gui/Base/GuiMessageBox.cpp 
    Sources/Managers/BoardManager.cpp 
    Sources/Managers/ChessPieceManager.cpp 
    Sources/Managers/EventManager.cpp 
    Sources/Managers/StateManager.cpp 
    Sources/Managers/Resources/SoundManager.cpp 
    Sources/Mechanics/ChessBoard.cpp 
    Sources/Mechanics/ChessPiece.cpp 
    Sources/Mechanics/ChessRules.cpp 
    Sources/Network/PacketType.cpp 
    Sources/States/SGameClient.cpp 
    Sources/States/SGameHost.cpp 
    Sources/States/SGameSandbox.cpp 
    Sources/States/SGameSinglePlayer.cpp 
    Sources/States/SMainMenu.cpp 
    Sources/States/Base/BaseGame.cpp 
    Sources/Systems/AnimatorSystem.cpp 
    Sources/Systems/UCIConnector.cpp 
    Sources/Utils/Configuration.cpp 
    Sources/Utils/Enums.cpp 
    Sources/Utils/Logger.cpp 
    Sources/Utils/Utils.cpp 
)

set(HEADERS
    Main.h 
    pch.h 
    Include/Mechanics/ChessColour.hpp 
    Include/Mechanics/ChessAction.h 
    Include/Mechanics/ChessBoard.h 
    Include/Mechanics/ChessMove.h 
    Include/Mechanics/ChessPiece.h 
    Include/Mechanics/ChessPosition.h 
    Include/Mechanics/ChessRules.h 
    Include/Network/PacketType.h 
    Include/Utils/Logger.hpp 
    Include/Utils/Math.hpp 
    Include/Utils/NonCopyable.hpp 
    Sources/Game.h 
    Sources/Entities/ChessPieceEntity.h 
    Sources/Entities/Entity.h 
    Sources/Gui/GuiBotInput.hpp 
    Sources/Gui/GuiClientStart.hpp 
    Sources/Gui/GuiHostStart.hpp 
    Sources/Gui/GuiGameMessage.h 
    Sources/Gui/GuiInfoBox.h 
    Sources/Gui/GuiMainMenu.h 
    Sources/Gui/GuiPauseMenu.h 
    Sources/Gui/Base/GuiContainer.hpp 
    Sources/Gui/Base/GuiMessageBox.hpp 
    Sources/Gui/Base/GuiWindow.hpp 
    Sources/Gui/Base/GuiWindowBlank.hpp 
    Sources/Gui/Base/GuiInputWindow.h 
    Sources/Managers/ResourceManagers.hpp 
    Sources/Managers/BoardManager.h 
    Sources/Managers/ChessPieceManager.h 
    Sources/Managers/EventManager.h 
    Sources/Managers/StateManager.h 
    Sources/Managers/Resources/AudioManager.hpp 
    Sources/Managers/Resources/FontManager.hpp 
    Sources/Managers/Resources/ResourceManager.hpp 
    Sources/Managers/Resources/SoundManager.hpp 
    Sources/Managers/Resources/TextureManager.hpp 
    Sources/Managers/Resources/AssetNames.h 
    Sources/Network/ClientInfo.hpp 
    Sources/Network/NetClient.hpp 
    Sources/Network/NetServer.hpp 
    Sources/Network/NetSettings.hpp 
    Sources/Network/PacketHandler.hpp 
    Sources/States/AllStates.hpp 
    Sources/States/SGameClient.h 
    Sources/States/SGameHost.h 
    Sources/States/SGameSandbox.h 
    Sources/States/SGameSinglePlayer.h 
    Sources/States/SMainMenu.h 
    Sources/States/States.h 
    Sources/States/STest.h 
    Sources/States/Base/BaseState.hpp 
    Sources/States/Base/SharedContext.hpp 
    Sources/States/Base/BaseGame.h 
    Sources/Systems/UCIConnector.hpp 
    Sources/Systems/AnimatorSystem.h 
    Sources/Utils/Bitmask.hpp 
    Sources/Utils/ConfigProperty.hpp 
    Sources/Utils/DebugOverlay.hpp 
    Sources/Utils/Event.hpp 
    Sources/Utils/Poller.hpp 
    Sources/Utils/Signal.hpp 
    Sources/Utils/Configuration.h 
    Sources/Utils/Enums.h 
    Sources/Utils/Utils.h 
)

# Create the executable
add_executable(Chess ${SOURCES} ${HEADERS})

# Specify include directories
target_include_directories(Chess PRIVATE Include Sources .)

# SFML Setup
set(SFML_ROOT "D:/Projects/C++ Projects/cpplibs/SFML-2.5.1")
set(TGUI_ROOT "D:/Projects/C++ Projects/cpplibs/TGUI-0.8")


target_include_directories(Chess PRIVATE ${SFML_ROOT}/include)
target_link_directories(Chess PRIVATE ${SFML_ROOT}/lib)

target_include_directories(Chess PRIVATE ${TGUI_ROOT}/include)
target_link_directories(Chess PRIVATE ${TGUI_ROOT}/lib)


target_link_libraries(Chess PRIVATE
    sfml-audio-d
    sfml-graphics-d
    sfml-window-d
    sfml-system-d
    sfml-network-d
)

target_link_libraries(Chess PRIVATE tgui-d)

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${SFML_ROOT}/bin/sfml-audio-d-2.dll,${SFML_ROOT}/bin/sfml-audio-2.dll>"
        "$<IF:$<CONFIG:Debug>,${SFML_ROOT}/bin/sfml-graphics-d-2.dll,${SFML_ROOT}/bin/sfml-graphics-2.dll>"
        "$<IF:$<CONFIG:Debug>,${SFML_ROOT}/bin/sfml-window-d-2.dll,${SFML_ROOT}/bin/sfml-window-2.dll>"
        "$<IF:$<CONFIG:Debug>,${SFML_ROOT}/bin/sfml-system-d-2.dll,${SFML_ROOT}/bin/sfml-system-2.dll>"
        "$<IF:$<CONFIG:Debug>,${SFML_ROOT}/bin/sfml-network-d-2.dll,${SFML_ROOT}/bin/sfml-network-2.dll>"
        "$<IF:$<CONFIG:Debug>,${TGUI_ROOT}/bin/tgui-d.dll,${TGUI_ROOT}/bin/tgui.dll>"
        "${SFML_ROOT}/bin/openal32.dll"
        $<TARGET_FILE_DIR:Chess>
)

# Copy assets to output
add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Assets" $<TARGET_FILE_DIR:Chess>/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Lib" $<TARGET_FILE_DIR:Chess>/Lib
)