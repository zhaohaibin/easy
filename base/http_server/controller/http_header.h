#ifndef _HTTP_HTTP_HEADER_H
#define _HTTP_HTTP_HEADER_H
#include <vector>
#include <string>
#include <tuple>
using namespace std;

class http_header
{
public:
	void add_header(const string& name, const string& value);
	const vector<tuple<string, string>>& get_headers();
private:
	vector<tuple<string, string>> m_headers;
};
#endif //_HTTP_HTTP_HEADER_H
