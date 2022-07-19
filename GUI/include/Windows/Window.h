#pragma once

#include <string>

class Window
{
public:
	std::string Name;

	bool IncludeInWindows = false;
	bool _Open = true;

	virtual void SetOpen(bool open) { _Open = open; }
	virtual void Open() { _Open = true; }
	virtual void Close() { _Open = false; }
	virtual bool IsOpen() { return _Open; }


	virtual void Init() {}
	virtual void SimulationStart() {}
	virtual void Render() {}
	virtual void Update() {}
	virtual void PreDestroy() {}
};