#include "decoration.hpp"

using namespace simple_logger;

/** VoidLoggerDecoration **/
string VoidLoggerDecoration::get_decoration() const {
    return "";
}

DecorationBundler::DecorationBundler(std::vector<LoggerDecoration*> &decoration_list, const string separator) : separator{separator} {
    this->decoration_list = &decoration_list;
}

string DecorationBundler::get_decoration() const {
    string decoration = "";
    
    for (auto log_dec : (*decoration_list)) {
        decoration += log_dec->get_decoration();
    }
    
    return decoration;
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
    os << std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start_time) << " | ";
    
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