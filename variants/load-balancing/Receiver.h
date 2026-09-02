#pragma once
#include "DataTypes.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

struct MessageBuffer {

  int fragsLeft;
  std::vector<Fragment> fragments;

  MessageBuffer(int fragsLeft) : fragsLeft(fragsLeft) {
    fragments.resize(fragsLeft);
  }
};

struct Receiver {

  std::unordered_map<uint32_t, MessageBuffer> buffermap;
  std::vector<Message> output;

  void ProcessFrag(const Fragment &frag);
};