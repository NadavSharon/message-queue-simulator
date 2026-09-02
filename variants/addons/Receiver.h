#pragma once
#include "DataTypes.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

struct MessageBuffer {

  int fragsLeft;
  std::vector<Fragment> fragments;

  // Constructor
  MessageBuffer(int totalFrags) : fragsLeft(totalFrags) {
    fragments.reserve(totalFrags); // Pre-allocate memory
  }
};

struct Receiver {

  std::unordered_map<uint32_t, MessageBuffer> buffermap;
  std::vector<Message> output; // messages that finished, in order
  int id_min = -1;
  int id_max = -1;
  uint16_t size_min = 0;
  uint16_t size_max = 0;
  // Store fragment. if all arrived, reassemble into output
  void ProcessFragment(const Fragment &frag);
};