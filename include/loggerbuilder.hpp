#pragma once

#include "decoration.hpp"
#include "greeter.hpp"
#include "logger.hpp"
#include <memory>

namespace simple_logger {
struct LoggerBuilder {
    virtual std::unique_ptr<Logger> build() = 0;
};

class StandardLoggerBuilder : public LoggerBuilder {
    std::shared_ptr<LoggerDecoration> default_decoration{std::make_shared<VoidLoggerDecoration>()};
    std::shared_ptr<Greeter> default_greeter{std::make_shared<ColorfulDefaultGreeter>()};
    string greeting_string{StandardLogger::default_greeting_message};

    std::shared_ptr<LoggerDecoration> decoration{default_decoration};
    std::shared_ptr<Greeter> greeter{default_greeter};

  public:
    std::unique_ptr<Logger> build() override;
    StandardLoggerBuilder &set_decoration(std::shared_ptr<LoggerDecoration> decoration);
    StandardLoggerBuilder &set_greeter(std::shared_ptr<Greeter> greeter);
    StandardLoggerBuilder &set_greeting_string(const string &greeting_string);
    StandardLoggerBuilder &reset_config();
};

class StreamLoggerBuilder : public LoggerBuilder {
    std::shared_ptr<LoggerDecoration> default_decoration{std::make_shared<VoidLoggerDecoration>()};
    std::shared_ptr<Greeter> default_greeter{std::make_shared<DefaultGreeter>()};
    string greeting_string = StreamLogger::default_greeting_message;

    std::shared_ptr<LoggerDecoration> decoration{default_decoration};
    std::shared_ptr<Greeter> greeter{default_greeter};
    std::ostream *stream;

  public:
    StreamLoggerBuilder(std::ostream *stream);
    std::unique_ptr<Logger> build() override;
    StreamLoggerBuilder &set_stream(std::ostream *stream);
    StreamLoggerBuilder &set_decoration(std::shared_ptr<LoggerDecoration> decoration);
    StreamLoggerBuilder &set_greeter(std::shared_ptr<Greeter> greeter);
    StreamLoggerBuilder &set_greeting_string(const string &greeting_string);
    StreamLoggerBuilder &reset_config();
};
}; // namespace simple_logger
