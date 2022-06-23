#include "Logger.hpp"

#include <spdlog/async.h>
#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <array>
#include <memory>
#include <cassert>

#ifdef _DEBUG
int FearEngine::Logger::init()
{
	std::ifstream file(LogConfigPath);
	if (file)
	{
		file >> logConfig;
	}

	file.close();

	spdlog::init_thread_pool(QueueSize, Workers);
	if (logConfig.empty() || logConfig.find("Loggers") == logConfig.end())
	{
		return 0;
	}

	for (auto& logger : logConfig["Loggers"])
	{
		createLogger(logger["Name"], logger["Enabled"]);
		loggers.find(logger["Name"])->second.logger->set_pattern(logger["Pattern"]);
	}

	for (auto& logger: loggers)
	{
		logger.second.logger->set_level(spdlog::level::debug);
	}

	return 0;
}

int FearEngine::Logger::createLogger(const std::string_view& name, bool enabled)
{
	assert(loggers.find(name) == loggers.end() && "Logger already exists");

	std::array<spdlog::sink_ptr, 2> sinks;
	sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	constexpr const bool truncate = true;
	sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(fmt::format("{0}/{1}.{2}", LogDir, name.data(), LogExt), truncate);

	auto logger = std::make_shared<spdlog::logger>(name.data(), begin(sinks), end(sinks));
	loggers.emplace(logger->name(), LoggerData{enabled, logger});

	return 0;
}

void FearEngine::Logger::enableLogger(const std::string_view& name)
{
	auto logger = loggers.find(name);
	assert(logger != loggers.end());

	logger->second.enabled = true;
}

void FearEngine::Logger::disableLogger(const std::string_view& name)
{
	auto logger = loggers.find(name);
	assert(logger != loggers.end());

	logger->second.enabled = false;
}

void FearEngine::Logger::flush(const std::string_view& name) { loggers[name].logger->flush(); }

void FearEngine::Logger::flushAll()
{
	for (auto& logger : loggers)
	{
		logger.second.logger->flush();
	}
}

#else

int FearEngine::Logger::init() { return 0; }
void FearEngine::Logger::flush(const std::string_view& name) {}
void FearEngine::Logger::flushAll() {}
int FearEngine::Logger::createLogger(const std::string_view& name, bool enabled) { return 0; }
void FearEngine::Logger::enableLogger(const std::string_view& name) {}
void FearEngine::Logger::disableLogger(const std::string_view& name) {}

#endif	// _DEBUG

FearEngine::Logger::~Logger()
{
	flushAll();
	loggers.clear();
	spdlog::shutdown();
}
