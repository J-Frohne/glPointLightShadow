#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace logger
{
	inline void init()
	{
		spdlog::set_pattern("[%l]: %^%v%$");
		spdlog::set_level(spdlog::level::level_enum::debug);
	}
}