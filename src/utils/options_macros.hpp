#pragma once


#define OPTIONS_PRELUDE(eventPointer)                                                \
	auto const& options = (eventPointer)->command.get_command_interaction().options; \
	do {                                                                             \
		if (options.empty()) {                                                       \
			noOptions(true);                                                         \
			return;                                                                  \
		}                                                                            \
	} while (false)

#define OPTIONS_PRELUDE_CORO(eventPointer)                                           \
	auto const& options = (eventPointer)->command.get_command_interaction().options; \
	do {                                                                             \
		if (options.empty()) {                                                       \
			noOptions(true);                                                         \
			co_return;                                                               \
		}                                                                            \
	} while (false)

#define OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE(eventPointer)                           \
	OPTIONS_PRELUDE(eventPointer);                                                   \
	auto const& subcommandName = options[0].name

#define OPTIONS_EXCLUSIVE_SUBCOMMAND_PRELUDE_CORO(eventPointer)                      \
	OPTIONS_PRELUDE_CORO(eventPointer);                                              \
	auto const& subcommandName = options[0].name