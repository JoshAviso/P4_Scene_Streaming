#pragma once

#include <String.hpp>

class IResource {
protected:
	virtual bool LoadFromFile(String filepath) = 0;
	friend class ResourceManager;
};