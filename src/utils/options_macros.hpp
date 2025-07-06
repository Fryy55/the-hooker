#pragma once


#define OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE(eventPointer)                           \
	auto const& options = (eventPointer)->command.get_command_interaction().options; \
	if (options.empty()) {                                                           \
		noOptions(true);                                                             \
		return;                                                                      \
	}                                                                                \
	auto const& subcommandName = options[0].name

#define OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE_CORO(eventPointer)                      \
	auto const& options = (eventPointer)->command.get_command_interaction().options; \
	if (options.empty()) {                                                           \
		noOptions(true);                                                             \
		co_return;                                                                   \
	}                                                                                \
	auto const& subcommandName = options[0].name