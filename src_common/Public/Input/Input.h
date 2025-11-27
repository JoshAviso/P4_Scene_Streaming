#pragma once

#include <GLFW/glfw3.h>

#include <Common.hpp>
#include <Math/Vec2.h>

class Input
{
public:
	enum MouseMode { Normal, Locked, Hidden, Bounded };
	struct MouseData {
		MouseMode Mode = Normal;
		bool LeftDown = false;
		bool LeftPressed = false;
		bool LeftReleased = false;
		bool RightDown = false;
		bool RightPressed = false;
		bool RightReleased = false;
		Vec2 Position = { 0, 0 };
		Vec2 LastPosition = { 0, 0 };
		Vec2 Delta = { 0, 0 };
		float ScrollDelta = 0.f;
	};
	static MouseData Mouse;

	enum EKeyCode {
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Up, Down, Left,Right,PageUp,PageDown,Home,End,Insert,Delete,
		LCtrl,RCtrl,LShift,RShift,LAlt,RAlt,
		Space,Escape,Enter,Backspace,Tab,CapsLock,NumLock,ScrollLock,
		Numpad0,Numpad1,Numpad2,Numpad3,Numpad4,Numpad5,Numpad6,Numpad7,Numpad8,Numpad9,NumpadAdd,NumpadSubtract,NumpadMultiply,NumpadDivide,NumpadDecimal,NumpadEnter,
		PrintScreen,Pause,Menu,
		KEY_COUNT
	};
	static Dict<EKeyCode, bool> KeyDown;
	static Dict<EKeyCode, bool> KeyPressed;
	static Dict<EKeyCode, bool> KeyReleased;
	static EKeyCode LastUsedKey;

private:
	static void ResetInput();

public:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

// SINGLETON
public:
	static Input* Initialize();
private:
	static Input* _instance;
	explicit Input();
	~Input();
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

public:
	friend class Application;
};

