#include <dpp/dpp.h>

#include "secrets.hpp"
#include "logger.hpp"
#include "Commands.hpp"
#include "values.hpp"

using namespace dpp;


int main() {
	static_assert(values::info.size() == values::infoIDs.size());

	cluster bot(secrets::BOT_TOKEN);
	Commands commands(bot);

	bot.on_log(utility::logger("C:/Users/User/Desktop/debug_hooker"));

	bot.on_ready([&bot](ready_t const& event) {
		if (run_once<struct CmdRegister>()) {
			std::vector commandsVector {
				{ "badge", "the infinite dev badge glitch", bot.me.id },
				slashcommand("init", "Print different server messages (channel check)", bot.me.id)
					.add_option({ co_sub_command, "rules", "Prints out the rules" })
					.add_option({ co_sub_command, "info", "Prints out the server info" }),
				slashcommand("update", "Updates initialized server messages (channel check + hardcoded IDs)", bot.me.id)
					.add_option({ co_sub_command, "rules", "Updates the rules" })
					.add_option({ co_sub_command, "info", "Updates server info" }),
				slashcommand("release", "Posts a mod release/update log", bot.me.id)
					.add_option({ co_string, "owner-repo", "Owner's account name and repo's name for the mod in the format OWNER/REPO", true })
					.add_option({ co_boolean, "new-release", "Whether or not is this a new release", true })
					.add_option({ co_boolean, "ping", "Whether or not to ping Mod Notifs", true })
					.add_option({ co_string, "branch", "The repo's branch to retrieve mod.json and pack.png from (optional, defaults to main)" })
			};
			for (auto& command : commandsVector)
				command.set_interaction_contexts({itc_guild});

			bot.global_bulk_command_create(commandsVector);
		}

		bot.set_presence(presence(ps_online, at_competing, "being the worst bot :thumbsup:"));
	});

	bot.on_slashcommand([&bot, &commands](slashcommand_t const& event) -> task<void> {
		event.thinking(true);
		auto const& commandName = event.command.get_command_name(); // still can't get over comparing it directly :pensive:
		commands.bindCommand(event);

		if (commandName == "badge")
			commands.badge();
		else if (commandName == "init")
			co_await commands.init();
		else if (commandName == "update")
			co_await commands.update();
		else if (commandName == "release")
			co_await commands.release();
		else
			commands.unknownCommand(true);
	});

	bot.start(dpp::st_wait);

	return 0;
}