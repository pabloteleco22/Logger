#include "log.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::endl;

/** Level **/
Level::Level(Level &other) {
    level_number = other.level_number;
    color = other.color;
    level_name = other.level_name;
    printable = other.printable;
}

Level::Level(Level *other) {
    level_number = other->level_number;
    color = other->color;
    level_name = other->level_name;
    printable = other->printable;
}

Level::Level(std::shared_ptr<Level> other) {
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
    min_level = std::make_shared<Level>(new Level);
}

Logger::Logger(Logger &other) {
    min_level = other.min_level;
}

void Logger::set_min_level(Level &level) {
    min_level = shared_ptr<Level>{new Level{level}};
}

void Logger::set_min_level(std::shared_ptr<Level> level) {
    min_level = level;
}

/** WriterLogger **/
WriterLogger::WriterLogger(shared_ptr<const LoggerDecoration> decoration) : Logger() {
    this->decoration = decoration;
}

WriterLogger::WriterLogger(WriterLogger &other) : Logger(other) {
    this->decoration = other.decoration;
}

/** StreamLogger **/
StreamLogger::StreamLogger(StreamLogger &other) : WriterLogger(other) {
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
    if ((level >= *min_level) and (level.is_printable())) {
        (*stream) << "["
            << decoration->get_decoration()
            << level.get_level_name() << "] " << message << endl;
    }
}

/** StandardLogger **/
StandardLogger::StandardLogger() :
            WriterLogger(shared_ptr<const LoggerDecoration>{new VoidLoggerDecoration}) {}

StandardLogger::StandardLogger(StandardLogger &other) : WriterLogger(other) {
    decoration = other.decoration;
}

StandardLogger::StandardLogger(shared_ptr<const LoggerDecoration> decoration) :
            WriterLogger(decoration) {}

void StandardLogger::write(const Level &level, const string &message) {
    if ((level >= *min_level) and (level.is_printable())) {
        cout << level.get_color() << "["
            << decoration->get_decoration()
            << level.get_level_name() << "]\033[0m " << message << endl;
    }
}

/** ThreadLogger **/
ThreadLogger::ThreadLogger(Logger *other) : Logger() {
    logger = std::shared_ptr<Logger>(other);
}

ThreadLogger::ThreadLogger(shared_ptr<Logger> other) {
    logger = other;
}

void ThreadLogger::write(const Level &level, const string &message) {
    mut.lock();

    logger->write(level, message);

    mut.unlock();
}

void ThreadLogger::set_min_level(Level &level) {
    mut.lock();

    logger->set_min_level(level);

    mut.unlock();
}

void ThreadLogger::set_min_level(shared_ptr<Level> level) {
    mut.lock();

    logger->set_min_level(level);

    mut.unlock();
}

/** BiLogger **/
BiLogger::BiLogger(Logger *logger1, Logger *logger2) {
    this->logger1 = shared_ptr<Logger>(logger1);
    this->logger2 = shared_ptr<Logger>(logger2);
}

BiLogger::BiLogger(shared_ptr<Logger> logger1, shared_ptr<Logger> logger2) {
    this->logger1 = logger1;
    this->logger2 = logger2;
}

void BiLogger::write(const Level &level, const string &message) {
    logger1->write(level, message);
    logger2->write(level, message);
}

void BiLogger::set_min_level(Level &level) {
    logger1->set_min_level(level);
    logger2->set_min_level(level);
}

void BiLogger::set_min_level(shared_ptr<Level> level) {
    logger1->set_min_level(level);
    logger2->set_min_level(level);
}