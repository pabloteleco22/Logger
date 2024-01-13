#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <logger.hpp>
#include <mocklogger.hpp>
#include <iostream>
#include <mutex>

using namespace simple_logger;

using ::testing::_;
using ::testing::NiceMock;

Logger::LoggerStreamResponse::End end;

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

    lsr << end;
}

TEST(LoggerStreamResponseTests, HappyPathWithNotEmptyMessage) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(1);

    Logger::LoggerStreamResponse lsr = logger << level;

    lsr << "Hello" << 2 << std::endl << std::boolalpha << true;
    lsr << end;
}

TEST(LoggerStreamResponseTests, DontWriteAnyMessageOnEndlReception) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(0);

    Logger::LoggerStreamResponse lsr = logger << level;

    lsr << std::endl;
}

TEST(ThreadLoggerTests, CallLogger) {
    MockLogger logger;
    Debug level;

    EXPECT_CALL(logger, write(_, _)).Times(1);

    ThreadLogger threadLogger(&logger);

    threadLogger << level << end;
}

TEST(BiLoggerTests, CallTwoLoggers) {
    MockLogger logger1;
    MockLogger logger2;
    Debug level;

    EXPECT_CALL(logger1, write(_, _)).Times(1);
    EXPECT_CALL(logger2, write(_, _)).Times(1);

    BiLogger biLogger(&logger1, &logger2);

    biLogger << level << end;
}