#include <iostream>

#include "echo_defs.hpp"
#include "echo_server.hpp"

class Server final : public ServerBase {
public:
	// Inherit all constructors
	using ServerBase::ServerBase;

	void handle_request(const request req, Session::Ptr session) override {
		std::cout << "session " << session->id() << ": " << req.target() << std::endl;
	}
};

int main(int argc, char* argv[]) try {
	const auto address = boost::asio::ip::address::from_string("127.0.0.1");
	::boost::asio::io_context ctx;
	Server server(ctx);
	server.start(ctx, Server::protocol::endpoint(address, 8080));
	std::cout << "Starting server..." << std::endl;
	std::thread t([&ctx]() { ctx.run(); });
	ctx.run();
	t.join();
} catch (const std::exception& e) {
	std::cerr << "Caught exception " << e.what() << std::endl;
}