# R-Type Network Protocol

This document specifies the binary protocol spoken between `r-type_client` and
`r-type_server`. It is meant to be enough to write a new client (or server)
without reading the source code.

The key words **MUST**, **MUST NOT**, **SHOULD** and **MAY** are to be
interpreted as described in [RFC 2119](https://www.rfc-editor.org/rfc/rfc2119).

## Contents

1. [Overview](#1-overview)
2. [Packet format](#2-packet-format)
3. [Main server messages](#3-main-server-messages)
4. [Room messages](#4-room-messages)
5. [Snapshot format](#5-snapshot-format)
6. [Event format](#6-event-format)
7. [Liveness and disconnection](#7-liveness-and-disconnection)
8. [Limits and validation](#8-limits-and-validation)
9. [Security considerations](#9-security-considerations)
10. [Examples](#10-examples)

## 1. Overview

- All communication uses **UDP** over IPv4. There is no TCP channel.
- All multi-byte integers are **unsigned and big-endian** (network byte order).
- The server listens on a **main port** (given with `-p`). The main server only
  handles connection and matchmaking: listing, creating and joining rooms.
- Each **room** (one game instance, up to 4 players) listens on its **own UDP
  port**, chosen by the operating system and announced by the main server.
  The room port is on the same IP address as the main server.
- The server identifies a client by its **source address and port**. A client
  **MUST** use a single UDP socket for its whole session (main server and
  rooms alike).
- The server is **authoritative**: clients only send inputs; the room simulates
  the game at 60 ticks per second and sends the full world state every tick.

## 2. Packet format

Every datagram carries exactly one message: a 6-byte header followed by the
payload.

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Magic (0x38A2)        |          Message id           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        Payload length         |   Payload (length bytes) ...  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

| Field          | Size    | Description                                         |
|----------------|---------|-----------------------------------------------------|
| Magic          | 2 bytes | Always `0x38A2`.                                    |
| Message id     | 2 bytes | Identifies the message, see sections 3 and 4.      |
| Payload length | 2 bytes | Number of payload bytes. MUST equal datagram size − 6. |
| Payload        | n bytes | Message-specific content.                           |

A datagram is at most **4096 bytes** (header included), so a payload is at most
**4090 bytes**.

A receiver **MUST** silently drop a datagram that is shorter than 6 bytes, has a
wrong magic number, a length that does not match the datagram size, or an
unknown message id. The protocol has no error message: requests that cannot be
honoured are ignored, unless stated otherwise below.

## 3. Main server messages

These messages are exchanged on the main port.

### Client → main server

| Id     | Name          | Payload      | Answer                         |
|--------|---------------|--------------|--------------------------------|
| `0x65` | `CONNECT`     | username     | `CONNECTED`                    |
| `0x66` | `DISCONNECT`  | —            | none                           |
| `0x67` | `CREATE_ROOM` | —            | `ROOM_PORT`, or `ROOM_LIST`    |
| `0x68` | `LIST_ROOMS`  | —            | `ROOM_LIST`                    |
| `0x69` | `JOIN_ROOM`   | room id (1)  | `ROOM_PORT`, or `ROOM_LIST`    |

- **`CONNECT`** — The payload is the username: 1 to 12 bytes, with no byte below
  `0x20` and no `0x7F` (UTF-8 is accepted). An invalid username gets no answer.
  Sending `CONNECT` again from the same address only refreshes the username.
  Clients SHOULD connect before sending other requests, although the current
  server does not enforce it.
- **`DISCONNECT`** — The server forgets the client.
- **`CREATE_ROOM`** — Creates a room and answers with its port. When the server
  already runs its maximum number of rooms (see section 8), it answers with
  `ROOM_LIST` instead.
- **`LIST_ROOMS`** — Answers with the rooms that can be joined.
- **`JOIN_ROOM`** — The payload is the id of the room to join. The server answers
  with the room port if the room exists, its game has not started and it has
  fewer than 4 players. Otherwise it answers with an up-to-date `ROOM_LIST`.

After answering `CREATE_ROOM` or `JOIN_ROOM` with `ROOM_PORT`, the main server
forgets the client: it now belongs to the room.

### Main server → client

| Id     | Name              | Payload             |
|--------|-------------------|---------------------|
| `0x01` | `CONNECTED`       | —                   |
| `0x02` | `SERVER_SHUTDOWN` | —                   |
| `0x03` | `ROOM_LIST`       | room ids (n × 1)    |
| `0x08` | `ROOM_PORT`       | port (2)            |

- **`ROOM_LIST`** — One byte per joinable room (game not started, fewer than 4
  players). An empty payload means there is no joinable room.
- **`ROOM_PORT`** — The UDP port of the room to talk to, on the main server's
  IP address.
- **`SERVER_SHUTDOWN`** — The server is stopping. Clients SHOULD go back to their
  connection screen.

## 4. Room messages

These messages are exchanged on a room's port.

### Client → room

| Id     | Name          | Payload        | When          |
|--------|---------------|----------------|---------------|
| `0x65` | `CONNECT`     | username       | lobby         |
| `0x66` | `DISCONNECT`  | —              | any time      |
| `0x6A` | `LEAVE_ROOM`  | —              | any time      |
| `0x6B` | `START_GAME`  | level (1)      | lobby         |
| `0x6D` | `HEARTBEAT`   | —              | any time      |
| `0x79` | `MOVE`        | direction (1)  | game          |
| `0x7A` | `SHOOT`       | —              | game          |

- **`CONNECT`** — Joins the room with the same username rules as on the main
  server. The room answers `CONNECTED` and broadcasts `PLAYER_LIST` to all of
  its players. It is ignored when the game has started or when the room already
  has 4 players. Player ids are assigned from 1 upwards, in joining order, and
  are never reused within a room.
- **`DISCONNECT`**, **`LEAVE_ROOM`** — Both remove the player from the room. Before
  the game starts, the room broadcasts the new `PLAYER_LIST`; during the game it
  removes the player's ship and broadcasts `PLAYER_LEFT` with reason 0. The
  leaving client gets no answer.
- **`START_GAME`** — The payload is the level to play, from 1 to the number of
  levels the server defines (8 by default). Only a player of the room can start
  the game, and only the first valid request counts: the room broadcasts
  `GAME_STARTED` and ignores later `START_GAME` messages.
- **`HEARTBEAT`** — Keeps the player alive (see section 7). It MAY also be sent
  to the main server, which ignores it.
- **`MOVE`** — Moves the player's ship one step on the next tick:

  | Direction | Meaning |
  |-----------|---------|
  | `1`       | up      |
  | `2`       | right   |
  | `3`       | down    |
  | `4`       | left    |

  A step is 7 units (10.5 with a speed power-up). Moves along the same axis do
  not add up within one tick, so a client SHOULD send one `MOVE` per held
  direction per frame (about 60 per second); sending `1` and `2` in the same
  frame moves diagonally. The ship stays inside the playfield.
- **`SHOOT`** — Fires a missile if at least 0.5 s has passed since the player's
  last shot. Its damage grows from 5 to 20 with the time spent without shooting
  (2 s to fully charge), plus 15 with a damage power-up.

`MOVE` and `SHOOT` from an address that is not a player of the room are ignored.
Extra payload bytes are ignored.

### Room → client

| Id     | Name              | Payload                           |
|--------|-------------------|-----------------------------------|
| `0x01` | `CONNECTED`       | —                                 |
| `0x02` | `SERVER_SHUTDOWN` | —                                 |
| `0x05` | `PLAYER_LIST`     | players, see below                |
| `0x06` | `GAME_STARTED`    | level (1)                         |
| `0x14` | `SNAPSHOT`        | entities, see section 5           |
| `0x40` | `EVENTS`          | events, see section 6             |
| `0x51` | `GAME_OVER`       | result (1)                        |
| `0x52` | `PLAYER_LEFT`     | player id (1), reason (1)         |

- **`PLAYER_LIST`** — Sent to every player whenever the list changes before the
  game starts. For each player, in joining order:

  | Size     | Field                    |
  |----------|--------------------------|
  | 1        | player id                |
  | variable | username (not terminated) |
  | 1        | `0x00` terminator        |

- **`GAME_STARTED`** — The level being played. Clients switch to the game view;
  the level selects the background.
- **`GAME_OVER`** — `1` for a victory, `0` for a defeat. The game is won when the
  level's boss is destroyed (levels without a boss are won by surviving until
  the end of the level's script, see `config/scenes.config`), and lost when
  every player's ship is destroyed. The room also sends `0` if the game cannot
  run (for example a server-side loading error). The room is closed afterwards.
- **`PLAYER_LEFT`** — A player left during the game. Reason `0`: the player left
  on purpose (`LEAVE_ROOM` or `DISCONNECT`). Reason `1`: the player stopped
  answering (crash, network loss). Their ship is removed from the game.

### Reserved ids

`0x6C` was meant for cancelling a game. It is not sent by the current client and
is ignored by the server. Implementations **MUST** ignore unknown ids, so that
new messages can be added later.

## 5. Snapshot format

A `SNAPSHOT` (`0x14`) holds the full state of the game and is sent every tick
(60 times per second). It is not a delta: a client **SHOULD** replace everything
it displays with the content of the latest snapshot.

The payload is a sequence of entity records. Player records come first and take
7 bytes; every other record takes 5 bytes:

| Size | Field     | Notes                                     |
|------|-----------|-------------------------------------------|
| 1    | type      | see the table below                       |
| 2    | x         | position of the sprite's top-left corner  |
| 2    | y         |                                           |
| 1    | health    | players only, 0 to 255                    |
| 1    | player id | players only, as in `PLAYER_LIST`         |

Coordinates are in a **1900 × 1000** virtual playfield whose origin is the
top-left corner. Negative positions are sent as 0. Players start with 100
health; the health power-up sets it to 150.

| Type   | Entity                                                  |
|--------|---------------------------------------------------------|
| `0x15` | player ship                                             |
| `0x16` | Bydos: moves up and down on the right side and shoots    |
| `0x17` | lateral enemy: crosses the screen from right to left    |
| `0x18` | player missile                                          |
| `0x19` | enemy missile                                           |
| `0x20` | boss 1                                                  |
| `0x21` | wave enemy: zigzags from right to left                  |
| `0x22` | boss 1 missile                                          |
| `0x23` | boss 2                                                  |
| `0x24` | boss 2 missile                                          |
| `0x25` | damage power-up (once picked up, it is reported next to the player that holds it) |
| `0x26` | speed power-up                                          |
| `0x27` | health power-up                                         |

If the world does not fit in one datagram, the server drops the entities that
overflow. Players are never dropped since they come first.

## 6. Event format

`EVENTS` (`0x40`) tells clients about one-shot effects, so they can play an
animation and a sound. A room may send several `EVENTS` messages per tick. The
payload is a sequence of 6-byte records:

| Size | Field   | Description                               |
|------|---------|-------------------------------------------|
| 1    | event   | what happened, see the table below        |
| 1    | subject | entity type the effect relates to         |
| 2    | x       | where it happened                         |
| 2    | y       |                                           |

| Event  | Subject | Meaning                                    |
|--------|---------|--------------------------------------------|
| `0x41` | `0x15`  | a player's ship was destroyed              |
| `0x41` | `0x17`  | an enemy was destroyed                     |
| `0x41` | `0x20`  | boss 1 was destroyed                       |
| `0x41` | `0x23`  | boss 2 was destroyed                       |
| `0x41` | `0x18`  | a player missile hit a boss                |
| `0x41` | `0x24`  | an enemy missile hit a player              |
| `0x42` | `0x15`  | a player fired                             |
| `0x43` | `0x15`  | an enemy fired                             |
| `0x22` | `0x20`  | boss 1 fired                               |
| `0x22` | `0x23`  | boss 2 fired                               |
| `0x44` | `0x16`  | a player picked up a power-up              |

The reference client draws the effect scaled to the sprite of the subject's
entity type. Clients **MUST** ignore events they do not know.

## 7. Liveness and disconnection

UDP has no connection, so a room cannot tell a quiet player from a crashed one
by itself:

- A client **MUST** send at least one message to its room every 5 seconds. The
  reference client sends `HEARTBEAT` every second, whatever its state.
- Any well-formed message from a player's address counts as a sign of life.
- A room checks its players every second and removes those that have been
  silent for 5 seconds. Before the game, it broadcasts the new `PLAYER_LIST`;
  during the game, it removes the ship and broadcasts `PLAYER_LEFT` with
  reason `1`. A crashed client is therefore noticed within 5 to 6 seconds.
- The main server has no timeout: clients only pass through it.

## 8. Limits and validation

| Limit                        | Value                                         |
|------------------------------|-----------------------------------------------|
| Datagram size                | 4096 bytes, header included                   |
| Username                     | 1 to 12 bytes, no control characters          |
| Players per room             | 4                                             |
| Rooms per server             | 32                                            |
| Room ids                     | 0 to 255; the lowest free id is reused        |
| Levels                       | 1 to 8 (as many as in `config/scenes.config`) |
| Room tick rate               | 60 per second                                 |
| Silence before removal       | 5 seconds                                     |

Finished rooms, and rooms that nobody joined within 30 seconds, are freed to
make room for new ones.

Receivers validate every message before using it: a payload that is too short,
a level out of range or a direction other than 1 to 4 is ignored. A malformed
or unexpected message never ends a session, and never gets an answer.

## 9. Security considerations

- **No authentication or encryption.** Usernames are not unique and anyone on
  the network can read the traffic. The game is meant for trusted local
  networks.
- **Address-based identity.** Players are identified by their source address and
  port, which an attacker on the same network could spoof to send inputs on a
  player's behalf.
- **Resource limits.** The limits of section 8 bound memory and socket usage:
  repeated `CONNECT` messages from one address do not create new entries, the
  number of rooms is capped, and abandoned rooms are freed.
- **No reliability layer.** Lost datagrams are not resent. Snapshots carry the
  full state, so a lost one is replaced by the next tick. A lost `GAME_STARTED`,
  `GAME_OVER` or `PLAYER_LEFT` is not recovered, and snapshots carry no
  sequence number, so a late snapshot can briefly show an older state.

## 10. Examples

`CONNECT` with the username `Adrien` (payload of 6 bytes):

```
38 A2  00 65  00 06  41 64 72 69 65 6E
magic  id     length "A  d  r  i  e  n"
```

`ROOM_PORT` announcing port 54321 (`0xD431`):

```
38 A2  00 08  00 02  D4 31
```

`MOVE` up:

```
38 A2  00 79  00 01  01
```

`SNAPSHOT` with player 1 at (50, 150) with 100 health, and a Bydos at
(1750, 300):

```
38 A2  00 14  00 0C
15  00 32  00 96  64  01     player: type, x=50, y=150, health=100, id=1
16  06 D6  01 2C             Bydos:  type, x=1750, y=300
```

`PLAYER_LEFT`: player 2 lost their connection:

```
38 A2  00 52  00 02  02 01
```
