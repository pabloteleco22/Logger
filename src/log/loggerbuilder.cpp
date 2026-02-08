#include "loggerbuilder.hpp"
#include "decoration.hpp"
#include "greeter.hpp"
#include "logger.hpp"
#include <memory>

using namespace simple_logger;

using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

/** StandardLoggerBuilder **/
unique_ptr<Logger> StandardLoggerBuilder::build() {
    return make_unique<StandardLogger>(decoration, *greeter, greeting_string);
}

StandardLoggerBuilder &
StandardLoggerBuilder::set_decoration(shared_ptr<LoggerDecoration> decoration) {
    this->decoration = decoration;

    return *this;
}

StandardLoggerBuilder &StandardLoggerBuilder::set_greeter(shared_ptr<Greeter> greeter) {
    this->greeter = greeter;

    return *this;
}

StandardLoggerBuilder &StandardLoggerBuilder::set_greeting_string(const string &greeting_string) {
    this->greeting_string = greeting_string;

    return *this;
}

StandardLoggerBuilder &StandardLoggerBuilder::reset_config() {
    greeting_string = StandardLogger::default_greeting_message;
    greeter = default_greeter;
    decoration = default_decoration;

    return *this;
}

/** StreamLoggerBuilder **/
StreamLoggerBuilder::StreamLoggerBuilder(std::ostream *stream) { this->stream = stream; }

unique_ptr<Logger> StreamLoggerBuilder::build() {
    return make_unique<StreamLogger>(stream, decoration, *greeter, greeting_string);
}

StreamLoggerBuilder &StreamLoggerBuilder::set_stream(std::ostream *stream) {
    this->stream = stream;

    return *this;
}

StreamLoggerBuilder &StreamLoggerBuilder::set_decoration(shared_ptr<LoggerDecoration> decoration) {
    this->decoration = decoration;

    return *this;
}

StreamLoggerBuilder &StreamLoggerBuilder::set_greeter(shared_ptr<Greeter> greeter) {
    this->greeter = greeter;

    return *this;
}

StreamLoggerBuilder &StreamLoggerBuilder::set_greeting_string(const string &greeting_string) {
    this->greeting_string = greeting_string;

    return *this;
};

StreamLoggerBuilder &StreamLoggerBuilder::reset_config() {
    greeting_string = StreamLogger::default_greeting_message;
    greeter = default_greeter;
    decoration = default_decoration;

    return *this;
}
