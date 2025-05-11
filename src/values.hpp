#pragma once

#include <dpp/dpp.h>

using namespace dpp;


struct {
	embed const rules = embed()
				.set_color(colors::purple_amethyst)
				.set_title("Server Rules")
				.set_description(
					"1. Follow [Discord's ToS](https://discord.com/terms) and [Discord's Community Guidelines](https://discord.com/guidelines).\n"
					"> For example, you cannot use Discord if you're under the age of 13, and will be banned from this server if we find out.\n"
					"\n"
					"2. Don't engage in hateful conduct.\n"
					"> This includes bullying, harassment, racism, homophobia, etc., and will not be tolerated.\n"
					"\n"
					"3. Respect the staff members, and do not engage in conduct that annoys them.\n"
					"> Examples of such conduct include, but are not limited to, blatantly ignoring a mod's instructions, mini-modding, tagging a staff member to annoy them, etc.\n"
					"\n"
					"4. Avoid talking about NSFW topics.\n"
					"> Leniencies can be given to jokes about sex, as long as they don't go too far.\n"
					"\n"
					"5. Do not support/engage in conduct that supports biastophilia, necrophilia, pedophilia, zoophilia.\n"
					"> Doing so, even outside the server, will result in a ban without chance of appeal.\n"
					"\n"
					"6. Do not discuss anything that involves the consumption or promotion of harmful substances, such as drugs, alcohol and cigarettes (even if it is legal for you to).\n"
					"> As an exception, discussions about dealing with the negative side effects of these substances are permissible.\n"
					"\n"
					"7. Do not send spam.\n"
					"> This includes sending messages, images, videos, etc.in a way that clogs up channels and annoys server members.\n"
					"\n"
					"8. Send volume warnings for videos that are loud.\n"
					"> Doing so intentionally with deliberate aim to cause panic can result in a warn, while accidental ones can be verbally warned.\n"
					"\n"
					"9. Use channels for their intended purposes.\n"
					"> Read the channel name, description, or check for any pinned messages to find the purpose of the channel, as well as any channel-specific rules. These rules are equally as enforceable as server rules.\n"
					"\n"
					"10. Keep personal drama out of the server.\n"
					"> If a staff member tells you to stop and bring it to DMs, listen to them. Doing otherwise can result in a warn.\n"
					"\n"
					"11. Do not send server invites within the server.\n"
					"> Sending server invites is not allowed, _even in <#1351956762428837918>_.\n"
					"\n"
					"12. Use common sense.\n"
					"> Just because certain types of conduct are not covered in these rules, doesn't mean that you cannot get warned for them.\n"
					"\n"
				)
				.set_footer(
					embed_footer()
					.set_text("This server's personality and rules were almost entirely stolen from TSL, I am not sorry.")
				);

	std::vector<embed> const info = {
		embed()
		.set_color(colors::purple_amethyst)
		.set_title("Roles Information")
		.set_description(
			"Below are some of the main roles of the server.\n"
		)
		.add_field(
			"Staff/Privileged Roles",
			"<@&1351991165603545249> - People in charge of the whole server. Each gets a personalized role.\n"
			"<@&1351991422550802533> - Trusted users who keep the server organized and safe.\n"
			"<@&1351937107060326451> - Programmers that have access to special hidden chats for easier cooperative development.",
			false
		)
		.add_field(
			"Leveling Roles",
			"<@&1351944696640962581> - reaching this level gives you the <@&1351904191194599544> role.\n"
			"<@&1351944961830031412>\n"
			"<@&1351945155883696221>",
			false
		),
		embed()
		.set_color(colors::purple_amethyst)
		.set_title("Staff list")
		.add_field(
			":crown: Owners",
			"Fryy_55 [she/her]",
			true
		)
		.add_field(
			":tools: Moderators",
			"None",
			true
		),
		embed()
			.set_color(colors::purple_amethyst)
			.set_title("Extras")
			.set_description(
				":link: **Invite Link**\n"
				"https://discord.com/invite/4vqtjfdhTk"
			)
			.set_footer(
				embed_footer()
				.set_text("yeah idk what to add here ¯\\_(ツ)_/¯")
			)
	};
	std::vector<snowflake> const infoIDs = {
		1365011275834327111,
		1365011281761009674,
		1365011287599485031
	};
	char const* months[12] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};
} Values;