#ifndef _SAFE_FILE_H_
#define _SAFE_FILE_H_

#ifdef _WIN32
#include <Windows.h>
#endif
#include <stdio.h>
#include <type_traits>

namespace wyre {

template<typename ResourceType>
using deleter_t = void(*)(ResourceType);

template<typename T, T DefaultValue, deleter_t<T> Deleter>
class SafeResource {
public:
	typedef T ResourceType;

private:
	ResourceType r;

public:
	SafeResource() : r(DefaultValue) {}
	SafeResource(ResourceType r) : r(r) {}
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
	operator ResourceType &() { return r; }
	operator const ResourceType &() const { return r; }
	ResourceType & get() { return r; }
	const ResourceType & get() const { return r; }
	ResourceType * operator&() { return &r; }
	static const ResourceType defaultValue() { return DefaultValue; }

	ResourceType release() {
		ResourceType v = r;
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

void _fclose(FILE * f);
using SafeFile = SafeResource<FILE *, nullptr, _fclose>;

static_assert(std::is_move_assignable<SafeFile>::value, "SafeFile is not move-assignable");

#ifdef _WIN32
void _CloseHandle(HANDLE h);
using SafeHandle = SafeResource<HANDLE, INVALID_HANDLE_VALUE, _CloseHandle>;
#endif

}

#endif