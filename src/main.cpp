#include <dpp/dpp.h>
#include "values.hpp"
#include "secrets.hpp"
#include "include/logger.hpp"

using namespace dpp;


void startup() {
	cluster bot(BOT_TOKEN);

	utility::logger logger;
	bot.on_log(logger);

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
		auto const commandName = event.command.get_command_name();

		if (commandName == "badge") {
			namespace ch = std::chrono;

			time_t restartDateTimeType = ch::system_clock::to_time_t(
				ch::sys_days(
					ch::year_month_day(floor<ch::days>(ch::system_clock::now())) + ch::months(1)
				)
			);
			tm restartDateTM;
			localtime_s(&restartDateTM, &restartDateTimeType);
			event.reply(message(
				std::string("Badge extended, next refresh needed until approximately `") + Values.months[restartDateTM.tm_mon] + " " + std::to_string(restartDateTM.tm_mday) + " " + std::to_string(1900 + restartDateTM.tm_year) + "`"
			).set_flags(m_ephemeral));
		}
		else if (commandName == "rules-init") {
			bot.message_create({ event.command.channel_id, Values.rules });
			event.reply(message("Rules published.").set_flags(m_ephemeral));
		} else if (commandName == "info-init") {
			event.reply(message("Info published.").set_flags(m_ephemeral));
			for (auto embed : Values.info)
				co_await bot.co_message_create({ event.command.channel_id, embed });
		} else if (commandName == "rules" || commandName == "info") {
			bool isRules = commandName == "rules";
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

	return;
}

int main() {
	assert(Values.info.size() == Values.infoIDs.size());

	utility::logger::preinit("C:/Users/User/Desktop/debug_hooker");
	size_t startupCount = 1;
	utility::logger::logStartup(1);
	while (true) {
		try {
			startup();
		} catch (...) {
			++startupCount;
			utility::logger::logStartup(startupCount);
		}
	}

	return 0;
}