#include "Demo.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void initializeSpdlog()
{
#ifdef OS_LINUX
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#else
    auto consoleSink = std::make_shared<spdlog::sinks::windebug_sink_mt>();
#endif

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("general.txt", true);

    consoleSink->set_level(spdlog::level::trace);
    fileSink->set_level(spdlog::level::trace);

    auto logger = std::make_shared<spdlog::logger>("general");
    logger->set_level(spdlog::level::debug);
    auto& sinks = logger->sinks();
    sinks.push_back(consoleSink);
    sinks.push_back(fileSink);
    spdlog::set_default_logger(logger);
    spdlog::flush_every(std::chrono::seconds(1));
}


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    initializeSpdlog();
    DemoApplication demoApplication;
    demoApplication.exec();

    return 0;
}
