#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <memory>

#include "echo_defs.hpp"

class ClientBase : public std::enable_shared_from_this<ClientBase> {
public:
	using request = ::boost::beast::http::request<::boost::beast::http::string_body>;
	using response = ::boost::beast::http::response<::boost::beast::http::string_body>;
	using protocol = ::boost::asio::ip::tcp;
	using ec_t = ::boost::beast::error_code;

	struct Config {
		std::chrono::milliseconds connect_timeout;
		std::chrono::milliseconds write_timeout;
		std::chrono::milliseconds read_timeout;
	};

	ClientBase(::boost::asio::io_context&, Config = {});
	ClientBase(const ClientBase&) = delete;
	ClientBase(ClientBase&&) = default;
	virtual ~ClientBase() { stop(); }

	void start(const ::boost::asio::ip::address&, uint16_t);
	void start(const protocol::endpoint&);
	void stop();

protected:
	Config _conf;
	::boost::asio::ip::tcp::resolver _resolver;
	::boost::beast::flat_buffer _buffer;
	::boost::beast::tcp_stream _stream;
	response _response;

	unsigned char _json_buffer[1024 + 256 + 128];

	void on_resolve(const ec_t&, protocol::resolver::results_type);
	void on_connect(const ec_t&, protocol::resolver::endpoint_type);
	void on_write(const ec_t&, size_t);
	void on_read(const ec_t&, size_t);
};
