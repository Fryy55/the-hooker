#include <dpp/dpp.h>
#include <chrono>
#include "values.hpp"
#include "secrets.hpp"

using namespace dpp;


int main() {
	assert(Values.info.size() == Values.infoIDs.size());

	cluster bot(BOT_TOKEN);
	std::ofstream O("C:/Users/User/Desktop/debug_hooker.txt", std::ofstream::out | std::ofstream::trunc);
	O.close();

	bot.on_log([](log_t const& log) {
		using namespace std::chrono;

		std::ofstream O("C:/Users/User/Desktop/debug_hooker.txt", std::ofstream::app);

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

		auto now_tt = system_clock::to_time_t(system_clock::now());
		tm now_tm;
		localtime_s(&now_tm, &now_tt);

		auto fmt = "%a %b %d %I:%M:%S %p %Y";

		O << '[' << std::put_time(&now_tm, fmt) << "] " << severity << log.message << '\n';
		O.close();
		std::cout << '[' << std::put_time(&now_tm, fmt) << "] " << severity << log.message << '\n';

		return;
	});

	bot.on_ready([&bot](ready_t const& event) {
		if (run_once<struct CmdRegister>()) {
			std::vector<slashcommand> commands {
				{ "badge", "the infinite dev badge glitch", bot.me.id },
				{ "rules-init", "Prints out the rules", bot.me.id },
				{ "info-init", "Prints out the server info", bot.me.id },
				{ "rules", "Updates the rules", bot.me.id },
				{ "info", "Updates the server info (hardcoded IDs)", bot.me.id }
			};
			for (slashcommand command : commands)
				command.set_interaction_contexts({itc_guild});

			bot.global_bulk_command_create(commands);
		}

		bot.set_presence(presence(ps_online, at_competing, "being the worst bot :thumbsup:"));
	});

	bot.on_slashcommand([&bot](slashcommand_t const& event) -> task<void> {
		auto hasher = std::hash<std::string>();
		auto const commandNameHash = hasher(event.command.get_command_name());

		if (commandNameHash == hasher("badge")) {
			using namespace std::chrono;

			time_t restartDateTimeType = system_clock::to_time_t(
				sys_days(
					year_month_day(floor<std::chrono::days>(system_clock::now())) + months(1)
				)
			);
			tm restartDateTM;
			localtime_s(&restartDateTM, &restartDateTimeType);
			event.reply(message(
				std::string("Badge extended, next refresh needed until approximately `") + Values.months[restartDateTM.tm_mon] + " " + std::to_string(restartDateTM.tm_mday) + " " + std::to_string(1900 + restartDateTM.tm_year) + "`"
			).set_flags(m_ephemeral));
		}
		else if (commandNameHash == hasher("rules-init")) {
			bot.message_create({ event.command.channel_id, Values.rules });
			event.reply(message("Rules published.").set_flags(m_ephemeral));
		} else if (commandNameHash == hasher("info-init")) {
			event.reply(message("Info published.").set_flags(m_ephemeral));
			for (auto embed : Values.info)
				co_await bot.co_message_create({ event.command.channel_id, embed });
		} else if (commandNameHash == hasher("rules") || commandNameHash == hasher("info")) {
			bool isRules = commandNameHash == hasher("rules");
			if (isRules ? event.command.channel.name == "server-info" : event.command.channel.name == "rules") {
				co_await event.co_reply(message(std::string("Wrong channel; expected ") + (isRules ? "#rules" : "#server-info")).set_flags(m_ephemeral));
				co_return;
			}
			event.reply(message(std::string(isRules ? "Rules" : "Info") + " updated.").set_flags(m_ephemeral));
			if (isRules) {
				auto rulesMessage = (co_await bot.co_message_get(event.command.channel.last_message_id, event.command.channel_id)).get<message>();
				rulesMessage.embeds[0] = Values.rules;
				bot.message_edit(rulesMessage);
			} else {
				for (unsigned char index = 0; index < Values.info.size(); ++index) {
					auto infoMessage = (co_await bot.co_message_get(Values.infoIDs[index], event.command.channel_id)).get<message>();
					infoMessage.embeds[0] = Values.info[index];
					bot.message_edit(infoMessage);
				}
			}
		}
	});

	bot.start(dpp::st_wait);

	return 0;
}