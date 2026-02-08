#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

using std::string;

namespace simple_logger {
struct LoggerDecoration {
    LoggerDecoration() {};
    LoggerDecoration(LoggerDecoration &) = delete;
    virtual string get_decoration() const = 0;
};

struct VoidLoggerDecoration : public LoggerDecoration {
    virtual string get_decoration() const override;
};

struct DecorationBundler : public LoggerDecoration {
    DecorationBundler(std::vector<std::shared_ptr<LoggerDecoration>> &decoration_list,
                      const string separator = " | ");
    virtual string get_decoration() const override;

  private:
    std::vector<std::shared_ptr<LoggerDecoration>> *decoration_list;
    const string separator;
};

struct PackDecoration : public LoggerDecoration {
    PackDecoration(const LoggerDecoration &logger_decoration, const string begin = "[",
                   const string end = "]");
    virtual string get_decoration() const override;

  private:
    const LoggerDecoration *logger_decoration;
    const string begin;
    const string end;
};

struct TimedLoggerDecoration : public LoggerDecoration {
    TimedLoggerDecoration();
    TimedLoggerDecoration(TimedLoggerDecoration &other);
    virtual string get_decoration() const override;

  private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
};

struct HourLoggerDecoration : public LoggerDecoration {
    virtual string get_decoration() const override;
};
}; // namespace simple_logger
