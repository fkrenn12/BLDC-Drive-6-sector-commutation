# Serial Command Interpreter

## Message format

Commands are terminated by CR (`\r`) or LF (`\n`). The complete message format is:

```text
<target>:<sender>:<command>[:<value>]
```

For this firmware, the configured target address is `#3` (`MYADDRESS` in `Drive/mcc/main.h`).
The sender address is returned in the response.

Example:

```text
#3:#1:*RPM?:1500\r
```

Only messages whose target address matches the device address are processed.
Command names and addresses are case-sensitive.

## Valid commands

| Command | Value | Example | Implemented action |
|---|---:|---|---|
| `*TBC!` | none | `#3:#1:*TBC!` | Test bus connection; returns ACK |
| `*SDD!` | none | `#3:#1:*SDD!` | Returns ACK; no shutdown action is currently implemented |
| `*GAW!` | none | `#3:#1:*GAW!` | Returns ACK; no additional action is currently implemented |
| `*RTW?` | none | `#3:#1:*RTW?` | Returns ACK; warning reset is not currently implemented |
| `*GDM?` | none | `#3:#1:*GDM?` | Returns ACK; drive mode is not returned |
| `*GDD?` | integer | `#3:#1:*GDD?:0` | Sets direction: `0` = clockwise, any other integer = anticlockwise; returns ACK |
| `*GAM?` | none | `#3:#1:*GAM?` | Returns ACK; automatic mode is not returned |
| `*SCC?` | none | `#3:#1:*SCC?` | Returns ACK; charging current is not stored or returned |
| `*SDC?` | mA | `#3:#1:*SDC?:1200` | Sets the current limit in mA; value is clamped to the configured maximum; returns ACK |
| `*RPM?` | rpm | `#3:#1:*RPM?:1500` | Sets the speed target in rpm; value is clamped to the current maximum speed; returns ACK |

## Response format

For a request from `#1` to device `#3`, a successful command produces:

```text
#1:#3:*ACK!
```

An unknown command produces:

```text
#1:#3:*DCK!
```

Messages addressed to another device produce no response.

## Important implementation notes

- `*GDD?`, `*SDC?`, and `*RPM?` require a value. Without the value, the parsed integer defaults to `0`.
- The `?` and `!` suffixes are part of the command name and must be sent exactly.
- The current interpreter sends acknowledgements only. It does not send values for the `G...` query commands.
- The parser expects both separators, including the sender address. Use `#3:#1:*TBC!`, not only `#3:*TBC!`.
