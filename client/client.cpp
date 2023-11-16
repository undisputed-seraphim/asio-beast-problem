#include <iostream>
#include <memory>

#include "echo_client.hpp"
#include "echo_defs.hpp"

namespace http = ::boost::beast::http;

class Client final : public ClientBase {
public:
	using ClientBase::ClientBase;
	using ClientBase::shared_from_this;

	void echo(std::string headerParam, std::string message) {
		constexpr std::string_view path = "/v1/echo?message=";
		::boost::json::monotonic_resource json_rsc(_json_buffer);
		request req;
		req.target(std::string(path) + message);
		req.method(http::verb::get);
		req.set("headerParam", headerParam);
		http::async_write(_stream, req, std::bind_front(&Client::on_write, this));
		// 200	 EchoResponse
		// default	 Error
	}
};

int main(int argc, char* argv[]) try {
	const auto address = boost::asio::ip::address::from_string("127.0.0.1");
	::boost::asio::io_context ctx;
	auto client = std::make_shared<Client>(ctx);
	client->start(Client::protocol::endpoint(address, 8080));
	ctx.run();
	client->echo("test message", "query");
	ctx.run();
} catch (const std::exception& e) {
	std::cerr << "Caught exception " << e.what() << std::endl;
}