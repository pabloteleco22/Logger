// #include "lib/log/log.hpp"
#include "decoration.hpp"
#include "level.hpp"
#include "loggerbuilder.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace simple_logger;

using std::cout;
using std::endl;
using lsr = Logger::LoggerStreamResponse;

using std::unique_ptr;

lsr::Flush flush;

int main() {
    Debug debug;
    Info info;
    Warning warning;
    Error error;
    Critical critical;
    Level trace{0, "\033[90m", "Trace"};

    std::shared_ptr<UserCustomFilter> user_filter = std::make_shared<UserCustomFilter>(
        //         [&warning](const Level &level) { return level == warning; }};
        [&warning](const Level &level) { return level >= warning; });

    cout << "Standard logger" << endl;
    StandardLoggerBuilder standard_logger_builder;

    unique_ptr<Logger> standard_logger{standard_logger_builder.build()};
    cout << "All levels" << endl;
    standard_logger->write(trace, "Trace message");
    standard_logger->write(debug, "Debug message");
    standard_logger->write(info, "Info message");
    standard_logger->write(warning, "Warning message");
    standard_logger->write(error, "Error message");
    standard_logger->write(critical, "Critical message");

    cout << "operator<<" << endl;
    (*standard_logger) << trace << "Trace message " << 2 << flush;
    (*standard_logger) << debug << "Debug message " << 2 << flush;
    (*standard_logger) << info << "Info message " << 2 << flush;
    (*standard_logger) << warning << "Warning message " << 2 << flush;
    (*standard_logger) << error << "Error message " << 2 << flush;
    (*standard_logger) << critical << "Critical message " << 2 << flush;
    (*standard_logger) << debug << "Debug message " << 3 << endl << "with endl" << flush;

    {
        auto debug_logger = (*standard_logger) << debug;
        auto warn_logger = (*standard_logger) << warning;

        debug_logger << "Iterating:";
        warn_logger << "Iterating:";

        for (int i = 0; i < 10; ++i) {
            debug_logger << endl << " - Iteration: " << i;
            warn_logger << endl << " - Iteration: " << i;
        }
        debug_logger << flush;
        warn_logger << flush;
    }

    cout << endl << "Custom filter" << endl;
    standard_logger->set_level_filter(user_filter);

    standard_logger->write(debug, "Debug message");
    standard_logger->write(info, "Info message");
    standard_logger->write(warning, "Warning message");
    standard_logger->write(error, "Error message");
    cout << "operator<<" << endl;
    (*standard_logger) << debug << "Debug message " << 2 << flush;
    (*standard_logger) << info << "Info message " << 2 << flush;
    (*standard_logger) << warning << "Warning message " << 2 << flush;
    (*standard_logger) << error << "Error message " << 2 << flush;

    {
        auto debug_logger = (*standard_logger) << debug;
        auto warn_logger = (*standard_logger) << warning;

        debug_logger << "Iterating:";
        warn_logger << "Iterating:";

        for (int i = 0; i < 10; ++i) {
            debug_logger << endl << " - Iteration: " << i;
            warn_logger << endl << " - Iteration: " << i;
        }
        debug_logger << flush;
        warn_logger << flush;
    }

    cout << endl << "Standard stream logger" << endl;
    StreamLoggerBuilder stream_logger_builder(&cout);

    unique_ptr<Logger> standard_stream_logger{stream_logger_builder.build()};
    cout << "All levels" << endl;
    standard_stream_logger->write(debug, "Debug message");
    standard_stream_logger->write(info, "Info message");
    standard_stream_logger->write(warning, "Warning message");
    standard_stream_logger->write(error, "Error message");
    cout << "operator<<" << endl;
    (*standard_stream_logger) << debug << "Debug message " << 2 << flush;
    (*standard_stream_logger) << info << "Info message " << 2 << flush;
    (*standard_stream_logger) << warning << "Warning message " << 2 << flush;
    (*standard_stream_logger) << error << "Error message " << 2 << flush;

    cout << endl << "Custom filter" << endl;
    standard_stream_logger->set_level_filter(user_filter);

    standard_stream_logger->write(debug, "Debug message");
    standard_stream_logger->write(info, "Info message");
    standard_stream_logger->write(warning, "Warning message");
    standard_stream_logger->write(error, "Error message");

    cout << "operator<<" << endl;
    (*standard_stream_logger) << debug << "Debug message " << 2 << flush;
    (*standard_stream_logger) << info << "Info message " << 2 << flush;
    (*standard_stream_logger) << warning << "Warning message " << 2 << flush;
    (*standard_stream_logger) << error << "Error message " << 2 << flush;

    std::ofstream log_demo_stream{"logs/test/log_demo.log", std::ios::out};
    stream_logger_builder.reset_config().set_stream(&log_demo_stream);

    unique_ptr<Logger> stream_logger{stream_logger_builder.build()};
    stream_logger->write(info, "All levels");
    stream_logger->write(debug, "Debug message");
    stream_logger->write(info, "Info message");
    stream_logger->write(warning, "Warning message");
    stream_logger->write(error, "Error message");

    stream_logger->write(info, "Custom filter");
    stream_logger->set_level_filter(user_filter);

    stream_logger->write(debug, "Debug message");
    stream_logger->write(info, "Info message");
    stream_logger->write(warning, "Warning message");
    stream_logger->write(error, "Error message");

    cout << endl << "Thread standard logger" << endl;

    std::vector<std::shared_ptr<LoggerDecoration>> decoration_list;
    std::shared_ptr<TimedLoggerDecoration> timed_logger_decoration =
        std::make_shared<TimedLoggerDecoration>();
    std::shared_ptr<HourLoggerDecoration> hour_logger_decoration =
        std::make_shared<HourLoggerDecoration>();

    decoration_list.push_back(timed_logger_decoration);
    decoration_list.push_back(hour_logger_decoration);

    std::shared_ptr<DecorationBundler> decoration_bundler{
        std::make_shared<DecorationBundler>(decoration_list)};

    standard_logger_builder.reset_config().set_decoration(decoration_bundler);
    std::shared_ptr<Logger> thread_standard_logger{standard_logger_builder.build()};
    ThreadLogger thread_logger{thread_standard_logger};

    cout << "All levels" << endl;
    thread_logger.write(debug, "Debug message");
    thread_logger.write(info, "Info message");
    thread_logger.write(warning, "Warning message");
    thread_logger.write(error, "Error message");

    cout << endl << "Custom filter" << endl;
    thread_logger.set_level_filter(user_filter);

    thread_logger.write(debug, "Debug message");
    thread_logger.write(info, "Info message");
    thread_logger.write(warning, "Warning message");
    thread_logger.write(error, "Error message");

    std::ofstream thread_log_demo_stream{"logs/test/thread_log_demo.log", std::ios::out};
    stream_logger_builder.reset_config()
        .set_decoration(timed_logger_decoration)
        .set_stream(&thread_log_demo_stream);

    std::shared_ptr<Logger> thread_stream_logger{stream_logger_builder.build()};
    thread_logger.set_logger(thread_stream_logger);

    thread_logger.write(info, "All levels");
    thread_logger.write(debug, "Debug message");
    thread_logger.write(info, "Info message");
    thread_logger.write(warning, "Warning message");
    thread_logger.write(error, "Error message");

    thread_logger.write(info, "Custom filter");
    thread_logger.set_level_filter(user_filter);

    thread_logger.write(debug, "Debug message");
    thread_logger.write(info, "Info message");
    thread_logger.write(warning, "Warning message");
    thread_logger.write(error, "Error message");

    cout << endl << "Bi logger" << endl;

    std::shared_ptr<UserCustomGreeter> custom_greeter{
        std::make_shared<UserCustomGreeter>([](const string &m) {
            HourLoggerDecoration decoration;

            return "\033[1;104m[" + decoration.get_decoration() + " | Greetings]\033[0m " + m;
        })};

    std::shared_ptr<HourLoggerDecoration> logger_decoration =
        std::make_shared<HourLoggerDecoration>();

    standard_logger_builder.reset_config()
        .set_decoration(logger_decoration)
        .set_greeter(custom_greeter)
        .set_greeting_string("Starting thread standard logger");

    std::ofstream bilog_demo_stream{"logs/test/bilog_demo.log", std::ios::out};
    stream_logger_builder.reset_config()
        .set_decoration(logger_decoration)
        .set_stream(&bilog_demo_stream);

    std::shared_ptr<Logger> std_logger{standard_logger_builder.build()};
    std::shared_ptr<Logger> str_logger{stream_logger_builder.build()};

    std::shared_ptr<ThreadLogger> bilog_thread_std_logger{
        std::make_shared<ThreadLogger>(std_logger)};
    std::shared_ptr<ThreadLogger> bilog_thread_str_logger{
        std::make_shared<ThreadLogger>(str_logger)};

    BiLogger bi_logger{bilog_thread_std_logger, bilog_thread_str_logger};

    bi_logger.write(info, "All levels");
    bi_logger.write(debug, "Debug message");
    bi_logger.write(info, "Info message");
    bi_logger.write(warning, "Warning message");
    bi_logger.write(error, "Error message");
    bi_logger << info << "operator<<" << flush;
    bi_logger << debug << "Debug message " << 2 << flush;
    bi_logger << info << "Info message " << 2 << flush;
    bi_logger << warning << "Warning message " << 2 << flush;
    bi_logger << error << "Error message " << 2 << flush;

    bi_logger.write(info, "Custom filter");
    bi_logger.set_level_filter(user_filter);

    bi_logger.write(debug, "Debug message");
    bi_logger.write(info, "Info message");
    bi_logger.write(warning, "Warning message");
    bi_logger.write(error, "Error message");

    bi_logger << info << "operator<<" << flush;
    bi_logger << debug << "Debug message " << 2 << flush;
    bi_logger << info << "Info message " << 2 << flush;
    bi_logger << warning << "Warning message " << 2 << flush;
    bi_logger << error << "Error message " << 2 << flush;

    return 0;
}
