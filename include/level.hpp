#pragma once

#include <string>

using std::string;

namespace simple_logger {
struct Level {
    Level(const unsigned short level_number = 0, const string color = "",
          const string level_name = "");
    Level(const Level *other);
    bool operator>=(const Level &other) const;
    bool operator>(const Level &other) const;
    bool operator<=(const Level &other) const;
    bool operator<(const Level &other) const;
    bool operator==(const Level &other) const;
    bool operator!=(const Level &other) const;
    string get_color() const;
    string get_level_name() const;

  protected:
    const unsigned short level_number{0};
    const string color;
    const string level_name;
};

struct Debug : public Level {
    using Level::Level;
    Debug() : Level(51, "\033[1;32m", "Debug") {}
};

struct Info : public Level {
    using Level::Level;
    Info() : Level(102, "\033[1;34m", "Info") {}
};

struct Warning : public Level {
    using Level::Level;
    Warning() : Level(153, "\033[1;33m", "Warn") {}
};

struct Error : public Level {
    using Level::Level;
    Error() : Level(204, "\033[1;31m", "Error") {}
};

struct Critical : public Level {
    using Level::Level;
    Critical() : Level(229, "\033[1;95m", "Critical") {}
};
}; // namespace simple_logger
