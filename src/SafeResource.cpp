
#include "SafeResource.h"

namespace wyre {
void _fclose(FILE * f) { fclose(f); }
#ifdef _WIN32
void _CloseHandle(HANDLE h) { CloseHandle(h); }
#endif
}
