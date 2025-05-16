# ft_ping

## About

This project is a custom implementation of the standard network utility `ping`. The goal is to replicate the core functionality of the original `ping` command, specifically sending ICMP Echo Request packets to a target IP address and measuring the round-trip time.

The implementation aims to provide a response time that is consistently less than 20ms different from the standard `ping` command when testing the same target.

It supports the following command-line arguments:

* `<ip_address>`: The target IP address to ping.
* `-v`: Enable verbose output.
* `-v -?`: Display help/usage information.
* `-h`: Display help/usage information.

## Specs

`ft_ping` is implemented in [C] using raw sockets to craft and send ICMP packets. It handles sending echo requests and receiving echo replies, calculating round-trip times, and displaying statistics similar to the original `ping` utility. The project focuses on achieving a high degree of compatibility and performance close to the system's native `ping`.


## How to use

### Compile

To compile the project, navigate to the project root directory in your terminal and run the following command:

```sh
make
```

### Execute

To run `ft_ping`, provide the target IP address as an argument:

```sh
./ft_ping <ip_address>
```
