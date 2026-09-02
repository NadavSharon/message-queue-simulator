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
    if (id_min == -1 || done.size < size_min) {
      id_min = done.id;
      size_min = done.size;
    }
    if (id_max == -1 || done.size > size_max) {
      id_max = done.id;
      size_max = done.size;
    }
    output.push_back(done);
    buffermap.erase(frag.id);
  }
}