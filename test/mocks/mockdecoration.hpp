#include "decoration.hpp"
#include <gmock/gmock.h>
#include <string>

class MockLoggerDecoration : public simple_logger::LoggerDecoration {
  public:
    MOCK_METHOD(std::string, get_decoration, (), (const, override));
};