#pragma once
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

class ServerBase {
public:
	using request = ::boost::beast::http::request<::boost::beast::http::string_body>;
	using response = ::boost::beast::http::response<::boost::beast::http::string_body>;
	using protocol = ::boost::asio::ip::tcp;
	using ec_t = ::boost::system::error_code;

	struct Config {};

	class Session : public std::enable_shared_from_this<Session> {
	public:
		using Ptr = std::shared_ptr<Session>;
		Session(ServerBase&, protocol::socket, Config, uint64_t);
		~Session() noexcept;

		void run();
		uint64_t id() const { return _id; }

	protected:
		friend ServerBase;

		ServerBase& _parent;
		::boost::beast::tcp_stream _stream;
		::boost::beast::flat_buffer _buffer;
		request _request;
		Config _config;
		uint64_t _id;

		void do_read();
		void on_read(ec_t ec, std::size_t bytes);
		void do_close();
	};

	ServerBase(boost::asio::io_context&, Config = {});

	void start(boost::asio::io_context&, const ::boost::asio::ip::address, uint16_t);
	void start(boost::asio::io_context&, const protocol::endpoint&);

	virtual void handle_request(const request, Session::Ptr) = 0;

protected:
	Config _conf;
	protocol::acceptor _acceptor;
	std::atomic<uint64_t> _client_id{0};

	void on_accept(boost::asio::io_context& ctx, const ec_t& ec, protocol::socket socket);
};
