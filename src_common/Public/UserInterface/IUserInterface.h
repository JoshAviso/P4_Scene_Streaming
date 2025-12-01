#pragma once

#include <String.hpp>
#include "imgui.h"

class IUserInterface {
public:
	String Name;
	IUserInterface(const String name) : Name(name){};
protected:
	virtual void Render() = 0;
	friend class UserInterfaceManager;
};