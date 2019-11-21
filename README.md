# Build

## Prerequisite:

Install nats

```
git clone git@github.com:nats-io/nats.c.git .
mkdir build && cd build
cmake .. -DNATS_BUILD_STREAMING=OFF -DCMAKE_BUILD_TYPE=Release
sudo make install
```

Run nats server

```
docker pull nats:latest
docker run -d --name nats-main -p 4222:4222 -p 6222:6222 -p 8222:8222 nats
```
