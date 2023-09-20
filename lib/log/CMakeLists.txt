add_library(Level level.cpp)
add_library(LevelFilter levelfilter.cpp)
link_libraries(LevelFilter
    Level
)
add_library(Decoration decoration.cpp)
add_library(Greeter greeter.cpp)
add_library(Logger logger.cpp)
link_libraries(Logger
    Greeter
    Decoration
    LevelFilter
)
add_library(LoggerBuilder loggerbuilder.cpp)
link_libraries(LoggerBuilder
    Logger
)