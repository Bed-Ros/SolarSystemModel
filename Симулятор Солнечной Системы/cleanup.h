#ifndef CLEANUP_H_
#define CLEANUP_H_

#include <functional>

using namespace std;

template <typename T>
class SDL_Cleanup
{
	using CleanupFunc = function<void(T*)>;
private:
	T*obj{ nullptr };
	CleanupFunc deleter;

public:
	SDL_Cleanup() : SDL_Cleanup([](T*) {}) {};
	SDL_Cleanup(const CleanupFunc& func) : deleter(func) {}
	~SDL_Cleanup() { clean(); }

	operator T*() const { return obj; }
	SDL_Cleanup& operator = (T*rhs) { clean(); obj = rhs; return *this; }

private:
	void clean() { if (obj) deleter(obj); obj = nullptr; };

};

#endif