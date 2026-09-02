#include "SystemQueues.h"
#include <iostream>

using namespace std;

void QueuesManager::ReceiveMsg(const Message &msg) {

  int totalFrags = (msg.size + 99) / 100; // Calculate count of fragments

  if (msg.qnum < 1 || msg.qnum > 5) {
    cout << "Invalid queue number: " << (int)msg.qnum << "\n";
    return;
  }

  if (queues[msg.qnum - 1].size() + totalFrags > MAX_QUEUE_SIZE) {
    dropCount++;
    cout << "[DROP] Message ID: " << msg.id << " has been dropped, Queue "
         << (int)msg.qnum << " reached its limit" << "\n";
    return;
  }

  for (int i = 0; i < totalFrags; i++) {
    string fragPayload = msg.payload.substr(i * 100, 100);
    Fragment frag(msg.id, i, totalFrags, fragPayload);
    queues[msg.qnum - 1].push(frag); // Push object to queue
  }
}

bool QueuesManager::PullFrag(Fragment &frag) {

  for (int i = 0; i < 5; i++) {
    if (queues[qidx].empty()) {
      qidx = (qidx + 1) % 5;
      continue; // Check next queue
    }

    frag = queues[qidx].front();
    queues[qidx].pop();

    qidx = (qidx + 1) % 5; // Move to next for fairness
    return true;
  }

  return false; // All queues empty
}