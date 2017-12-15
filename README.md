This repo contains solutions to the [Cryptopals](https://cryptopals.com/) crypto challenges in C++.

## Requirements

* g++
* make

## Build

To build, navigate to the repo root directory, and run `make`.

## Run

You can run a challenge solution by passing the set number and challenge number as an argument to the `cryptopals` binary. Note that each solution may have its own additional arguments.

E.g. to run the solution to [set 1 challenge 1](https://cryptopals.com/sets/1/challenges/1):

```bash
./cryptopals s01c01 deadbeef
```

Make targets have been created for each challenge so that you can automatically run a challenge with the test arguments provided in the challenge:

```bash
make s01c01
```
