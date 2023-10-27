# embedded-mbed
Firmware repository for mbed-based driver and device code.
<!-- TODO Table of contents -->


## Creating a Test
<!-- TODO Fill this in or wait until implementing a new test interface for others to implement -->

## Usage
This project can be compiled and optionally flashed using the following `make` targets:

Compile the project:
```
make compile [OPTIONS]
```

Compile and flash the project:
```
make flash MOUNTPOINT=</path/to/flash/mountpoint/> [OPTIONS]
```
With the following available `OPTIONS` (see the corresponding linked Arm Mbed OS documentation for more information):
- [`PROFILE`](https://os.mbed.com/docs/mbed-os/v6.16/program-setup/build-profiles-and-rules.html): `develop` (default), `debug`, or `release`
- [`TOOLCHAIN`](https://os.mbed.com/docs/mbed-os/v6.16/build-tools/index.html): `GCC_ARM` (default) or `GCC`
- <!-- TODO -->

## Configuration File Structures

### Setup Configuration

```yaml
# --- Structure ---
setup:
  target: <target>
  devices:
    <device>:
      <attribute>: <value>
      ...
    ...
  drivers:
    <driver>:
      <attribute>: <value>
      ...
    ...

# NOTE: If <value> is an array, format it as a Python list (i.e. [...])
```

### Test Configuration

```yaml
# --- Structure ---
<target>:
  <driver>:
    <test_name>: <mock|hardware|disabled>
    ...
  ...

# --- Test Options ---
# mock: Raspberry Pi emulates the device through software
# hardware: Hardware setup connected to the UUT. The Raspberry Pi does not mock the device
# disabled: Test will not be run
```
