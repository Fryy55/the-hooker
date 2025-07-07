#pragma once

#include <dpp/cluster.h>


class Commands final {
public:
	Commands(dpp::cluster& bot) : m_bot(bot) {}

	void bindCommand(dpp::slashcommand_t const& event) { m_event = &event; }

	void badge();
	dpp::task<void> init();
	dpp::task<void> update();
	dpp::task<void> release();

	void requestFailed(dpp::http_request_completion_t const& request, std::size_t tag, bool ephemeral);
	void noOptions(bool ephemeral);
	void unknownCommand(bool ephemeral);
	void unknownSubcommand(bool ephemeral);

private:
	// Fields
	dpp::cluster& m_bot;
	dpp::slashcommand_t const* m_event;
};