# embedded-mbed
Firmware repository for mbed-based driver and device code.

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
