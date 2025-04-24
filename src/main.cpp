#include <dpp/dpp.h>
#include "messages.hpp"
#include "secrets.hpp"

using namespace dpp;


int main()
{
	cluster bot(BOT_TOKEN);
	bot.on_log(utility::cout_logger());

	bot.on_ready([&bot](ready_t const& event) {
		if (run_once<struct CmdRegister>()) {
			std::vector<slashcommand> commands {
				{ "rules-init", "Prints out the rules", bot.me.id },
				{ "info-init", "Prints out the server info", bot.me.id },
				{ "rules", "Updates the rules", bot.me.id },
				{ "info", "Updates the server info", bot.me.id }
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
			bot.message_create({ event.command.channel_id, Messages.rules });
			event.reply(message("Rules published.").set_flags(m_ephemeral));
		} else if (commandNameHash == hasher("info-init")) {
			for (auto embed : Messages.info)
				bot.message_create({ event.command.channel_id, embed });
			event.reply(message("Info published.").set_flags(m_ephemeral));
		} else if (commandNameHash == hasher("rules") || commandNameHash == hasher("info")) {
			bool const isRules = commandNameHash == hasher("rules");
			auto messageMap = (co_await bot.co_messages_get(event.command.channel_id, 0, 0, 0, Messages.info.size())).get<message_map>();
			std::cout << "Message map size: " << messageMap.size();
			isRules ? 
		}
	});

	bot.start(dpp::st_wait);

	return 0;
}