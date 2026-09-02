#include "DataTypes.h"
#include "Receiver.h"
#include "SystemQueues.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#define NUM_STEPS 800

int RandomBetween(int minVal, int maxVal) {

  return minVal + rand() % (maxVal - minVal + 1);
}

string BuildPayload(int size) {

  int base = RandomBetween(1000, 8999);
  string payload = "";

  for (int i = 0; (int)payload.size() < size; i++) {
    payload += to_string(base + i);
  }

  return payload.substr(0, size);
}

Message MakeRandomMessage(uint32_t id) {

  uint8_t qnum = (uint8_t)RandomBetween(1, 5);
  uint16_t size = (uint16_t)RandomBetween(1, 1000);
  string payload = BuildPayload(size);
  return Message(id, qnum, size, payload);
}

int main() {

  srand(time(NULL));

  QueuesManager queues;
  Receiver receiver;
  uint32_t nextId = 1;

  for (int step = 0; step < NUM_STEPS; step++) {
    // Sometimes create a new message
    if (RandomBetween(0, 1) == 0) {
      Message msg = MakeRandomMessage(nextId);
      nextId++;
      queues.ReceiveMsg(msg);
    }

    // Always try to pull one fragment
    Fragment frag;
    if (queues.PullFrag(frag)) {
      receiver.ProcessFragment(frag);
    }
  }

  cout << "\nSimulation finished after " << NUM_STEPS << " steps.\n";
  cout << "Messages dropped: " << queues.dropCount << "\n";
  cout << "Messages pulled from output: " << receiver.output.size() << "\n";
  cout << "Order and sizes:\n";
  for (int i = 0; i < (int)receiver.output.size(); i++) {
    cout << (i + 1) << ". ID: " << receiver.output[i].id
         << ", Size: " << receiver.output[i].size << " [bytes]" << "\n";
  }
  cout << " -------------------------\n";
  cout << "More Statistics: \n";
  cout << " * Messages created: " << (nextId - 1) << "\n";
  int left = (nextId - 1) - queues.dropCount - receiver.output.size();
  cout << " * Messages left in the system : " << left << "\n";
  cout << "    - Including: " << left - receiver.buffermap.size()
       << " in the queues, and " << receiver.buffermap.size()
       << " in the receiver buffer..\n";
  cout << " * Message (ID " << receiver.id_max << " ) "
       << "is the longest (Sized: " << receiver.size_max << " )\n";
  cout << " * Message (ID " << receiver.id_min << " ) "
       << "is the shortest (Sized: " << receiver.size_min << " )\n";

  return 0;
}
