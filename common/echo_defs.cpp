#include "echo_defs.hpp"
#include <boost/json/conversion.hpp>
namespace js = ::boost::json;

EchoResponse tag_invoke(js::value_to_tag<EchoResponse>, const js::value& jv) {
	const auto& obj = jv.as_object();
	EchoResponse ret;
	ret.message = js::value_to<std::string>(obj.at("message"));
	return ret;
}

Error tag_invoke(js::value_to_tag<Error>, const js::value& jv) {
	const auto& obj = jv.as_object();
	Error ret;
	ret.code = js::value_to<int32_t>(obj.at("code"));
	ret.message = js::value_to<std::string>(obj.at("message"));
	ret.fields = js::value_to<std::string>(obj.at("fields"));
	return ret;
}

void tag_invoke(js::value_from_tag, js::value& jv, const EchoResponse& v) {
	jv = {
		{"message", js::value_from(v.message, jv.get_allocator())},
	};
}

void tag_invoke(js::value_from_tag, js::value& jv, const Error& v) {
	jv = {
		{"code", js::value_from(v.code, jv.get_allocator())},
		{"message", js::value_from(v.message, jv.get_allocator())},
		{"fields", js::value_from(v.fields, jv.get_allocator())},
	};
}
