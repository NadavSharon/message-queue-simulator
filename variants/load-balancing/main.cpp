#include "DataTypes.h"
#include "Receiver.h"
#include "SystemQueue.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#define NUM_STEPS 800

int Random(int min, int max) { return min + rand() % (max - min + 1); }

string BuildPayload(int size) {

  int base = Random(1000, 8999);
  string payload = "";

  for (int i = 0; (int)payload.size() < size; i++) {
    payload += to_string(base + i);
  }
  return payload.substr(0, size);
}

Message CreateMsg(uint32_t id) {
  uint8_t qnum = (uint8_t)Random(1, 5);
  uint16_t size = (uint16_t)Random(1, 1000);
  string payload = BuildPayload(size);
  return Message(id, qnum, size, payload);
}

int main() {

  srand(time(NULL));

  ManageQueue ManQ;
  Receiver Rec;
  uint32_t id = 1;

  for (int i = 0; i < NUM_STEPS; i++) {

    if (Random(0, 1) == 0) {
      Message msg = CreateMsg(id);
      id++;
      ManQ.ReceiveMsg(msg);
    }

    Fragment frag;
    if (ManQ.PullFrag(frag)) {
      Rec.ProcessFrag(frag);
    }
  }

  cout << "Simulation for " << NUM_STEPS << " Steps: \n";
  cout << "Number of dropped messages: " << ManQ.dropCount << "\n";
  cout << "Number of pulled messages: " << Rec.output.size() << "\n";
  cout << "Messages ID and Size : \n";
  for (int i = 0; i < (int)Rec.output.size(); i++) {
    cout << "Message ID: " << Rec.output[i].id
         << " ,Size: " << Rec.output[i].size << " [bytes] \n";
  }
  return 0;
}
