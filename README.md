# Traffic model simulation

This project is a simulator of internet traffic patterns.
Two models are available for simulation - Uniform and Poisson.

## Configure

For configuration, you need to change the first two lines in [`files/config.txt`](files/config.txt).
The first line is a non-negative floating-point number equal
to the simulation time in seconds (not more than 3600).
The second line includes several parameters separated by a space, the first parameter is the model name - "Poisson" or "Uniform", the second parameter is the average delay between packets in nanoseconds, the third parameter is the average traffic packet size in bytes

**The delimiter is a space.**

| Parameter                | Position        | Value                  |
| ------------------------ | --------------- | ---------------------- |
| Time [s]                 | First line      | 0.0 < X < 3600.0       |
| Model name               | Second line 1st | `Poisson` or `Uniform` |
| Inter packet delay [ns]  | Second line 2nd | A non-negative integer |
| Mean packet size [bytes] | Second line 3rd | A non-negative integer |

Example of a config for 1G Ethernet traffic simulation.

```
10.0
Poisson 5600 700
```

## Build

```shell
mkdir -p cpp/build
cmake -S cpp -B cpp/build
cmake --build cpp/build -j

python3 -m venv python/.venv
source python/.venv/bin/activate
pip install -r python/requirements.txt
```

## Execute

```shell
bash run.sh
```
