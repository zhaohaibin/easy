#ifndef _JSON_RESPONSE_H
#define _JSON_RESPONSE_H

#include "response.h"


class json_response : public response
{
public:
	json_response(struct mg_connection *conn);
};
#endif //_JSON_RESPONSE_H
