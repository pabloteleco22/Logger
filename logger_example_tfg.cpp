#define EX4

#ifdef EX1
#include "lib/log/log.hpp"

int main() {
    using namespace simple_logger;

    Debug debug;
    Info info;
    Warning warning;
    Error error;

    StandardLogger standard_logger;

    standard_logger.write(debug, "Mensaje de depuración");
    standard_logger.write(info, "Mensaje de información");
    standard_logger.write(warning, "Mensaje de aviso");
    standard_logger.write(error, "Mensaje de error");
    
    return 0;
}
#elif defined(EX2)
#include "lib/log/log.hpp"
#include <fstream>

int main() {
    using namespace simple_logger;
    using std::ostream;
    using std::fstream;
    using std::ios;

    Debug debug;
    Info info;
    Warning warning;
    Error error;

    shared_ptr<LoggerDecoration> decoration{
        new HourLoggerDecoration};

    StreamLogger stream_logger{shared_ptr<ostream>{
        new fstream{"registro.log", ios::out}}, decoration};

    stream_logger.write(debug, "Mensaje de depuración");
    stream_logger.write(info, "Mensaje de información");
    stream_logger.write(warning, "Mensaje de aviso");
    stream_logger.write(error, "Mensaje de error");
    
    return 0;
}
#elif defined(EX3)
#include "lib/log/log.hpp"
#include <fstream>
#include <string>

int main() {
    using namespace simple_logger;
    using std::ostream;
    using std::fstream;
    using std::ios;

    Debug debug;
    Info info;
    Warning warning;
    Error error;

    shared_ptr<LoggerDecoration> time_decoration{
        new TimedLoggerDecoration};

    shared_ptr<LoggerDecoration> hour_decoration{
        new HourLoggerDecoration};
    
    shared_ptr<Greeter> custom_greeter{
        new UserCustomGreeter{
            [&hour_decoration](const string &) {
                string message{"Logger empezado a las " +
                    hour_decoration->get_decoration()};

                // Para eliminar el separador que imprime
                // hour_decoration al final
                message.pop_back();
                message.pop_back();

                return message;
            }
        }
    };

    BiLogger bi_logger{
        shared_ptr<Logger>{new StandardLogger{
            hour_decoration, custom_greeter}},
        shared_ptr<Logger>{new StreamLogger{
            shared_ptr<ostream>{
                new fstream{"registro.log", ios::out}},
            time_decoration,
            custom_greeter
        }}
    };

    bi_logger.write(debug, "Mensaje de depuración");
    bi_logger.write(info, "Mensaje de información");
    bi_logger.write(warning, "Mensaje de aviso");
    bi_logger.write(error, "Mensaje de error");
    
    return 0;
}
#elif defined(EX4)
#include "lib/log/log.hpp"

int main() {
    using namespace simple_logger;

    Debug debug;
    Info info;
    Warning warning;
    Error error;

    StandardLogger standard_logger;

    shared_ptr<LevelFilter> custom_filter{
        new UserCustomFilter{
            [&debug, &warning](const Level &level) {
                return ((level == debug) || (level >= warning));
            }
        }
    };

    standard_logger.set_level_filter(custom_filter);

    standard_logger.write(debug, "Mensaje de depuración");
    standard_logger.write(info, "Mensaje de información");
    standard_logger.write(warning, "Mensaje de aviso");
    standard_logger.write(error, "Mensaje de error");
    
    return 0;
}
#elif defined(EX5)
#elif defined(EX6)
#elif defined(EX7)
#elif defined(EX8)
#elif defined(EX9)
#endif