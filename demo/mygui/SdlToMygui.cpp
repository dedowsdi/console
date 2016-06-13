#include "SdlToMygui.h"

namespace input {

//------------------------------------------------------------------------------
SdlToMygui::SdlToMygui()
    : mMouseX(0),
      mMouseY(0),
      mMouseZ(0),
      mMouseMove(false),
      mWidth(0),
      mHeight(0) {
  buildVKeyMap();
  buildMouseButtonMap();
}

//------------------------------------------------------------------------------
void SdlToMygui::handleSdlEvent(const SDL_Event &evt) {
  switch (evt.type) {
    case SDL_KEYDOWN:
      keyPressed(evt.key.keysym.sym, nullptr);
      break;
    case SDL_TEXTINPUT:
      keyPressed(evt.key.keysym.sym, &evt.text);
      break;
    case SDL_KEYUP:
      keyReleased(evt.key);
      break;
    // mouse events
    case SDL_MOUSEMOTION:
      mouseMoved(evt.motion);
      break;
    case SDL_MOUSEBUTTONDOWN:
      mousePressed(evt.button);
      break;
    case SDL_MOUSEBUTTONUP:
      mouseReleased(evt.button);
      break;
    case SDL_MOUSEWHEEL:
      mouseWheelMoved(evt.wheel);
      break;
    default:
      break;
  }
}

//------------------------------------------------------------------------------
void SdlToMygui::update() { computeMouseMove(); }

//------------------------------------------------------------------------------
void SdlToMygui::computeMouseMove() {
  if (mMouseMove) {
    MyGUI::InputManager::getInstance().injectMouseMove(
        mMouseX, mMouseY, mMouseZ);
    mMouseMove = false;
  }
}

//------------------------------------------------------------------------------
void SdlToMygui::buildVKeyMap() {
  mSDLVKeyMap.clear();
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_UNKNOWN, MyGUI::KeyCode::None));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_ESCAPE, MyGUI::KeyCode::Escape));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_1, MyGUI::KeyCode::One));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_2, MyGUI::KeyCode::Two));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_3, MyGUI::KeyCode::Three));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_4, MyGUI::KeyCode::Four));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_5, MyGUI::KeyCode::Five));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_6, MyGUI::KeyCode::Six));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_7, MyGUI::KeyCode::Seven));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_8, MyGUI::KeyCode::Eight));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_9, MyGUI::KeyCode::Nine));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_0, MyGUI::KeyCode::Zero));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_MINUS, MyGUI::KeyCode::Minus));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_EQUALS, MyGUI::KeyCode::Equals));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_BACKSPACE, MyGUI::KeyCode::Backspace));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_TAB, MyGUI::KeyCode::Tab));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_q, MyGUI::KeyCode::Q));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_w, MyGUI::KeyCode::W));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_e, MyGUI::KeyCode::E));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_r, MyGUI::KeyCode::R));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_t, MyGUI::KeyCode::T));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_y, MyGUI::KeyCode::Y));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_u, MyGUI::KeyCode::U));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_i, MyGUI::KeyCode::I));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_o, MyGUI::KeyCode::O));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_p, MyGUI::KeyCode::P));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_LEFTBRACKET, MyGUI::KeyCode::LeftBracket));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_RIGHTBRACKET, MyGUI::KeyCode::RightBracket));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RETURN, MyGUI::KeyCode::Return));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_LCTRL, MyGUI::KeyCode::LeftControl));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_a, MyGUI::KeyCode::A));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_s, MyGUI::KeyCode::S));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_d, MyGUI::KeyCode::D));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_f, MyGUI::KeyCode::F));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_g, MyGUI::KeyCode::G));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_h, MyGUI::KeyCode::H));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_j, MyGUI::KeyCode::J));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_k, MyGUI::KeyCode::K));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_l, MyGUI::KeyCode::L));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_SEMICOLON, MyGUI::KeyCode::Semicolon));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_QUOTEDBL, MyGUI::KeyCode::Apostrophe));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_BACKQUOTE, MyGUI::KeyCode::Grave));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_LSHIFT, MyGUI::KeyCode::LeftShift));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_BACKSLASH, MyGUI::KeyCode::Backslash));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_z, MyGUI::KeyCode::Z));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_x, MyGUI::KeyCode::X));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_c, MyGUI::KeyCode::C));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_v, MyGUI::KeyCode::V));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_b, MyGUI::KeyCode::B));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_n, MyGUI::KeyCode::N));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(SDLK_m, MyGUI::KeyCode::M));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_COMMA, MyGUI::KeyCode::Comma));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_PERIOD, MyGUI::KeyCode::Period));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_SLASH, MyGUI::KeyCode::Slash));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RSHIFT, MyGUI::KeyCode::RightShift));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_KP_MULTIPLY, MyGUI::KeyCode::Multiply));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_LALT, MyGUI::KeyCode::LeftAlt));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_SPACE, MyGUI::KeyCode::Space));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_CAPSLOCK, MyGUI::KeyCode::Capital));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F1, MyGUI::KeyCode::F1));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F2, MyGUI::KeyCode::F2));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F3, MyGUI::KeyCode::F3));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F4, MyGUI::KeyCode::F4));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F5, MyGUI::KeyCode::F5));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F6, MyGUI::KeyCode::F6));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F7, MyGUI::KeyCode::F7));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F8, MyGUI::KeyCode::F8));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F9, MyGUI::KeyCode::F9));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F10, MyGUI::KeyCode::F10));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_NUMLOCKCLEAR, MyGUI::KeyCode::NumLock));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_SCROLLLOCK, MyGUI::KeyCode::ScrollLock));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_7, MyGUI::KeyCode::Numpad7));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_8, MyGUI::KeyCode::Numpad8));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_9, MyGUI::KeyCode::Numpad9));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_MINUS, MyGUI::KeyCode::Subtract));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_4, MyGUI::KeyCode::Numpad4));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_5, MyGUI::KeyCode::Numpad5));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_6, MyGUI::KeyCode::Numpad6));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_PLUS, MyGUI::KeyCode::Add));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_1, MyGUI::KeyCode::Numpad1));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_2, MyGUI::KeyCode::Numpad2));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_3, MyGUI::KeyCode::Numpad3));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_0, MyGUI::KeyCode::Numpad0));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_PERIOD, MyGUI::KeyCode::Decimal));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::OEM_102));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F11, MyGUI::KeyCode::F11));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F12, MyGUI::KeyCode::F12));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F13, MyGUI::KeyCode::F13));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F14, MyGUI::KeyCode::F14));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_F15, MyGUI::KeyCode::F15));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::Kana));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::ABNT_C1));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Convert));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::NoConvert));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::Yen));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::ABNT_C2));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_KP_EQUALS, MyGUI::KeyCode::NumpadEquals));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::PrevTrack));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::At));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Colon));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Underline));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Kanji));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::Stop));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::AX));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Unlabeled));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::NextTrack));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_KP_DIVIDE, MyGUI::KeyCode::NumpadEnter));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RCTRL, MyGUI::KeyCode::RightControl));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::Mute));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::Calculator));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::PlayPause));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::MediaStop));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::VolumeDown));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::VolumeUp));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::WebHome));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::NumpadComma));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_KP_DIVIDE, MyGUI::KeyCode::Divide));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_SYSREQ, MyGUI::KeyCode::SysRq));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RALT, MyGUI::KeyCode::RightAlt));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_PAUSE, MyGUI::KeyCode::Pause));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_HOME, MyGUI::KeyCode::Home));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_UP, MyGUI::KeyCode::ArrowUp));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_PAGEUP, MyGUI::KeyCode::PageUp));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_LEFT, MyGUI::KeyCode::ArrowLeft));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RIGHT, MyGUI::KeyCode::ArrowRight));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_END, MyGUI::KeyCode::End));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_DOWN, MyGUI::KeyCode::ArrowDown));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_PAGEDOWN, MyGUI::KeyCode::PageDown));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_INSERT, MyGUI::KeyCode::Insert));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_DELETE, MyGUI::KeyCode::Delete));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_LGUI, MyGUI::KeyCode::LeftWindows));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(,
  // MyGUI::KeyCode::RightWindow));
  mSDLVKeyMap.insert(
      std::pair<int, MyGUI::KeyCode>(SDLK_RGUI, MyGUI::KeyCode::RightWindows));
  mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(
      SDLK_APPLICATION, MyGUI::KeyCode::AppMenu));
  // mSDLVKeyMap.insert(std::pair<int, MyGUI::KeyCode>(, MyGUI::KeyCode::));
}

//------------------------------------------------------------------------------
void SdlToMygui::buildMouseButtonMap() {
  mSDLMouseMap.clear();
  mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(
      SDL_BUTTON_LEFT, MyGUI::MouseButton::Left));
  mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(
      SDL_BUTTON_RIGHT, MyGUI::MouseButton::Right));
  mSDLMouseMap.insert(std::pair<int, MyGUI::MouseButton>(
      SDL_BUTTON_MIDDLE, MyGUI::MouseButton::Middle));
}

//------------------------------------------------------------------------------
bool SdlToMygui::mouseMoved(const SDL_MouseMotionEvent &evt) {
  mMouseX = evt.x;
  mMouseY = evt.y;
  mMouseMove = true;
  return true;
}

//------------------------------------------------------------------------------
bool SdlToMygui::mousePressed(const SDL_MouseButtonEvent &evt) {
  computeMouseMove();
  MyGUI::InputManager::getInstance().injectMousePress(
      mMouseX, mMouseY, mSDLMouseMap[evt.button]);
  return true;
}

//------------------------------------------------------------------------------
bool SdlToMygui::mouseReleased(const SDL_MouseButtonEvent &evt) {
  computeMouseMove();
  MyGUI::InputManager::getInstance().injectMouseRelease(
      mMouseX, mMouseY, mSDLMouseMap[evt.button]);
  return true;
}

//------------------------------------------------------------------------------
bool SdlToMygui::keyPressed(SDL_Keycode key, const SDL_TextInputEvent *evt) {
  if (mSDLVKeyMap.count(key) == 0) {
    return false;
  }
  MyGUI::KeyCode myGuiKeyCode = mSDLVKeyMap[key];
  if (evt == nullptr) {
    MyGUI::InputManager::getInstance().injectKeyPress(myGuiKeyCode, NULL);
  } else {
    MyGUI::UString ustring(evt->text);
    MyGUI::UString::utf32string utf32string = ustring.asUTF32();
    for (MyGUI::UString::utf32string::const_iterator it = utf32string.begin();
         it != utf32string.end(); ++it) {
      MyGUI::InputManager::getInstance().injectKeyPress(myGuiKeyCode, *it);
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool SdlToMygui::keyReleased(const SDL_KeyboardEvent &key) {
  if (mSDLVKeyMap.count(key.keysym.sym) == 0) {
    return false;
  }
  MyGUI::InputManager::getInstance().injectKeyRelease(
      mSDLVKeyMap[key.keysym.sym]);
  return true;
}

//------------------------------------------------------------------------------
bool SdlToMygui::mouseWheelMoved(const SDL_MouseWheelEvent &evt) {
  mMouseZ += evt.y;
  mMouseMove = true;
  return true;
}
}
