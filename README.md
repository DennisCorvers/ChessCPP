# ChessCPP

This project was a motivation to use C++ in a game development setting. The project uses SFML for rendering/sound etc and is structured using an finite state model along with Entities and Components.

## Extra Notes
In order to structurize the project/files better, post-build are used.

The following post-build events are used. Assets contains a folder structure with textures/sounds, etc while Lib includes additional libraries (.dll's) that the executable requires.

```
xcopy "$(ProjectDir)Assets" "$(TargetDir)Assets" /e /y /i /r
xcopy "$(ProjectDir)Lib" "$(TargetDir)" /e /y /i /r
```

Loading files with SFML uses a path relative to the current executing application. For instance, when loading the Board texture, the following .loadFromFile argument is used:

```C++
loadFromFile("Assets\\Sprites\\Board.png")
```
