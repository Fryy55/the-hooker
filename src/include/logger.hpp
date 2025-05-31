#pragma once

#include <dpp/dpp.h>


namespace dpp::utility {

tm now_tm();

class logger final {
public:
	logger() = default;
	logger(logger const&);

	static void preinit(char const* dirPath);
	static void logStartup(size_t);

	void operator()(dpp::log_t const&);

	template<typename T>
	auto operator<<(T const& output) -> decltype(std::cout << output, *this) {
		m_log << output;
		std::cout << output;
		
		return *this;
	}

	void open() { m_log.open(m_path, std::ofstream::app); }
	void close() { m_log.close(); }
	std::string timestamp();

private:
	// Fields
	static inline std::string m_path;
	static char const* const m_logEndpointFmt;
	std::ofstream m_log;
};

}