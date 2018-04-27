#include "json_response.h"
#include <json/value.h>
json_response::json_response(struct mg_connection *conn)
	: response(conn, "content-type application/json")
{
}