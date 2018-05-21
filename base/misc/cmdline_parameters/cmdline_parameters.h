#ifndef _CMDLINE_PARAMETERS_H
#define _CMDLINE_PARAMETERS_H
#include <map>
#include <string>
using namespace std;

namespace easy
{
	class cmdline_parameters
	{
	public:
		cmdline_parameters(int argc, char* argv[]);

		string get_parameter(const string& key) const;
	private:
		void parse(int argc, char* argv[]);
	private:
		map<string, string> m_parameters;
	};
}
#endif //_CMDLINE_PARAMETERS_H
