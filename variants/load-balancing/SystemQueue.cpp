#include "SystemQueue.h"
#include "DataTypes.h"
#include <iostream>

using namespace std;

void ManageQueue::ReceiveMsg(const Message &msg) {
  int totalfrags = (msg.size + 99) / 100;

  /*
  if (msg.qnum < 1 || msg.qnum > 5) {
    cout << "[ERROR] Invalid queue (Index: )" << msg.qnum << "\n";
    return;
  }
    // Not Needed..
  */

  int chosen[totalfrags]; // Idx = seq , Val = queue
  int add[5] = {0};       // Idx = queue , Val = number of new fragments

  for (int i = 0; i < totalfrags; i++) {
    int minSize = MAX_QUEUE_SIZE;
    chosen[i] = 0;

    for (int q = 0; q < 5; q++) {
      if (queues[q].size() + add[q] < minSize) {
        minSize = queues[q].size() + add[q];
        chosen[i] = q;
      }
    }

    if (queues[chosen[i]].size() + add[chosen[i]] + 1 > MAX_QUEUE_SIZE) {
      // DROP + return
      dropCount++;
      cout << "[DROP] Message (ID: " << (int)msg.id << ") "
           << "has been dropped (Queues limit reached) \n";
      return;
    }

    add[chosen[i]]++;
  }

  for (int i = 0; i < totalfrags; i++) {
    string FragPayload = msg.payload.substr(i * 100, 100);
    Fragment frag(msg.id, i, totalfrags, FragPayload);
    queues[chosen[i]].push(frag);
  }
}

bool ManageQueue::PullFrag(Fragment &frag) {
  for (int i = 0; i < 5; i++) {
    if (queues[qidx].empty()) {
      qidx = (qidx + 1) % 5;
      continue;
    }
    frag = queues[qidx].front();
    queues[qidx].pop();
    qidx = (qidx + 1) % 5;
    return true;
  }
  return false;
}