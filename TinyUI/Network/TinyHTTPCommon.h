#pragma once
#include "../Common/TinyCommon.h"
using namespace TinyUI;

namespace TinyUI
{
	namespace Network
	{
		enum HttpCode
		{
			HTTP_CODE_OK = 200,
			HTTP_CODE_NON_AUTHORITATIVE = 203,
			HTTP_CODE_NO_CONTENT = 204,
			HTTP_CODE_PARTIAL_CONTENT = 206,

			HTTP_CODE_MULTIPLE_CHOICES = 300,
			HTTP_CODE_MOVED_PERMANENTLY = 301,
			HTTP_CODE_FOUND = 302,
			HTTP_CODE_SEE_OTHER = 303,
			HTTP_CODE_NOT_MODIFIED = 304,
			HTTP_CODE_MOVED_TEMPORARILY = 307,

			HTTP_CODE_BAD_REQUEST = 400,
			HTTP_CODE_UNAUTHORIZED = 401,
			HTTP_CODE_FORBIDDEN = 403,
			HTTP_CODE_NOT_FOUND = 404,
			HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = 407,
			HTTP_CODE_GONE = 410,

			HTTP_CODE_INTERNAL_SERVER_ERROR = 500,
			HTTP_CODE_NOT_IMPLEMENTED = 501,
			HTTP_CODE_SERVICE_UNAVAILABLE = 503,
		};

		enum HttpVersion
		{
			HTTP_VERSION_1_0,
			HTTP_VERSION_1_1,
			HTTP_VERSION_UNKNOWN,
			HTTP_VERSION_LAST = HTTP_VERSION_UNKNOWN
		};

		enum HttpVerb
		{
			HTTP_VERB_GET,
			HTTP_VERB_POST,
			HTTP_VERB_PUT,
			HTTP_VERB_DELETE,
			HTTP_VERB_CONNECT,
			HTTP_VERB_HEAD,
			HTTP_VERB_LAST = HTTP_VERB_HEAD
		};

		enum HttpError
		{
			HTTP_ERROR_NONE,
			HTTP_ERROR_PROTOCOL,
			HTTP_ERROR_DISCONNECTED,
			HTTP_ERROR_OVERFLOW,
			HTTP_ERROR_CONNECT_FAILED,
			HTTP_ERROR_SOCKET_ERROR,
			HTTP_ERROR_SHUTDOWN,
			HTTP_ERROR_OPERATION_CANCELLED,
			HTTP_ERROR_AUTH,
			HTTP_ERROR_CERTIFICATE_EXPIRED,
			HTTP_ERROR_STREAM,
			HTTP_ERROR_CACHE,
			HTTP_ERROR_DEFAULT
		};

		enum HttpHeader
		{
			HTTP_HEADER_AGE,
			HTTP_HEADER_CACHE_CONTROL,
			HTTP_HEADER_CONNECTION,
			HTTP_HEADER_CONTENT_DISPOSITION,
			HTTP_HEADER_CONTENT_LENGTH,
			HTTP_HEADER_CONTENT_RANGE,
			HTTP_HEADER_CONTENT_TYPE,
			HTTP_HEADER_COOKIE,
			HTTP_HEADER_DATE,
			HTTP_HEADER_ETAG,
			HTTP_HEADER_EXPIRES,
			HTTP_HEADER_HOST,
			HTTP_HEADER_IF_MODIFIED_SINCE,
			HTTP_HEADER_IF_NONE_MATCH,
			HTTP_HEADER_KEEP_ALIVE,
			HTTP_HEADER_LAST_MODIFIED,
			HTTP_HEADER_LOCATION,
			HTTP_HEADER_PROXY_AUTHENTICATE,
			HTTP_HEADER_PROXY_AUTHORIZATION,
			HTTP_HEADER_PROXY_CONNECTION,
			HTTP_HEADER_RANGE,
			HTTP_HEADER_SET_COOKIE,
			HTTP_HEADER_TE,
			HTTP_HEADER_TRAILERS,
			HTTP_HEADER_TRANSFER_ENCODING,
			HTTP_HEADER_UPGRADE,
			HTTP_HEADER_USER_AGENT,
			HTTP_HEADER_WWW_AUTHENTICATE,
			HTTP_HEADER_LAST = HTTP_HEADER_WWW_AUTHENTICATE
		};

		const USHORT HTTP_DEFAULT_PORT = 80;
		const USHORT HTTP_SECURE_PORT = 443;
	}
}
