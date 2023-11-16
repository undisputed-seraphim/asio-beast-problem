// Automatically generated from tests/echo.json. Do not modify this file.
#pragma once

#include <boost/json.hpp>
#include <string>

using namespace std::literals;

struct EchoResponse {
	std::string message;
};

struct Error {
	int32_t code;
	std::string message;
	std::string fields;
};

// Success
using get__echo_200_body = EchoResponse;
// Error
using get__echo_default_body = Error;

EchoResponse tag_invoke(::boost::json::value_to_tag<EchoResponse>, const ::boost::json::value& jv);
Error tag_invoke(::boost::json::value_to_tag<Error>, const ::boost::json::value& jv);
void tag_invoke(::boost::json::value_from_tag, ::boost::json::value& jv, const EchoResponse& v);
void tag_invoke(::boost::json::value_from_tag, ::boost::json::value& jv, const Error& v);