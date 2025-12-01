#pragma once

#include <UserInterface/IUserInterface.h>
#include <Common.hpp>
#include <String.hpp>

class UserInterfaceManager {
public:
	struct Desc {
		bool isServer;
	};

public:
	static Shared<IUserInterface> AddUI(IUserInterface* ui);
	static Shared<IUserInterface> GetUI(const String name);
	static void RemoveUI(const String name);

private:
	List<Shared<IUserInterface>> _userInterfaces;
	static void BeginFrame();
	static void Render();

	// SINGLETON
public:
	static UserInterfaceManager* Initialize(const Desc desc);
private:
	static UserInterfaceManager* _instance;
	explicit UserInterfaceManager(const Desc& desc);
	~UserInterfaceManager();
	UserInterfaceManager& operator= (const UserInterfaceManager&) = delete;
	UserInterfaceManager(const UserInterfaceManager&) = delete;

	friend class GraphicsSystem;
};