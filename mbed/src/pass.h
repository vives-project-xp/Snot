#pragma once

#include <iostream>
#include <vector>

// 6 bits passwords -> hex

namespace Helpers{
  class PassGen{
    public:
      static std::vector<char> genPass(void);

    private:
      std::vector<char> hex_chars {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  };
}