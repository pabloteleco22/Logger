#include <gmock/gmock.h>
#include <logger.hpp>
#include <string>

using namespace simple_logger;

using std::string;

struct MockLogger : public Logger {
    MOCK_METHOD(void, write, (const Level &level, const string &message),
                (const, override));
    MOCK_METHOD(void, set_level_filter, (const LevelFilter *level_filter),
                (override));
};