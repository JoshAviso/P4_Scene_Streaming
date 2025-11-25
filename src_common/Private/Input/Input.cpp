#include <Input/Input.h>

#include <Application.h>
#include <Logger.hpp>

void Input::ResetInput()
{
    Mouse.LeftPressed = false;
    Mouse.LeftReleased = false;
    Mouse.RightPressed = false;
    Mouse.RightReleased = false;
    Mouse.Delta = { 0.f, 0.f };

    for (int i = 0; i < KEY_COUNT; i++) {
        KeyPressed[(EKeyCode)i] = false;
        KeyReleased[(EKeyCode)i] = false;
    }
}

Input::EKeyCode Input::LastUsedKey = Input::EKeyCode::KEY_COUNT;
Dict<Input::EKeyCode, bool> Input::KeyDown;
Dict<Input::EKeyCode, bool> Input::KeyPressed;
Dict<Input::EKeyCode, bool> Input::KeyReleased;
Input::MouseData Input::Mouse = Input::MouseData();
void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{;
    EKeyCode keycode;
    switch (key) {
    case GLFW_KEY_Q: keycode = Q; break;
    case GLFW_KEY_E: keycode = E; break;
    case GLFW_KEY_W: keycode = W; break;
    case GLFW_KEY_A: keycode = A; break;
    case GLFW_KEY_S: keycode = S; break;
    case GLFW_KEY_D: keycode = D; break;
    default: keycode = KEY_COUNT; break;
    }

    if (keycode == KEY_COUNT) return;

    bool pressed = action == GLFW_PRESS;
    bool released = action == GLFW_RELEASE;

    LastUsedKey = keycode;
    KeyDown[keycode] = pressed ? true : released ? false : KeyDown[keycode];
    KeyPressed[keycode] = pressed ? true : KeyPressed[keycode];
    KeyReleased[keycode] = released ? true : KeyReleased[keycode];
}

void Input::CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Vec2 newPos = {xpos, ypos};
    Mouse.Delta = newPos - Mouse.Position;
    Mouse.LastPosition = Mouse.Position;
    Mouse.Position = newPos;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    bool isPressed = false;
    if (action == GLFW_PRESS) isPressed = true;
    if (action != GLFW_RELEASE) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (!Mouse.LeftDown && isPressed) Mouse.LeftPressed = true;
        if (Mouse.LeftDown && !isPressed) Mouse.LeftReleased = true;
        Mouse.LeftDown = isPressed;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (!Mouse.LeftDown && isPressed) Mouse.RightPressed = true;
        if (Mouse.LeftDown && !isPressed) Mouse.RightReleased = true;
        Mouse.RightDown = isPressed;
    }
}

// SINGLETON 
Input* Input::_instance = nullptr;
Input* Input::Initialize()
{
    if (_instance != nullptr) return _instance;

    _instance = new Input();
    return _instance;
}

Input::Input()
{

}

Input::~Input() {}
