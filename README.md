# Real-Time-Currency-Arbitrage-Detection-Engine

a low-latency c++ engine that detects risk-free profit loops in foreign exchange markets using graph theory.

## How it works
the system models forex pairs as a directed weighted graph. to find multiplicative profit loops (where exchange rates multiply to greater than 1), it transforms the rates using the negative natural logarithm. this allows the **bellman-ford algorithm** to detect negative-weight cycles, which mathematically correspond to arbitrage opportunities.

## Tech stack
* language: c++
* concepts: graph theory, object-oriented programming, bellman-ford algorithm

## How to run
compile the code using standard g++:
g++ main.cpp -o arbitrage

run the executable:
./arbitrage
