#include "cmdline_parameters.h"

namespace easy
{

	cmdline_parameters::cmdline_parameters(int argc, char* argv[])
	{
		parse(argc, argv);
	}

	std::string cmdline_parameters::get_parameter(const string& key) const
	{
		map<string, string>::const_iterator it = m_parameters.find(key);
		if (it != m_parameters.end())
			return it->second;
		return "";
	}

	void cmdline_parameters::parse(int argc, char* argv[])
	{
		for (int i = 0; i < argc;)
		{
			string key;
			string value;
			key.append(argv[i]);
			if (++i < argc)
				value.append(argv[i]);

			m_parameters.insert(make_pair(key, value));
		}
	}
}

