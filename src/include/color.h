// COLOR HEADER

#pragma once

#include <ostream>

namespace sqlcheck {

enum ColorCode {

  FG_DEFAULT = 39,
  FG_BLACK = 30,
  FG_RED = 31,
  FG_GREEN = 32,
  FG_YELLOW = 33,
  FG_BLUE = 34,
  FG_MAGENTA = 35,
  FG_CYAN = 36,
  FG_LIGHT_GRAY = 37,

  FG_DARK_GRAY = 90,
  FG_LIGHT_RED = 91,
  FG_LIGHT_GREEN = 92,
  FG_LIGHT_YELLOW = 93,
  FG_LIGHT_BLUE = 94,
  FG_LIGHT_MAGENTA = 95,
  FG_LIGHT_CYAN = 96,
  FG_WHITE = 97,

  BG_RED = 41,
  BG_GREEN = 42,
  BG_BLUE = 44,
  BG_DEFAULT = 49

};

class ColorModifier {

 public:
  ColorModifier(ColorCode color_code, bool enable_color)
 : color_code_(color_code),
   enable_color_(enable_color) {
  }

  friend std::ostream& operator<<(std::ostream& os, const ColorModifier& color_modifier) {
    if(color_modifier.enable_color_ == true) {
      return os << "\033[" << color_modifier.color_code_ << "m";
    }
    else {
      return os;
    }
  }

 private:

  // color code
  ColorCode color_code_;

  // enable color
  bool enable_color_;

};

}  // namespace sqlcheck
