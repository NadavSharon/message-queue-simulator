#pragma once
#include "DataTypes.h"

#include <array>
#include <cstdint>
#include <queue>

struct ManageQueue {
  std::array<std::queue<Fragment>, 5> queues;
  int qidx = 0;
  const int MAX_QUEUE_SIZE = 400;
  int dropCount = 0;

  void ReceiveMsg(const Message &msg);
  bool PullFrag(Fragment &frag);
};