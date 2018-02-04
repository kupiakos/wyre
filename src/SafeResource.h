#ifndef _SAFE_FILE_H_
#define _SAFE_FILE_H_

#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdio.h>
#include <type_traits>

namespace wyre {

template<typename T>
using deleter_t = void(*)(T);

namespace {
void _fclose(FILE * f) { fclose(f); }
}

template<typename T, T DefaultValue, deleter_t<T> Deleter>
class SafeResource {
private:
	T r;

public:
	SafeResource() : r(DefaultValue) {}
	SafeResource(T r) : r(r) {}
	~SafeResource() {
		close();
	}

	friend void swap(SafeResource & first, SafeResource & second) {
		using std::swap;
		swap(first.r, second.r);
	}

	SafeResource(SafeResource &) = delete;
	SafeResource(SafeResource && other) : SafeResource() {
		swap(*this, other);
	}
	SafeResource & operator=(SafeResource other) {
		swap(*this, other);
		return *this;
	}

	operator bool() { return r != DefaultValue; }
	operator T &() { return r; }
	operator const T &() const { return r; }
	T & get() { return r; }
	const T & get() const { return r; }
	T * operator&() { return &r; }

	T release() {
		T v = r;
		r = DefaultValue;
		return v;
	}

	void close() {
		if (r != DefaultValue) {
			Deleter(r);
			r = DefaultValue;
		}
	}
};

using SafeFile = SafeResource<FILE *, nullptr, _fclose>;

static_assert(std::is_move_assignable<SafeFile>::value, "SafeFile is not move-assignable");

#ifdef _WIN32

namespace {
void _CloseHandle(HANDLE h) { CloseHandle(h); }
}
using SafeHandle = SafeResource<HANDLE, INVALID_HANDLE_VALUE, _CloseHandle>;

#endif

}

#endif