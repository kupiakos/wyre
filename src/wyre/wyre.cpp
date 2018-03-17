#include "wyre.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <filesystem>

#include "wyre.h"
#include "SafeResource.h"
#include "ChildProcess.h"
#include "proto_util.h"
#include "network_util.h"

#include "SHA1.h"
#ifdef _WIN32
#include "win32/winunicode.h"
#endif

#include "wyre.grpc.pb.h"

namespace wyre {

using grpc::Status;
namespace fs = std::experimental::filesystem;
constexpr size_t BUF_SIZE = 32768;

inline FILE * utf8Open(const char * filename, const char * mode) {
#ifdef _WIN32
	FILE *f;
	auto err =_wfopen_s(
		&f,
		unicode::utf8ToUtf16(filename).c_str(),
		unicode::utf8ToUtf16(mode).c_str());
	if (err) {
		throw std::system_error(
			std::make_error_code(static_cast<std::errc>(err)), filename);
	}
	return f;
#else
	return std::open(filename, mode);
#endif
}


void WyreClient::sendFile(
		std::unique_ptr<grpc::ClientWriter<proto::DataChunk>> writer,
		proto::DataChunk && d, FILE * inputFile, FILE * outputCopy) {
	if (!inputFile || !writer) { return; }

	SHA1 sha;
	auto data = std::make_unique<char[]>(BUF_SIZE);

	do {
		size_t nRead = 0;
		if (!std::feof(inputFile)) {
			nRead = std::fread(data.get(), 1, BUF_SIZE, inputFile);
		}
		if (nRead > 0) {
			sha.update(data.get(), nRead);
			d.set_data(data.get(), nRead);
			if (outputCopy) {
				std::fwrite(data.get(), 1, nRead, outputCopy);
			}
		} else if (nRead < 0) {
			throw std::runtime_error("Error reading");
		} else {
			continue;
		}
		if (std::feof(inputFile)) {
			d.set_finalhash(sha.hexdigest());
		}
		if (!writer->Write(d)) {
			break;  // broken stream
		}
	} while (!std::feof(inputFile));
	writer->WritesDone();
	Status status = writer->Finish();
	if (!status.ok()) {
		throw std::runtime_error("Send failed");
	}
}

void WyreClient::run(std::vector<std::string>& args) {
	ChildProcess p;
	p.withArgv(args).useStdout().run();
	auto processOut = p.stdout_();
	if (!processOut) { throw std::runtime_error("stdout not defined"); }
	
	grpc::ClientContext ctx;
	proto::Result res;
	proto::DataChunk d;
	d.set_description(p.cmdLine());

	std::cerr << "Running " << p.cmdLine() << std::endl;
	sendFile(stub_->RunCommand(&ctx, &res), std::move(d), processOut, stdout);
}

void WyreClient::push(std::vector<std::string>& args) {

	grpc::ClientContext ctx;
	for (const auto & fname : args) {
		proto::DataChunk d;
		proto::Result res;
		SafeFile f = utf8Open(fname.c_str(), "rb");
		if (!f) {
			throw std::runtime_error("Could not open file " + fname);
		}
		d.set_description(fs::canonical(fname).u8string());
		sendFile(stub_->UploadFile(&ctx, &res), std::move(d), f);
	}
}


} // namespace wyre
