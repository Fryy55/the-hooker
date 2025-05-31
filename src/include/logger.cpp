#include <dpp/dpp.h>

#include "logger.hpp"

using namespace dpp;


constexpr char const* const utility::logger::m_logEndpointFmt = " %a %b %d %I:%M:%S %p %Y ";

tm utility::now_tm() {
	namespace ch = std::chrono;

	auto now_tt = ch::system_clock::to_time_t(ch::system_clock::now());
	tm now_tm;
	localtime_s(&now_tm, &now_tt);

	return now_tm;
}



utility::logger::logger(utility::logger const&) {
	m_log = std::ofstream();
}

void utility::logger::preinit(char const* dirPath) {
	namespace fs = std::filesystem;

	m_path = dirPath;

	// Path management
	if (!m_path.ends_with('/'))
		m_path.append("/");

	if (!fs::is_directory(m_path))
		fs::create_directory(m_path);

	fs::directory_entry firstFile;
	unsigned char filesCount = 0;
	for (auto file : fs::directory_iterator(m_path)) {
		if (!filesCount)
			firstFile = file;
		++filesCount;
	}

	if (filesCount >= 5)
		remove(firstFile);

	// Current log creation
	auto now_tm = utility::now_tm();
	auto fmt = "Log %F %H.%M.%S.txt";
	char fileName[32];
	strftime(fileName, sizeof(char) * 32, fmt, &now_tm);

	m_path.append(fileName);
}

void utility::logger::logStartup(size_t startupNumber) {
	utility::logger log;
	log.open();
	log << "STARTUP NUMBER: " << startupNumber << '\n';
	log.close();

	return;
}

void utility::logger::operator()(log_t const& log) {
	char const* severity;
	switch (log.severity) {
		case loglevel::ll_debug:
			severity = "DEBUG: ";
			break;
		case loglevel::ll_info:
			severity = "INFO: ";
			break;
		case loglevel::ll_warning:
			severity = "WARNING: ";
			break;
		case loglevel::ll_error:
			severity = "ERROR: ";
			break;
		case loglevel::ll_critical:
			severity = "CRITICAL: ";
			break;
		default:
			return;
	}

	this -> open();
	*this << timestamp() << severity << log.message << '\n';
	this -> close();

	return;
}

std::string utility::logger::timestamp() {
	auto fmt = "[%a %b %d %I:%M:%S %p %Y] ";
	auto now_tm = utility::now_tm();

	char ret[64];
	strftime(ret, sizeof(char) * 64, fmt, &now_tm);
	return std::string(ret);
}