// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: wyre.proto

#include "wyre.pb.h"
#include "wyre.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace wyre {
namespace proto {

static const char* WyreService_method_names[] = {
  "/wyre.proto.WyreService/ServerSession",
  "/wyre.proto.WyreService/RunCommand",
  "/wyre.proto.WyreService/UploadFile",
};

std::unique_ptr< WyreService::Stub> WyreService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< WyreService::Stub> stub(new WyreService::Stub(channel));
  return stub;
}

WyreService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_ServerSession_(WyreService_method_names[0], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_RunCommand_(WyreService_method_names[1], ::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_UploadFile_(WyreService_method_names[2], ::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  {}

::grpc::ClientReader< ::wyre::proto::WyreCommand>* WyreService::Stub::ServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request) {
  return ::grpc::internal::ClientReaderFactory< ::wyre::proto::WyreCommand>::Create(channel_.get(), rpcmethod_ServerSession_, context, request);
}

::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>* WyreService::Stub::AsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::wyre::proto::WyreCommand>::Create(channel_.get(), cq, rpcmethod_ServerSession_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>* WyreService::Stub::PrepareAsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::wyre::proto::WyreCommand>::Create(channel_.get(), cq, rpcmethod_ServerSession_, context, request, false, nullptr);
}

::grpc::ClientWriter< ::wyre::proto::DataChunk>* WyreService::Stub::RunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
  return ::grpc::internal::ClientWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), rpcmethod_RunCommand_, context, response);
}

::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* WyreService::Stub::AsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), cq, rpcmethod_RunCommand_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* WyreService::Stub::PrepareAsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), cq, rpcmethod_RunCommand_, context, response, false, nullptr);
}

::grpc::ClientWriter< ::wyre::proto::DataChunk>* WyreService::Stub::UploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
  return ::grpc::internal::ClientWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), rpcmethod_UploadFile_, context, response);
}

::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* WyreService::Stub::AsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), cq, rpcmethod_UploadFile_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* WyreService::Stub::PrepareAsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::wyre::proto::DataChunk>::Create(channel_.get(), cq, rpcmethod_UploadFile_, context, response, false, nullptr);
}

WyreService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      WyreService_method_names[0],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< WyreService::Service, ::wyre::proto::StartServerSession, ::wyre::proto::WyreCommand>(
          std::mem_fn(&WyreService::Service::ServerSession), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      WyreService_method_names[1],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< WyreService::Service, ::wyre::proto::DataChunk, ::wyre::proto::Result>(
          std::mem_fn(&WyreService::Service::RunCommand), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      WyreService_method_names[2],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< WyreService::Service, ::wyre::proto::DataChunk, ::wyre::proto::Result>(
          std::mem_fn(&WyreService::Service::UploadFile), this)));
}

WyreService::Service::~Service() {
}

::grpc::Status WyreService::Service::ServerSession(::grpc::ServerContext* context, const ::wyre::proto::StartServerSession* request, ::grpc::ServerWriter< ::wyre::proto::WyreCommand>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status WyreService::Service::RunCommand(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status WyreService::Service::UploadFile(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace wyre
}  // namespace proto

