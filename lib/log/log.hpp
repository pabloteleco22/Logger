#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <ostream>
#include <mutex>
#include <functional>

using std::string;
using std::shared_ptr;

struct Level {
    Level() {};
    Level(const Level &other);
    Level(const Level *other);
    Level(shared_ptr<const Level> other);
    bool operator>=(const Level &other) const;
    bool operator>(const Level &other) const;
    bool operator<=(const Level &other) const;
    bool operator<(const Level &other) const;
    bool operator==(const Level &other) const;
    bool operator!=(const Level &other) const;
    string get_color() const;
    string get_level_name() const;
    bool is_printable() const;

    protected:
        unsigned short level_number{0};
        string color{"\033[0m"};
        string level_name{"Level"};
        bool printable{false};
};

struct Debug : public Level {
    using Level::Level;
    using Level::operator>=;
    using Level::operator>;
    using Level::operator<=;
    using Level::operator<;
    using Level::operator==;
    using Level::operator!=;
    Debug() {
        level_number = 51;
        color = "\033[1;32m";
        level_name = "Debug";
        printable = true;
    }
};

struct Info : public Level {
    using Level::Level;
    using Level::operator>=;
    using Level::operator>;
    using Level::operator<=;
    using Level::operator<;
    using Level::operator==;
    using Level::operator!=;
    Info() {
        level_number = 102;
        color = "\033[1;34m";
        level_name = "Info";
        printable = true;
    }
};

struct Warning : public Level {
    using Level::Level;
    using Level::operator>=;
    using Level::operator>;
    using Level::operator<=;
    using Level::operator<;
    using Level::operator==;
    using Level::operator!=;
    Warning() {
        level_number = 153;
        color = "\033[1;33m";
        level_name = "Warn";
        printable = true;
    }
};

struct Error : public Level {
    using Level::Level;
    using Level::operator>=;
    using Level::operator>;
    using Level::operator<=;
    using Level::operator<;
    using Level::operator==;
    using Level::operator!=;
    Error() {
        level_number = 204;
        color = "\033[1;31m";
        level_name = "Error";
        printable = true;
    }
};

struct Silence : public Level {
    using Level::Level;
    using Level::operator>=;
    using Level::operator>;
    using Level::operator<=;
    using Level::operator<;
    using Level::operator==;
    using Level::operator!=;
    Silence() {
        level_number = 255;
        level_name = "Silence";
    }
};

struct LevelFilter {
    virtual bool filter(const Level &filter) const = 0;
};

struct DefaultFilter : public LevelFilter {
    bool filter(const Level &filter) const override;
};

struct UserCustomFilter : public LevelFilter {
    UserCustomFilter(std::function<bool(const Level &level)> custom_filter) :
                                                            custom_filter{custom_filter} {}

    bool filter(const Level &filter) const override;

    private:
        std::function<bool(const Level &level)> custom_filter;
};

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

struct Logger {
    Logger();
    Logger(const Logger &other);

    virtual ~Logger() {};

    virtual void write(const Level &level, const string &message) = 0;
    virtual void write(shared_ptr<const Level> level, const string &message) = 0;
    virtual void set_level_filter(shared_ptr<const LevelFilter> level_filter);

    protected:
        shared_ptr<const LevelFilter> level_filter;
};

struct WriterLogger : public Logger {
    WriterLogger(const WriterLogger &other);
    WriterLogger(shared_ptr<const LoggerDecoration> decoration);

    protected:
        shared_ptr<const LoggerDecoration> decoration;
};

struct StreamLogger : public WriterLogger {
    StreamLogger() = delete;
    StreamLogger(const StreamLogger &other);

    StreamLogger(shared_ptr<std::ostream> stream);

    StreamLogger(shared_ptr<std::ostream> stream, shared_ptr<const LoggerDecoration> decoration);

    virtual void write(const Level &level, const string &message) override;
    virtual void write(shared_ptr<const Level> level, const string &message) override;

    protected:
        shared_ptr<std::ostream> stream;
        static void default_greeting();
};

struct StandardLogger : public WriterLogger {
    StandardLogger();
    StandardLogger(const StandardLogger &other);

    StandardLogger(shared_ptr<const LoggerDecoration> decoration);

    virtual void write(const Level &level, const string &message) override;
    virtual void write(shared_ptr<const Level> level, const string &message) override;
    
    protected:
        static void default_greeting();
};

struct ThreadLogger : public Logger {
    ThreadLogger() = delete;
    ThreadLogger(const ThreadLogger &other) = delete;

    ThreadLogger(shared_ptr<Logger> other);

    void write(const Level &level, const string &message) override;
    void write(shared_ptr<const Level> level, const string &message) override;
    void set_level_filter(shared_ptr<const LevelFilter> level_filter) override;

    private:
        std::mutex mut;
        shared_ptr<Logger> logger;
};

struct BiLogger : public Logger {
    BiLogger() = delete;
    BiLogger(const BiLogger &other) = delete;
    BiLogger(shared_ptr<Logger> logger1, shared_ptr<Logger> logger2);

    void write(const Level &level, const string &message) override;
    void write(shared_ptr<const Level> level, const string &message) override;
    void set_level_filter(shared_ptr<const LevelFilter> level_filter) override;

    private:
        shared_ptr<Logger> logger1;
        shared_ptr<Logger> logger2;
};
