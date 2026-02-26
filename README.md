# KV-Store

A high-performance in-memory key-value store with HTTP REST API, built in C++17.

## Features

- **In-memory storage**: Fast key-value operations
- **HTTP REST API**: Standard interface, easy to test
- **TTL support**: Automatic key expiration
- **LRU eviction**: Automatic cleanup when capacity reached
- **Thread-safe**: Handles concurrent requests
- **Configurable**: Via config file

## Quick Start

### Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run

```bash
./kv_store.exe ../config.ini
```

### Test

```bash
# Set a key
curl -X PUT http://localhost:8080/keys/name -d "Alice"

# Get a key
curl http://localhost:8080/keys/name

# Set with TTL (expires in 30 seconds)
curl -X PUT "http://localhost:8080/keys/session?ttl=30" -d "abc123"

# Delete a key
curl -X DELETE http://localhost:8080/keys/name
```

## API Documentation

### PUT /keys/{key}

Store a value for the given key.

**Query Parameters:**
- `ttl` (optional): Time-to-live in seconds

**Request Body:** Value to store

**Responses:**
- `201 Created`: New key created
- `200 OK`: Existing key updated

### GET /keys/{key}

Retrieve the value for the given key.

**Responses:**
- `200 OK`: Key found, value in body
- `404 Not Found`: Key doesn't exist or expired

### DELETE /keys/{key}

Delete the given key.

**Responses:**
- `200 OK`: Key deleted successfully
- `404 Not Found`: Key doesn't exist

## Configuration

Create a `config.ini` file:

```ini
[server]
port=8080
thread_count=4
max_capacity=10000

[logging]
level=info
```

## Architecture

```
┌─────────────┐     ┌──────────────┐     ┌─────────────────┐
│   Client    │────▶│   Server     │────▶│   Thread Pool   │
│   (curl)    │     │  (accept)    │     │   (workers)     │
└─────────────┘     └──────────────┘     └─────────────────┘
                                                 │
                          ┌──────────────────────┴──────────────────┐
                          ▼                                          ▼
                   ┌─────────────┐                           ┌─────────────┐
                   │   Store     │                           │   LRU       │
                   │  (HashMap)  │◄──────────────────────────│   Cache     │
                   └─────────────┘                           └─────────────┘
                          │
                          ▼
                   ┌─────────────┐
                   │   TTL       │
                   │   Manager   │
                   └─────────────┘
```

## Technologies

- C++17
- Winsock (Windows)
- CMake
- Standard Library (unordered_map, list, mutex, thread)

## License

MIT
