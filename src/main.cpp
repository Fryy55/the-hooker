#include <dpp/dpp.h>
#include "values.hpp"
#include "secrets.hpp"

using namespace dpp;


int main()
{
	assert(Values.info.size() == Values.infoIDs.size());

	cluster bot(BOT_TOKEN);
	bot.on_log(utility::cout_logger());

	bot.on_ready([&bot](ready_t const& event) {
		if (run_once<struct CmdRegister>()) {
			std::vector<slashcommand> commands {
				{ "rules-init", "Prints out the rules", bot.me.id },
				{ "info-init", "Prints out the server info", bot.me.id },
				{ "rules", "Updates the rules", bot.me.id },
				{ "info", "Updates the server info (hardcoded IDs)", bot.me.id }
			};
			for (slashcommand command : commands)
				command.set_interaction_contexts({itc_guild});

			bot.global_bulk_command_create(commands);
		}
	});

	bot.on_slashcommand([&bot](slashcommand_t const& event) -> task<void> {
		auto hasher = std::hash<std::string>();
		auto const commandNameHash = hasher(event.command.get_command_name());

		if (commandNameHash == hasher("rules-init")) {
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