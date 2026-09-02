#include "Receiver.h"
#include <iostream>

using namespace std;

void Receiver::ProcessFrag(const Fragment &frag) {

  if (buffermap.find(frag.id) == buffermap.end()) {
    buffermap.insert({frag.id, MessageBuffer(frag.total)});
  }
  MessageBuffer &buffer = buffermap.at(frag.id);
  buffer.fragments[frag.seq] = frag;
  buffer.fragsLeft--;

  if (buffer.fragsLeft == 0) {
    string payload;
    for (int i = 0; i < (int)frag.total; i++) {
      payload += buffer.fragments[i].subpayload;
    }
    cout << "Message Received : " << payload << "\n";
    Message done(frag.id, 0, (uint16_t)payload.size(), payload);
    output.push_back(done);
    buffermap.erase(frag.id);
  }
}