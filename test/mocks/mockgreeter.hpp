#include "greeter.hpp"
#include <gmock/gmock.h>
#include <string>

class MockGreeter : public simple_logger::Greeter {
    public:
        MOCK_METHOD(std::string, greetings, (const std::string &m), (const, override));
};