#include "Commands.hpp"

#include "values.hpp"
#include "check_for_channel.hpp"
#include "options_macros.hpp"

using namespace dpp;


void Commands::badge() {
	namespace ch = std::chrono;

	time_t restartDateTimeType = ch::system_clock::to_time_t(
		ch::sys_days(
			ch::year_month_day(floor<ch::days>(ch::system_clock::now())) + ch::months(1)
		)
	);
	tm restartDateTM;
	static_cast<void>(localtime_s(&restartDateTM, &restartDateTimeType));
	m_event->edit_original_response(message(
		std::format(
			"Badge extended, next refresh needed until approximately `{} {} {}`.",
			values::months[restartDateTM.tm_mon],
			restartDateTM.tm_mday,
			1900 + restartDateTM.tm_year
		)
	).set_flags(m_ephemeral));

	return;
}

task<void> Commands::init() {
	OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE_CORO(m_event);

	if (subcommandName == "rules") {
		CHECK_FOR_CHANNEL_CORO("rules", m_event);
		co_await m_bot.co_message_create({ m_event->command.channel_id, values::rules });
		m_event->edit_original_response(message("Rules published.").set_flags(m_ephemeral));
	} else if (subcommandName == "info") {
		CHECK_FOR_CHANNEL_CORO("server-info", m_event);
		for (auto const& embed : values::info)
			co_await m_bot.co_message_create({ m_event->command.channel_id, embed });
		m_event->edit_original_response(message("Info published.").set_flags(m_ephemeral));
	} else
		unknownSubcommand(true);

	co_return;
}

task<void> Commands::update() {
	OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE_CORO(m_event);

	if (subcommandName == "rules") {
		CHECK_FOR_CHANNEL_CORO("rules", m_event);

		auto rulesMessage = (
			co_await m_bot.co_message_get(values::rulesID, m_event->command.channel_id)
		).get<message>();
		rulesMessage.embeds[0] = values::rules;
		m_bot.message_edit(rulesMessage);

		m_event->edit_original_response(message("Rules updated.").set_flags(m_ephemeral));
	} else if (subcommandName == "info") {
		CHECK_FOR_CHANNEL_CORO("server-info", m_event);

		for (std::uint8_t index = 0; index < static_cast<std::uint8_t>(values::info.size()); ++index) {
			auto infoMessage = (
				co_await m_bot.co_message_get(values::infoIDs[index], m_event->command.channel_id)
			).get<message>();
			infoMessage.embeds[0] = values::info[index];
			m_bot.message_edit(infoMessage);
		}

		m_event->edit_original_response(message("Info updated.").set_flags(m_ephemeral));
	} else
		unknownSubcommand(true);

	co_return;
}

void Commands::noOptions(bool ephemeral) {
	m_event->edit_original_response(
		message("No options provided for a command that kinda requires them! How? idk, tell Fryy about this")
			.set_flags(ephemeral ? m_ephemeral : 0u)
		);

	return;
}

void Commands::unknownCommand(bool ephemeral) {
	m_event->edit_original_response(
		message("Whoopsies, unhandled command! Tell Fryy about this lmao idk")
			.set_flags(ephemeral ? m_ephemeral : 0u)
		);

	return;
}

void Commands::unknownSubcommand(bool ephemeral) {
	m_event->edit_original_response(
		message("Unhandled subcommand! It's probably WIP, but maybe Fryy is stupid")
			.set_flags(ephemeral ? m_ephemeral : 0u)
	);

	return;
}