// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: wyre.proto
#ifndef GRPC_wyre_2eproto__INCLUDED
#define GRPC_wyre_2eproto__INCLUDED

#include "wyre.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace wyre {
namespace proto {

class WyreService final {
 public:
  static constexpr char const* service_full_name() {
    return "wyre.proto.WyreService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderInterface< ::wyre::proto::WyreCommand>> ServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::wyre::proto::WyreCommand>>(ServerSessionRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>> AsyncServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>>(AsyncServerSessionRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>> PrepareAsyncServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>>(PrepareAsyncServerSessionRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>> RunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>>(RunCommandRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>> AsyncRunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>>(AsyncRunCommandRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>> PrepareAsyncRunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>>(PrepareAsyncRunCommandRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>> UploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>>(UploadFileRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>> AsyncUploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>>(AsyncUploadFileRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>> PrepareAsyncUploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>>(PrepareAsyncUploadFileRaw(context, response, cq));
    }
  private:
    virtual ::grpc::ClientReaderInterface< ::wyre::proto::WyreCommand>* ServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>* AsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::wyre::proto::WyreCommand>* PrepareAsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>* RunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>* AsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>* PrepareAsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientWriterInterface< ::wyre::proto::DataChunk>* UploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>* AsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::wyre::proto::DataChunk>* PrepareAsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReader< ::wyre::proto::WyreCommand>> ServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::wyre::proto::WyreCommand>>(ServerSessionRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>> AsyncServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>>(AsyncServerSessionRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>> PrepareAsyncServerSession(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>>(PrepareAsyncServerSessionRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientWriter< ::wyre::proto::DataChunk>> RunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::wyre::proto::DataChunk>>(RunCommandRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>> AsyncRunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>>(AsyncRunCommandRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>> PrepareAsyncRunCommand(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>>(PrepareAsyncRunCommandRaw(context, response, cq));
    }
    std::unique_ptr< ::grpc::ClientWriter< ::wyre::proto::DataChunk>> UploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::wyre::proto::DataChunk>>(UploadFileRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>> AsyncUploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>>(AsyncUploadFileRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>> PrepareAsyncUploadFile(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>>(PrepareAsyncUploadFileRaw(context, response, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientReader< ::wyre::proto::WyreCommand>* ServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request) override;
    ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>* AsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReader< ::wyre::proto::WyreCommand>* PrepareAsyncServerSessionRaw(::grpc::ClientContext* context, const ::wyre::proto::StartServerSession& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientWriter< ::wyre::proto::DataChunk>* RunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) override;
    ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* AsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* PrepareAsyncRunCommandRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientWriter< ::wyre::proto::DataChunk>* UploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response) override;
    ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* AsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncWriter< ::wyre::proto::DataChunk>* PrepareAsyncUploadFileRaw(::grpc::ClientContext* context, ::wyre::proto::Result* response, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_ServerSession_;
    const ::grpc::internal::RpcMethod rpcmethod_RunCommand_;
    const ::grpc::internal::RpcMethod rpcmethod_UploadFile_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status ServerSession(::grpc::ServerContext* context, const ::wyre::proto::StartServerSession* request, ::grpc::ServerWriter< ::wyre::proto::WyreCommand>* writer);
    virtual ::grpc::Status RunCommand(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response);
    virtual ::grpc::Status UploadFile(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_ServerSession : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_ServerSession() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_ServerSession() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerSession(::grpc::ServerContext* context, const ::wyre::proto::StartServerSession* request, ::grpc::ServerWriter< ::wyre::proto::WyreCommand>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestServerSession(::grpc::ServerContext* context, ::wyre::proto::StartServerSession* request, ::grpc::ServerAsyncWriter< ::wyre::proto::WyreCommand>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(0, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_RunCommand : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_RunCommand() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_RunCommand() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunCommand(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestRunCommand(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::wyre::proto::Result, ::wyre::proto::DataChunk>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(1, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_UploadFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_UploadFile() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_UploadFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UploadFile(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestUploadFile(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::wyre::proto::Result, ::wyre::proto::DataChunk>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(2, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_ServerSession<WithAsyncMethod_RunCommand<WithAsyncMethod_UploadFile<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_ServerSession : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_ServerSession() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_ServerSession() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status ServerSession(::grpc::ServerContext* context, const ::wyre::proto::StartServerSession* request, ::grpc::ServerWriter< ::wyre::proto::WyreCommand>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_RunCommand : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_RunCommand() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_RunCommand() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status RunCommand(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_UploadFile : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_UploadFile() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_UploadFile() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status UploadFile(::grpc::ServerContext* context, ::grpc::ServerReader< ::wyre::proto::DataChunk>* reader, ::wyre::proto::Result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  typedef Service StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_ServerSession : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithSplitStreamingMethod_ServerSession() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::SplitServerStreamingHandler< ::wyre::proto::StartServerSession, ::wyre::proto::WyreCommand>(std::bind(&WithSplitStreamingMethod_ServerSession<BaseClass>::StreamedServerSession, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_ServerSession() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status ServerSession(::grpc::ServerContext* context, const ::wyre::proto::StartServerSession* request, ::grpc::ServerWriter< ::wyre::proto::WyreCommand>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedServerSession(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::wyre::proto::StartServerSession,::wyre::proto::WyreCommand>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_ServerSession<Service > SplitStreamedService;
  typedef WithSplitStreamingMethod_ServerSession<Service > StreamedService;
};

}  // namespace proto
}  // namespace wyre


#endif  // GRPC_wyre_2eproto__INCLUDED