#ifndef SDLTOMYGUI_H
#define SDLTOMYGUI_H

#include <MyGUI.h>
#include <SDL.h>

namespace input {
class SdlToMygui {
private:
  int mMouseX;
  int mMouseY;
  int mMouseZ;
  bool mMouseMove;
  int mWidth;
  int mHeight;
  std::map<int, MyGUI::KeyCode> mSDLVKeyMap;
  std::map<int, MyGUI::MouseButton> mSDLMouseMap;

public:
  SdlToMygui();
  void handleSdlEvent(const SDL_Event &evt);
  void update();

private:
  void buildVKeyMap();
  void buildMouseButtonMap();
  bool mouseMoved(const SDL_MouseMotionEvent &evt);
  bool mousePressed(const SDL_MouseButtonEvent &evt);
  bool mouseReleased(const SDL_MouseButtonEvent &evt);
  bool keyPressed(SDL_Keycode key, const SDL_TextInputEvent *evt);
  bool keyReleased(const SDL_KeyboardEvent &key);
  bool mouseWheelMoved(const SDL_MouseWheelEvent &evt);
  void computeMouseMove();
};
}

#endif /* SDLTOMYGUI_H */
