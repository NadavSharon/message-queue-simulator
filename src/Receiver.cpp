#include "Receiver.h"
#include <iostream>

using namespace std;

void Receiver::ProcessFragment(const Fragment &frag) {

  if (buffermap.find(frag.id) == buffermap.end()) {
    buffermap.insert({frag.id, MessageBuffer(frag.total)});
  }
  MessageBuffer &buffer = buffermap.at(frag.id);
  buffer.fragments.push_back(frag);
  buffer.fragsLeft--;
  if (buffer.fragsLeft == 0) {
    string payload;
    for (const Fragment &f : buffer.fragments) {
      payload += f.payload;
    }
    cout << "Received message: " << payload << "\n";

    Message done(frag.id, 0, (uint16_t)payload.size(), payload);
    output.push_back(done);
    buffermap.erase(frag.id);
  }
}