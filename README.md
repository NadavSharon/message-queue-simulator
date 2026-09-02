# Message queue simulator

A small C++ simulator of a communications path: a message source, five finite input queues, fragmentation, Round-Robin dequeue, and a receiver that reassembles messages.

Personal side project for exploring queueing, fragmentation, and reassembly.

## What it does

Each time step (~800 total):

1. With ~50% probability, create a random message (`id`, target queue 1–5, payload 1–1000 bytes).
2. Split it into fragments of at most 100 bytes and enqueue **all** fragments of that message on the chosen input queue (sticky).
3. If the queue cannot hold the whole message, **drop** it (all-or-nothing).
4. Always try to pull **one** fragment, Round-Robin across the five queues.
5. The receiver collects fragments by `id`. When all have arrived, it concatenates the payload and records the completed message.

At the end it prints drop count, completed messages (order and sizes), and how many were created.

## Layout

| Folder | Contents |
|---|---|
| [`src/`](src/) | Main simulator |
| [`variants/addons/`](variants/addons/) | Extra stats: min/max size and leftover messages |
| [`variants/load-balancing/`](variants/load-balancing/) | Fragments sprayed / joined to shorter queues |

## Build and run

Needs `g++`. From a code folder (for example `src/`):

```bash
g++ *.cpp -o sim.exe
./sim.exe
```

## Design choices (short)

- **Drop whole message** if it does not fit: avoids stuck incomplete buffers and wasted queue slots.
- **Round-Robin on dequeue** is fair *service*, not load balancing of occupancy. Target queue comes from `qnum`.
- **`push_back` reassembly** assumes in-order fragments (true here because of sticky + FIFO). Out-of-order would use `resize` + `fragments[seq]`.
- Queue limit is **400 fragments**, not bytes — the service unit is one fragment per step.
