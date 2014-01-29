#pragma once

#include <Windows.h>
#include <functional>


class Window;

class MessagePasserInterface
{
public:
	MessagePasserInterface(){};
	virtual ~MessagePasserInterface(){};

	virtual void operator ()(Window* wind, UINT Msg, WPARAM wParam, LPARAM lParam) = 0;
};

template<class T>
class MessagePasser : public MessagePasserInterface
{
public:
	MessagePasser(T& t, std::function<void(T&, Window* wind, UINT Msg, WPARAM wParam, LPARAM lParam)> func):target(&t),handler(func){};
	virtual ~MessagePasser(){};

	virtual void operator ()(Window* wind, UINT Msg, WPARAM wParam, LPARAM lParam){
		handler(*target, wind, Msg, wParam, lParam);
	}
private:
	std::function<void(T&, Window* wind, UINT Msg, WPARAM wParam, LPARAM lParam)> handler;
	T* target;
};