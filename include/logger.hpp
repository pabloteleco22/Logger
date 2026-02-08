#pragma once

#include "decoration.hpp"
#include "greeter.hpp"
#include "level.hpp"
#include "levelfilter.hpp"

#include <memory>
#include <mutex>
#include <sstream>

namespace simple_logger {
struct Logger {
    virtual ~Logger() {};

    virtual void write(const Level &level, const string &message) const = 0;
    virtual void set_level_filter(std::shared_ptr<LevelFilter> level_filter) = 0;

    class LoggerStreamResponse {
      private:
        const Logger *logger;
        const Level *level;
        std::ostringstream message;

      public:
        LoggerStreamResponse(const Logger *logger, const Level *level);
        LoggerStreamResponse &operator<<(auto message) {
            this->message << message;

            return *this;
        }

        class Flush {};

        static Flush end;

        LoggerStreamResponse &operator<<(std::ostream &(*func)(std::ostream &));
        void operator<<(Flush);
        void flush();
    };

    LoggerStreamResponse operator<<(const Level &level) const;
};

struct WriterLogger : public Logger {
    WriterLogger(WriterLogger &) = delete;
    WriterLogger(std::shared_ptr<LoggerDecoration> decoration);
    virtual void set_level_filter(std::shared_ptr<LevelFilter> level_filter) override;

  protected:
    std::shared_ptr<LoggerDecoration> decoration;
    std::shared_ptr<LevelFilter> level_filter;
};

struct StreamLogger : public WriterLogger {
    StreamLogger() = delete;
    StreamLogger(StreamLogger &) = delete;
    StreamLogger(std::ostream *stream, std::shared_ptr<LoggerDecoration> decoration,
                 const Greeter &greeter, const string &greeting_message = default_greeting_message);

    virtual void write(const Level &level, const string &message) const override;

    static const string default_greeting_message;

  protected:
    std::ostream *stream;

  private:
    void greetings(const string &message) const;
};

struct StandardLogger : public StreamLogger {
    StandardLogger(StandardLogger &) = delete;

    StandardLogger(std::shared_ptr<LoggerDecoration> decoration, const Greeter &greeter,
                   const string &greeting_message = default_greeting_message);

    virtual void write(const Level &level, const string &message) const override;

    static const string default_greeting_message;
};

struct ThreadLogger : public Logger {
    ThreadLogger() = delete;
    ThreadLogger(const ThreadLogger &other) = delete;

    ThreadLogger(std::shared_ptr<Logger> other);

    void write(const Level &level, const string &message) const override;
    void set_level_filter(std::shared_ptr<LevelFilter> level_filter) override;
    void set_logger(std::shared_ptr<Logger> logger);

  private:
    mutable std::mutex mut;
    std::shared_ptr<Logger> logger;
};

struct BiLogger : public Logger {
    BiLogger() = delete;
    BiLogger(const BiLogger &other) = delete;
    BiLogger(std::shared_ptr<Logger> logger1, std::shared_ptr<Logger> logger2);

    void write(const Level &level, const string &message) const override;
    void set_level_filter(std::shared_ptr<LevelFilter> level_filter) override;
    void set_first_logger(std::shared_ptr<Logger> logger);
    void set_second_logger(std::shared_ptr<Logger> logger);

  private:
    std::shared_ptr<Logger> logger1;
    std::shared_ptr<Logger> logger2;
};
}; // namespace simple_logger
