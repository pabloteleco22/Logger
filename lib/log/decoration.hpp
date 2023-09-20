#pragma once

#include <string>
#include <chrono>

using std::string;

namespace simple_logger {
struct LoggerDecoration {
    LoggerDecoration() {};
    LoggerDecoration(LoggerDecoration &) {};
    virtual string get_decoration() const = 0;
};

struct VoidLoggerDecoration : public LoggerDecoration {
    virtual string get_decoration() const override;
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
};