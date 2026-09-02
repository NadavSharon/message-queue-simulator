#pragma once
#include <cstdint>
#include <string>

struct Message {
  uint32_t id;
  uint8_t qnum;
  uint16_t size;

  std::string payload;

  Message(uint32_t id, uint8_t qnum, uint16_t size, const std::string &payload)
      : id(id), qnum(qnum), size(size), payload(payload) {}
};

struct Fragment {
  uint32_t id;
  uint8_t seq;
  uint8_t total;

  std::string subpayload;

  Fragment(uint32_t id, uint8_t seq, uint8_t total,
           const std::string &subpayload)
      : id(id), seq(seq), total(total), subpayload(subpayload) {}
  Fragment() : id(0), seq(0), total(0), subpayload("") {}
};