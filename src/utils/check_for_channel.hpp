#pragma once


#define CHECK_FOR_CHANNEL(channelName, eventPointer)                          \
	do {                                                                      \
		if ((eventPointer)->command.channel.name != (channelName)) {          \
			(eventPointer)->edit_original_response(                           \
				message(                                                      \
					std::format("Wrong channel; expected #{}", (channelName)) \
				).set_flags(m_ephemeral));                                    \
			return;                                                  	      \
		}                                                                     \
	} while(false)

#define CHECK_FOR_CHANNEL_CORO(channelName, eventPointer)                     \
	do {                                                                      \
		if ((eventPointer)->command.channel.name != (channelName)) {          \
			co_await (eventPointer)->co_edit_original_response(               \
				message(                                                      \
					std::format("Wrong channel; expected #{}", (channelName)) \
				).set_flags(m_ephemeral));                                    \
			co_return;                                                        \
		}                                                                     \
	} while(false)