#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <level.hpp>
#include <mocklogger.hpp>
#include <iostream>

using namespace simple_logger;

using ::testing::_;

TEST(LogLevelTest, LevelHierarchy) {
    Debug debug;
    Info info;
    Warning warning;
    Error error;

    ASSERT_GT(error, warning);
    ASSERT_GT(warning, info);
    ASSERT_GT(info, debug);
}

TEST(LoggerStreamResponseTests, HappyPathWithEmptyMessage) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(1);

    Logger::LoggerStreamResponse lsr = logger << level;

    lsr << Logger::LoggerStreamResponse::end;
}

TEST(LoggerStreamResponseTests, HappyPathWithNotEmptyMessage) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(1);

    Logger::LoggerStreamResponse lsr = logger << level;

    lsr << "Hello" << 2 << std::endl << std::boolalpha << true;
    lsr << Logger::LoggerStreamResponse::end;
}

TEST(LoggerStreamResponseTests, DontWriteAnyMessageOnEndlReception) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(0);

    Logger::LoggerStreamResponse lsr = logger << level;

    lsr << std::endl;
}