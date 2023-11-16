#include <boost/json.hpp>
#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>

#include "echo_client.hpp"

namespace asio = ::boost::asio;
namespace http = ::boost::beast::http;
namespace json = ::boost::json;
using namespace std::literals;
using request = ::boost::beast::http::request<::boost::beast::http::string_body>;
using response = ::boost::beast::http::response<::boost::beast::http::string_body>;

void fail(std::string_view facility, ::boost::system::error_code ec) {
	std::cerr << facility << ": " << ec.to_string() << ' ' << ec.message() << std::endl;
}

ClientBase::ClientBase(asio::io_context& ctx, Config config)
	: _conf(std::move(config))
	, _resolver(asio::make_strand(ctx))
	, _stream(asio::make_strand(ctx)) {}

void ClientBase::start(const asio::ip::address& address, uint16_t port) { start(protocol::endpoint(address, port)); }
void ClientBase::start(const protocol::endpoint& endpoint) {
	_resolver.async_resolve(endpoint, std::bind_front(&ClientBase::on_resolve, shared_from_this()));
}

void ClientBase::stop() {
	_resolver.cancel();
	_stream.close();
}

void ClientBase::on_resolve(const ec_t& ec, protocol::resolver::results_type results) {
	if (ec) {
		return fail("on_resolve", ec);
	}
	_stream.async_connect(results, std::bind_front(&ClientBase::on_connect, shared_from_this()));
}

void ClientBase::on_connect(const ec_t& ec, protocol::resolver::endpoint_type endpoint) {
	if (ec) {
		return fail("on_connect", ec);
	}
}

void ClientBase::on_write(const ec_t& ec, size_t) {
	if (ec) {
		return fail("on_write", ec);
	}
	http::async_read(_stream, _buffer, _response, std::bind_front(&ClientBase::on_read, shared_from_this()));
}

void ClientBase::on_read(const ec_t& ec, size_t) {
	if (ec) {
		return fail("on_read", ec);
	}
	http::async_read(_stream, _buffer, _response, std::bind_front(&ClientBase::on_read, shared_from_this()));
}