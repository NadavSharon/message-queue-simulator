#pragma once
#include "DataTypes.h"

#include <array>
#include <cstdint>
#include <queue>

struct QueuesManager {

  std::array<std::queue<Fragment>, 5> queues;
  int qidx = 0;                   // Active pull queue index (for RR)
  const int MAX_QUEUE_SIZE = 400; // limit for each queue (num of fragments)
  int dropCount = 0;

  // Processes an incoming message (splits into fragments and pushes to queues)
  void ReceiveMsg(const Message &msg);

  // Pulls a fragment from the queue. Returns true if successful, false if empty
  bool PullFrag(Fragment &frag);
};
