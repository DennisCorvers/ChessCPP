# ChessCPP

This project was a motivation to use C++ in a game development setting. The project uses SFML for rendering/sound etc and is structured using an finite state model along with Entities and Components. The game features a client-host multiplayer mode, meaning that the host has to port forward the game and the client connects directly 1-on-1 with the host.

## Feature list

- Movement of pieces via mouse; Selection, Deselection, Drag-and-Drop, Single-Click movement
- Preview of all possible moves for a selected piece
- Partial history of previous states
- Various sounds in menus and during games
- All chess rules for pieces
- End-game via Check, Checkmate, Stalemate and Draw
- Board mirroring
- Piece animations
- Main Menu
- In-game Menu
- Customizable UI themes
- Chess Engine integration
- Client-Host multiplayer
- Client validation/rollback of inputs
- Spectator mode

## Application Preview 

![Main Screen](https://i.imgur.com/xWQN6Rg.png)

![Game Screen](https://i.imgur.com/IAh9PlA.png)

![Multiplayer Screen](https://i.imgur.com/xPLTJXJ.png)

## Extra Notes
In order to structurize the project/files better, post-build events are used.

The following post-build events are used:
```
xcopy "$(ProjectDir)Assets" "$(TargetDir)Assets" /e /y /i /r /d
xcopy "$(ProjectDir)Lib" "$(TargetDir)" /e /y /i /r /d
```
Assets contains a folder structure with textures/sounds, etc while Lib includes additional libraries (.dll's) that the executable requires.

Loading files with SFML uses a path relative to the current executing application. For instance, when loading the Board texture, the following .loadFromFile argument is used:

```C++
loadFromFile("Assets\\Sprites\\Board.png")
```

Disabling the Console Window on release builds:

Go to Configuration Properties > Linker > System > SubSystem
```
Windows (/SUBSYSTEM:WINDOWS)
```
