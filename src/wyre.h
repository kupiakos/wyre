
#ifndef _WYRE_H_
#define _WYRE_H_

#include <vector>
#include <string>

#include "socket.h"
#include "wyre.pb.h"

namespace wyre {

class WyreClient {
private:
	wyre::socket _sock;
	void sendFile(proto::DataChunk & d, FILE *f, FILE *fcopy=nullptr);

public:
	void connect(const std::string & hostname, uint16_t port);

	void run(std::vector<std::string> &args);
	void push(std::vector<std::string> &args);

	wyre::socket & sock() { return _sock; }
};

} // namespace wyre

#endif
