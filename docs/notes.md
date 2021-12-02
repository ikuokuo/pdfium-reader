# Notes

## Add Dependencies

```zsh
# imgui: https://github.com/ocornut/imgui
git submodule add https://github.com/ocornut/imgui.git third_party/imgui
cd third_party/imgui
git checkout v1.85

# glfw: https://github.com/glfw/glfw
#  mac
brew install glfw
#  debian/ubuntu
sudo apt install libglfw3-dev

# glm: https://github.com/g-truc/glm
git submodule add https://github.com/g-truc/glm.git third_party/glm
cd third_party/glm
git checkout 0.9.9.8

# native file dialog: https://github.com/btzy/nativefiledialog-extended.git
git submodule add https://github.com/btzy/nativefiledialog-extended.git third_party/nativefiledialog
#  mac: AppKit
#  debian/ubuntu
sudo apt install libgtk-3-dev
#  win: ole32.lib, uuid.lib

# stb: https://github.com/nothings/stb
git submodule add https://github.com/nothings/stb.git third_party/stb
```
