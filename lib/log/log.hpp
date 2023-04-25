#pragma once

#include <string>
#include <memory>
#include <chrono>
#include <ostream>
#include <mutex>

using std::string;
using std::shared_ptr;

struct Level {
    Level() {};
    Level(Level &other);
    Level(Level *other);
    Level(shared_ptr<Level> other);
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
    Logger(Logger &other);

    virtual ~Logger() {};

    virtual void write(const Level &level, const string &message) = 0;
    virtual void set_min_level(Level &level);
    virtual void set_min_level(shared_ptr<Level> level);

    protected:
        shared_ptr<Level> min_level;
};

struct WriterLogger : public Logger {
    WriterLogger(shared_ptr<const LoggerDecoration> decoration);
    WriterLogger(WriterLogger &other);

    protected:
        shared_ptr<const LoggerDecoration> decoration;
};

struct StreamLogger : public WriterLogger {
    StreamLogger() = delete;
    StreamLogger(StreamLogger &other);

    StreamLogger(shared_ptr<std::ostream> stream);

    StreamLogger(shared_ptr<std::ostream> stream, shared_ptr<const LoggerDecoration> decoration);

    virtual void write(const Level &level, const string &message) override;

    protected:
        shared_ptr<std::ostream> stream;
        static void default_greeting();
};

struct StandardLogger : public WriterLogger {
    StandardLogger();
    StandardLogger(StandardLogger &other);

    StandardLogger(shared_ptr<const LoggerDecoration> decoration);

    virtual void write(const Level &level, const string &message) override;
    
    protected:
        static void default_greeting();
};

struct ThreadLogger : public Logger {
    ThreadLogger() = delete;
    ThreadLogger(ThreadLogger &other) = delete;

    ThreadLogger(Logger *other);
    ThreadLogger(shared_ptr<Logger> other);

    void write(const Level &level, const string &message) override;
    void set_min_level(Level &level) override;
    void set_min_level(shared_ptr<Level> level) override;

    private:
        std::mutex mut;
        shared_ptr<Logger> logger;
};

struct BiLogger : public Logger {
    BiLogger() = delete;
    BiLogger(BiLogger &other) = delete;
    BiLogger(Logger *logger1, Logger *logger2);
    BiLogger(shared_ptr<Logger> logger1, shared_ptr<Logger> logger2);

    void write(const Level &level, const string &message) override;
    void set_min_level(Level &level) override;
    void set_min_level(shared_ptr<Level> level) override;

    private:
        shared_ptr<Logger> logger1;
        shared_ptr<Logger> logger2;
};
