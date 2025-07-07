#pragma once


#define REQUEST_VERIFICATION(request, tag)         \
	do {                                           \
		if ((request).status != 200) {             \
			requestFailed((request), (tag), true); \
			return;                                \
		}                                          \
	} while (false)

#define REQUEST_VERIFICATION_CORO(request, tag)    \
	do {                                           \
		if ((request).status != 200) {             \
			requestFailed((request), (tag), true); \
			co_return;                             \
		}                                          \
	} while (false)

#define REQUESTS_VERIFICATION(tagOffset, ...)       \
	do {                                            \
		std::size_t tag = tagOffset;                \
		for (auto const& request : {__VA_ARGS__}) { \
			REQUEST_VERIFICATION(request, tag);     \
			++tag;                                  \
		}                                           \
	} while (false)

#define REQUESTS_VERIFICATION_CORO(tagOffset, ...)   \
	do {                                             \
		std::size_t tag = tagOffset;                 \
		for (auto const& request : {__VA_ARGS__}) {  \
			REQUEST_VERIFICATION_CORO(request, tag); \
			++tag;                                   \
		}                                            \
	} while (false)