
#ifndef _WYRE_H_
#define _WYRE_H_

#include <vector>
#include <string>

#include "wyre.pb.h"
#include "wyre.grpc.pb.h"
#include <grpc++/channel.h>

namespace wyre {

class WyreClient {
public:
	WyreClient(std::shared_ptr<grpc::Channel> channel) :
		stub_(proto::WyreService::NewStub(channel)) {}
	void run(std::vector<std::string> &args);
	void push(std::vector<std::string> &args);

private:
	void sendFile(std::unique_ptr<grpc::ClientWriter<proto::DataChunk>> writer,
		proto::DataChunk && d, FILE *inputFile, FILE *outputCopy = nullptr);
	std::unique_ptr<proto::WyreService::Stub> stub_;
};

} // namespace wyre

#endif
