#include "log.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::endl;

/** Level **/
Level::Level(const Level &other) {
    level_number = other.level_number;
    color = other.color;
    level_name = other.level_name;
    printable = other.printable;
}

Level::Level(const Level *other) {
    level_number = other->level_number;
    color = other->color;
    level_name = other->level_name;
    printable = other->printable;
}

Level::Level(std::shared_ptr<const Level> other) {
    level_number = other->level_number;
    color = other->level_number;
    level_name = other->level_name;
    printable = other->printable;
};

bool Level::operator>=(const Level &other) const {
    return level_number >= other.level_number;
}

bool Level::operator>(const Level &other) const {
    return level_number > other.level_number;
}

bool Level::operator<=(const Level &other) const {

    return level_number <= other.level_number;
}

bool Level::operator<(const Level &other) const {

    return level_number < other.level_number;
}

bool Level::operator==(const Level &other) const {

    return level_number == other.level_number;
}

bool Level::operator!=(const Level &other) const {

    return level_number != other.level_number;
}


string Level::get_color() const {
    return color;
}

string Level::get_level_name() const {
    return level_name;
}

bool Level::is_printable() const {
    return printable;
}

/** DefaultFilter **/
bool DefaultFilter::filter(const Level &filter) const {
    return filter.is_printable();
}

/** UserCustomFilter **/
bool UserCustomFilter::filter(const Level &filter) const {
    return filter.is_printable() and custom_filter(filter);
}

/** VoidLoggerDecoration **/
string VoidLoggerDecoration::get_decoration() const {
    return "";
}

/** TimedLoggerDecoration **/
TimedLoggerDecoration::TimedLoggerDecoration() : LoggerDecoration() {
    start_time = std::chrono::steady_clock::now();
}

TimedLoggerDecoration::TimedLoggerDecoration(TimedLoggerDecoration &other) : LoggerDecoration() {
    start_time = other.start_time;
}

string TimedLoggerDecoration::get_decoration() const {
    std::ostringstream os;
    os << std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start_time).count() << " | ";
    
    return os.str();
}

/** HourLoggerDecoration **/
string HourLoggerDecoration::get_decoration() const {
    std::ostringstream os;

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::time_t t_now = std::chrono::system_clock::to_time_t(now);
    os << std::put_time(std::localtime(&t_now), "%T") << " | ";

    return os.str();
}

/** Logger **/
Logger::Logger() {
    level_filter = shared_ptr<const LevelFilter>(new DefaultFilter);
}

Logger::Logger(const Logger &other) {
    level_filter = other.level_filter;
}

void Logger::set_level_filter(std::shared_ptr<const LevelFilter> level_filter) {
    this->level_filter = level_filter;
}

/** WriterLogger **/
WriterLogger::WriterLogger(shared_ptr<const LoggerDecoration> decoration) : Logger() {
    this->decoration = decoration;
}

WriterLogger::WriterLogger(const WriterLogger &other) : Logger(other) {
    this->decoration = other.decoration;
}

/** StreamLogger **/
StreamLogger::StreamLogger(const StreamLogger &other) : WriterLogger(other) {
    this->stream = other.stream;
    this->decoration = other.decoration;
}

StreamLogger::StreamLogger(shared_ptr<std::ostream> stream) :
                WriterLogger(shared_ptr<const LoggerDecoration>{new VoidLoggerDecoration}) {
    this->stream = stream;
    decoration = shared_ptr<const LoggerDecoration>{new VoidLoggerDecoration};
}

StreamLogger::StreamLogger(shared_ptr<std::ostream> stream, shared_ptr<const LoggerDecoration> decoration) :
                WriterLogger(decoration) {
    this->stream = stream;
    this->decoration = decoration;
}

void StreamLogger::write(const Level &level, const string &message) {
    if (level_filter->filter(level)) {
        (*stream) << "["
            << decoration->get_decoration()
            << level.get_level_name() << "] " << message << endl;
    }
}

void StreamLogger::write(shared_ptr<const Level> level, const string &message) {
    write(*level, message);
}

/** StandardLogger **/
StandardLogger::StandardLogger() :
            WriterLogger(shared_ptr<const LoggerDecoration>{new VoidLoggerDecoration}) {}

StandardLogger::StandardLogger(const StandardLogger &other) : WriterLogger(other) {
    decoration = other.decoration;
}

StandardLogger::StandardLogger(shared_ptr<const LoggerDecoration> decoration) :
            WriterLogger(decoration) {}

void StandardLogger::write(const Level &level, const string &message) {
    if (level_filter->filter(level)) {
        cout << level.get_color() << "["
            << decoration->get_decoration()
            << level.get_level_name() << "]\033[0m " << message << endl;
    }
}

void StandardLogger::write(shared_ptr<const Level> level, const string &message) {
    write(*level, message);
}

/** ThreadLogger **/
ThreadLogger::ThreadLogger(shared_ptr<Logger> other) {
    logger = other;
}

void ThreadLogger::write(const Level &level, const string &message) {
    mut.lock();

    logger->write(level, message);

    mut.unlock();
}

void ThreadLogger::write(shared_ptr<const Level> level, const string &message) {
    write(*level, message);
}

void ThreadLogger::set_level_filter(shared_ptr<const LevelFilter> level_filter) {
    logger->set_level_filter(level_filter);
}

/** BiLogger **/
BiLogger::BiLogger(shared_ptr<Logger> logger1, shared_ptr<Logger> logger2) {
    this->logger1 = logger1;
    this->logger2 = logger2;
}

void BiLogger::write(const Level &level, const string &message) {
    logger1->write(level, message);
    logger2->write(level, message);
}

void BiLogger::write(shared_ptr<const Level> level, const string &message) {
    write(*level, message);
}

void BiLogger::set_level_filter(shared_ptr<const LevelFilter> level_filter) {
    logger1->set_level_filter(level_filter);
    logger2->set_level_filter(level_filter);
}