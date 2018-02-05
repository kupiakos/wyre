
#ifndef _WYRE_H_
#define _WYRE_H_

#include <vector>
#include <string>

namespace wyre {

class network_error : public std::runtime_error {
	int _errorCode;
public:
	network_error(const char *message, int errorCode) :
		std::runtime_error(message),
		_errorCode(errorCode) {}
	int errorCode() const { return _errorCode; }
};

void run(std::vector<std::string> &args);
void push(std::vector<std::string> &args);

} // namespace wyre

#endif
