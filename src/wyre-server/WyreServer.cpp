#include "WyreServer.h"

#include <filesystem>
#include <fstream>

#include "time_util.h"
#include "path_util.h"
#include "SHA1.h"

using namespace ::wyre::proto;
using ::grpc::Status;
using ::grpc::StatusCode;

namespace wyre {

WyreServer::WyreServer(
		std::ostream * reportFile,
		std::ostream * shaFile,
		const std::string & destDir) :
		reportFile_(reportFile), shaFile_(shaFile), destDir_(destDir) {
	if (!fs::is_directory(destDir)) {
		fs::create_directory(destDir);
	}
}

Status WyreServer::ServerSession(
		grpc::ServerContext * context,
		const StartServerSession * request,
		grpc::ServerWriter<WyreCommand>* writer) {
	// TODO
	return Status::OK;
}

Status WyreServer::RunCommand(
		grpc::ServerContext * context,
		grpc::ServerReader<DataChunk>* reader,
		Result *) {
	return writeDataChunks(context, reader, Source::COMMAND);
}

Status WyreServer::UploadFile(
		grpc::ServerContext * context,
		grpc::ServerReader<DataChunk>* reader,
		Result *) {
	return writeDataChunks(context, reader, Source::FILE);
}

grpc::Status WyreServer::writeDataChunks(
		grpc::ServerContext * context,
		grpc::ServerReader<proto::DataChunk>* reader,
		Source source) {
	if (!reportFile_ || !shaFile_) {
		return Status(StatusCode::INVALID_ARGUMENT, "Output file null");
	}
	DataChunk d;
	if (!reader->Read(&d)) {
		return Status(StatusCode::RESOURCE_EXHAUSTED, "Empty chunk stream");
	}
	auto description = d.description();
	if (description.empty()) {
		return Status(StatusCode::INVALID_ARGUMENT,
			"Chunk description empty");
	}
	fs::path dest = destDir_;
	if (source == Source::FILE) {
		auto slash = description.find_last_of("/\\");
		if (slash == std::string::npos) {
			slash = 0;
		}
		dest /= description.substr(slash);
	} else {
		dest /= wyre::escapeFile(description);
	}

	std::fstream destFile;
	{
		std::unique_lock<std::mutex> lock(fileWrite_);
		if (fs::exists(dest)) {
			dest += ".0";
		}
		for (int i = 1; fs::exists(dest); ++i) {
			dest.replace_extension(std::to_string(i));
		}
		destFile.open(dest, std::ios::out | std::ios::binary);
		if (!destFile) {
			return Status(StatusCode::DATA_LOSS, "Could not open destFile");
		}

		// TODO: make generic
		std::cout << "[" << context->peer() << "] " << description
			<< " -> " << dest << std::endl;
		
		*reportFile_ << wyre::dateTimeToString(
			wyre::now(), "###### %F %T %z%n");
		if (source == Source::FILE) {
			*reportFile_
				<< "Saved file '" << description << "'\n\n"
				<< "Impact: filesystem, cache\n\n";
		} else {
			*reportFile_
				<< "Ran command '" << description << "'\n\n"
				<< "Impact: new process, TODO\n\n";
		}
		*reportFile_
			<< "Saved at " << dest << ".\n"
			<< std::endl;
		reportFile_->flush();
	}
	
	SHA1 sha;
	do {
		destFile << d.data();
		sha.update(d.data());
	} while (reader->Read(&d));

	auto digest = sha.hexdigest();
	if (!d.finalhash().empty()) {
		if (d.finalhash() != digest) {
			return Status(StatusCode::DATA_LOSS, "Checksum failure!");
		}
	}

	return Status::OK;
}

} // namespace wyre
