#pragma once
#include <cstdint>
#include <string>

struct Message {

  // Metadata (7 bytes):
  uint32_t id;   // Unique ID (1 to 1M)
  uint8_t qnum;  // Input queue number (1 to 5)
  uint16_t size; // Total size of payload [chars/bytes]
  // Data:
  std::string payload; // Message content

  // Constructor
  Message(uint32_t msgId, uint8_t targetQ, uint16_t msgSize,
          const std::string &data)
      : id(msgId), qnum(targetQ), size(msgSize), payload(data) {}
};

struct Fragment {

  // Metadata (6 bytes):
  uint32_t id;   // ID of the original message
  uint8_t seq;   // Sequence number (index)
  uint8_t total; // Total fragments
  // Data:
  std::string payload; // Content (up to 100 chars)

  // Constructor
  Fragment(uint32_t msgId, uint8_t seqNum, uint8_t totalFrags,
           const std::string &data)
      : id(msgId), seq(seqNum), total(totalFrags), payload(data) {}

  // Default constructor
  Fragment() : id(0), seq(0), total(0), payload("") {}
};