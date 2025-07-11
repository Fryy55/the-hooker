#include "Commands.hpp"

#include <dpp/nlohmann/json.hpp>

#include "values.hpp"
#include "check_for_channel.hpp"
#include "options_macros.hpp"
#include "requests_macros.hpp"
#include "enums.hpp"

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

task<void> Commands::release() {
	OPTIONS_PRELUDE_CORO(m_event);

	auto ownerRepo = std::get<std::string>(m_event->get_parameter("owner-repo"));
	auto tagPos = std::ranges::find_if(
		options.begin(),
		options.end(),
		[](command_data_option const& option) { return option.name == "tag"; }
	);
	auto releaseURL = (tagPos == options.end()) ?
		std::format(
			"https://api.github.com/repos/{}/releases/latest",
			ownerRepo
		)
		:
		std::format(
			"https://api.github.com/repos/{}/releases/tags/{}",
			ownerRepo,
			std::get<std::string>(tagPos->value)
		);
	auto releaseReq = co_await m_bot.co_request(
		releaseURL,
		m_get,
		"", // postdata
		"", // mimetype
		{ { "User-Agent", values::userAgent } } // headers
	);

	auto branchPos = std::ranges::find_if(
		options.begin(),
		options.end(),
		[](command_data_option const& option) { return option.name == "branch"; }
	);
	auto branch = (branchPos == options.end()) ? "main" : std::get<std::string>(branchPos->value);
	auto modjsonReq = co_await m_bot.co_request(
		std::format("https://raw.githubusercontent.com/{}/{}/mod.json", ownerRepo, branch),
		m_get
	);
	REQUESTS_VERIFICATION_CORO(100, releaseReq, modjsonReq);

	auto release = nlohmann::json::parse(releaseReq.body);
	std::string tag = release["tag_name"];
	bool prerelease = release["prerelease"];

	auto modjson = nlohmann::json::parse(modjsonReq.body);
	std::string name = modjson["name"];
	std::string devs;
	if (modjson.contains("developer"))
		devs = modjson["developer"];
	else {
		std::vector<nlohmann::basic_json<>> devsVector = modjson["developers"];
		switch (devsVector.size()) {
			case 1u:
				devs = devsVector[0];
				break;

			case 2u:
				devs = std::format("{} & {}", std::string(devsVector[0]), std::string(devsVector[1]));
				break;

			default:
				devs = std::format("{} and more", std::string(devsVector[0]));
				break;
		}
	}

	auto releaseTypeString = std::get<std::string>(m_event->get_parameter("release-type"));
	ReleaseType releaseType;
	if (releaseTypeString == "new")
		releaseType = ReleaseType::NEW;
	else if (releaseTypeString == "full")
		releaseType = ReleaseType::FULL;
	else
		releaseType = ReleaseType::UPDATE;
	std::uint32_t color;
	std::string_view title;
	std::string_view emoji;
	switch (releaseType) {
		case ReleaseType::NEW:
			color = colors::cyan;
			title = "New release!";
			emoji = ":new: ";
			break;

		case ReleaseType::FULL:
			if (prerelease) {
				m_event->edit_original_response(
					message("A full release can't be a prerelease!").set_flags(m_ephemeral)
				);
				co_return;
			}

			color = colors::pale_green;
			title = "Full release!";
			emoji = ":white_check_mark: ";
			break;

		case ReleaseType::UPDATE:
			color = colors::purple_amethyst;
			title = "New update!";
			break;

		default:
			throw "Unknown color";
	}
	if (prerelease)
		color = colors::dark_yellow;
	auto releaseMessage = message(
		m_event->command.channel_id,
		std::get<bool>(m_event->get_parameter("ping")) ? "||<@&1351951492407627877>||" : ""
	).add_embed(
		embed()
			.set_color(color)
			.set_title(title)
			.set_description(
				std::format(
					"## {}{} {} by {}\n{}" // Status emoji/name/version/authors/prerelease
					"### Changelog:\n"
					"# {}\n" // Version
					"{}\n" // Release body
					"## [Download]({})", // Download URL
					emoji, name, tag, devs, prerelease ? values::prereleaseEmojis : "",
					std::move(tag),
					std::string(release["body"]),
					std::string(release["assets"][0]["browser_download_url"])
				)
			)
			.set_thumbnail(
				std::format("https://raw.githubusercontent.com/{}/{}/logo.png", ownerRepo, branch)
			)
			.set_footer("Published", "")
			.set_timestamp(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
			.set_url(release["html_url"])
	).set_allowed_mentions(
		true,
		true
	);


	m_bot.message_create(releaseMessage);

	m_event->edit_original_response(message("Release published.").set_flags(m_ephemeral));

	co_return;
}

void Commands::requestFailed(http_request_completion_t const& request, std::size_t tag, bool ephemeral) {
	m_event->edit_original_response(
		message(
			std::format(
				"HTTP request failed. (Tag: {}; Status: {})\n{}",
				tag,
				request.status,
				request.body
			)
		).set_flags(ephemeral ? m_ephemeral : 0u)
	);

	return;
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