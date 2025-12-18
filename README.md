# festGL [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

<img width="534" height="467" alt="festGL transparent logo" src="https://github.com/user-attachments/assets/c5d74f77-8f59-42e5-a086-8b2db600700e" />

festGl is a OpenGL 3D Renderer (work in progress) written in modern C++ and modern OpenGL API 4.6. This project is meant to be cross-platform compatible with Windows and Linux using CMake.

## Showcase
<img width="641" height="511" alt="imagen" src="https://github.com/user-attachments/assets/d093c376-a9fd-47bc-9a69-70c5d4a6f0a1" />

![festGL-Renderer_rotating_3d_cube-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/a1ec4767-4a98-490e-ba1c-03620b430e42)

## Building
In order to build successfully, a compiler compatible with C++23 standard is needed.

The project can be built using CMake as follows:
```sh
git clone https://github.com/pabletefest/festGL.git
cd festGL
mkdir build
cd build
cmake ..
```

This will build the project inside the build folder using installed compiler on your system.

## External dependencies
Currently using the following third-party libraries:
  - [GLFW 3.4](https://github.com/glfw/glfw)
  - [Glad](https://github.com/Dav1dde/glad)
  - [glm](https://github.com/g-truc/glm)
  - [ImGui](https://github.com/ocornut/imgui)
  - [stb_image](https://github.com/nothings/stb)

## Acknowledgements

  - [Learn OpenGL](https://learnopengl.com/): for teaching OpenGL and graphics basics.
  - [The Cherno](https://www.thecherno.com/): for teaching graphics programming.
  - [RenderDoc](https://renderdoc.org/): open source graphics debugging tool. 

## Copyright

festGL is Copyright © 2025 - 2025 pabletefest.<br>
It is licensed under the terms of the GNU General Public License (GPL) 3.0 or any later version. See [LICENSE](LICENSE) for details.
