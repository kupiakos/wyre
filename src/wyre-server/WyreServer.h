#pragma once

#include "wyre.grpc.pb.h"
#include <functional>
#include <filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

namespace wyre {

class WyreServer : public wyre::proto::WyreService::Service {
public:
	WyreServer(
		std::ostream *reportFile,
		std::ostream *shaFile,
		const std::string & destDir);
	
	grpc::Status ServerSession(grpc::ServerContext* context, const proto::StartServerSession* request, grpc::ServerWriter<proto::WyreCommand>* writer);
	grpc::Status RunCommand(grpc::ServerContext* context, grpc::ServerReader<proto::DataChunk>* reader, proto::Result* response);
	grpc::Status UploadFile(grpc::ServerContext* context, grpc::ServerReader<proto::DataChunk>* reader, proto::Result* response);

private:
	enum class Source {
		FILE, COMMAND
	};

	grpc::Status writeDataChunks(grpc::ServerContext* context, grpc::ServerReader<proto::DataChunk> * reader, Source source);

	std::mutex fileWrite_;
	fs::path destDir_;
	std::ostream *reportFile_;
	std::ostream *shaFile_;
};

} // namespace wyre
