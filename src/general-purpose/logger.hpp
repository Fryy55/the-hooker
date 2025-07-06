#pragma once

#include <dpp/dispatcher.h>

#include <fstream>


namespace dpp::utility {

	tm now_tm();

	class logger final {
	public:
		logger(char const* dirPath);
		logger(logger const&);

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
		std::string m_path;
		std::ofstream m_log;
	};

}