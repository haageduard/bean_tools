# BEAN Tools

**BEAN Tools** is a Qt-based utility for researching, monitoring and testing the Toyota **BEAN (Body Electronics Area Network)** bus.

The application is intended primarily for reverse engineering and experimentation with Toyota/Lexus vehicle electronics. It provides tools for capturing BEAN traffic, manually constructing and transmitting frames, replaying captured traffic, maintaining small frame databases, and building custom dashboards that decode bus values into human-readable metrics and actions.

The project was originally developed while researching the Toyota Prius NHW20.

> **Warning**
>
> This is a research/debugging tool. Transmitting arbitrary frames to a vehicle network may cause unexpected ECU behaviour. Use it at your own risk.

## Features

### Dashboard

The Dashboard converts raw BEAN traffic into readable values such as:

* odometer
* engine RPM
* coolant temperature
* selector position
* light states
* door states
* any other value that can be extracted from a BEAN frame

Dashboard definitions are stored as simple JSON files.

A dashboard may contain:

* **metrics** — values decoded from received BEAN frames;
* **actions** — buttons that transmit or modify BEAN frames.

The repository contains an example:

```text
data/prius_20.json
```

### Toggler / frame editor

The **Toggler** tab allows BEAN frames to be built and modified manually.

You can configure:

* frame priority;
* destination ID;
* message ID;
* payload length from 1 to 11 bytes;
* individual payload bits;
* complete payload bytes.

Individual bytes can be quickly set or cleared, making the tool useful when determining which bits of an unknown message control a particular vehicle function.

Frames can be transmitted directly to the connected BEAN interface.

### Frame database

Frequently used frames can be stored in a local frame database.

Frame databases use the `.framedb` extension.

The editor supports:

* creating a new database;
* adding frames;
* deleting frames;
* editing frames through the Toggler;
* loading a database;
* saving a database.

An example database is included:

```text
data/prius.framedb
```

### Logger

The logger records traffic received from the BEAN interface and displays:

```text
TIME | PRI | ML | DST | MSG | DATA | CRC | ACK
```

Frames are also grouped by destination/message ID, which makes it easier to isolate periodically transmitted messages while reverse engineering the bus.

The log view can be filtered by:

* destination + message ID;
* destination ID;
* message ID.

Captured traffic can be saved as a `.dat` dump and opened again later.

Several example captures can be found in the `data/` directory.

### Player

Captured traffic can be copied from the Logger to the **Player**.

The Player retransmits frames while preserving the delays between the original captured messages.

It can be used for experiments such as:

1. capture traffic while an action is performed in the vehicle;
2. disconnect or disable the original source;
3. replay the captured sequence;
4. observe which ECU or display reacts.

Frames may be replayed as a complete sequence or transmitted one at a time.

Transmission ACK/error state is tracked by the player.

### Serial BEAN interface

BEAN Tools communicates with an external BEAN interface over a serial port.

Current serial settings are:

```text
115200 baud
8 data bits
no parity
1 stop bit
no flow control
```

The application expects the adapter to convert physical BEAN traffic into the simple text protocol understood by `Device`.

## Building

The project uses Qt and qmake.

Required Qt modules:

```text
Core
Gui
Widgets
SerialPort
QML
```

QML is required because Dashboard expressions are evaluated using `QJSEngine`.

Example build:

```bash
git clone https://github.com/haageduard/bean_tools.git
cd bean_tools

qmake bean-combination-meter.pro
make -j$(nproc)
```

Run:

```bash
./bean-combination-meter
```

---

# Dashboard configuration

A dashboard is a JSON document with the following top-level structure:

```json
{
  "name": "My vehicle",
  "version": 1,
  "metric": [],
  "action": []
}
```

`name`
: Human-readable dashboard name.

`version`
: Dashboard format version. Current examples use version `1`.

`metric`
: List of values decoded from received BEAN frames.

`action`
: List of buttons that can transmit BEAN frames.

---

## Metrics

A metric watches a particular combination of:

```text
DST ID + MSG ID
```

When a matching frame arrives, its payload bytes are substituted into a JavaScript expression.

For example:

```json
{
  "name": "Engine temperature",
  "enabled": true,
  "dst_id": "0xFE",
  "msg_id": "0x2C",
  "expr": "$0 / 2",
  "output": "$value C"
}
```

If the incoming payload is:

```text
B4 ...
```

then:

```text
$0 = 0xB4 = 180
```

and the expression becomes:

```javascript
180 / 2
```

so the Dashboard displays:

```text
90 C
```

### Metric fields

| Field         | Description                                       |
| ------------- | ------------------------------------------------- |
| `name`        | Name displayed in the dashboard                   |
| `enabled`     | Enables/disables this metric                      |
| `dst_id`      | BEAN destination ID                               |
| `msg_id`      | BEAN message ID                                   |
| `expr`        | JavaScript expression used to calculate the value |
| `match_value` | Optional mapping from calculated values to text   |
| `output`      | Optional output formatting                        |

`dst_id` and `msg_id` are normally written as hexadecimal strings:

```json
"dst_id": "0xFE",
"msg_id": "0x26"
```

---

## Payload variables

Payload bytes are available as:

```text
$0
$1
$2
...
$10
```

`$0` is the first data byte, `$1` the second, and so on.

For example, for:

```text
12 34 56 78
```

the variables are:

```text
$0 = 0x12
$1 = 0x34
$2 = 0x56
$3 = 0x78
```

---

## JavaScript expressions

The `expr` field is evaluated using Qt's `QJSEngine`, so normal JavaScript arithmetic and bitwise operators may be used.

### Single byte

```json
"expr": "$0"
```

### Bit test

```json
"expr": "$0 & 0x10"
```

### 16-bit big-endian value

```json
"expr": "($0 << 8) + $1"
```

### 24-bit big-endian value

```json
"expr": "(($0 & 0xFF) << 16) + ($1 << 8) + $2"
```

### Scaling

```json
"expr": "$0 / 2"
```

### RPM example

```json
{
  "name": "Engine RPM",
  "enabled": true,
  "dst_id": "0xFE",
  "msg_id": "0x26",
  "expr": "(($2 << 8) + $3) / 5.12",
  "output": "$value RPM"
}
```

---

## `match_value`

`match_value` converts numeric results into human-readable values.

For example, suppose byte 0 contains the transmission selector state:

```json
{
  "name": "Selector",
  "enabled": true,
  "dst_id": "0xFE",
  "msg_id": "0x40",
  "expr": "$0",
  "match_value": {
    "0x80": "P",
    "0x40": "R",
    "0x20": "N",
    "0x10": "D"
  },
  "output": "$value"
}
```

Instead of displaying:

```text
128
64
32
16
```

the Dashboard displays:

```text
P
R
N
D
```

It is also useful for individual flags:

```json
{
  "name": "Parking lights",
  "enabled": true,
  "dst_id": "0xFE",
  "msg_id": "0x7F",
  "expr": "$0 & 0x10",
  "match_value": {
    "0x10": "ON",
    "0x00": "OFF"
  },
  "output": "$value"
}
```

---

## Output formatting

After evaluating `expr` and optionally applying `match_value`, the result can be formatted using `output`.

The special variable is:

```text
$value
```

Example:

```json
"expr": "$0 / 2",
"output": "$value °C"
```

Another example:

```json
"expr": "(($0 << 8) + $1)",
"output": "$value rpm"
```

If `output` is omitted or empty, the calculated value is displayed directly.

---

# Dashboard actions

Actions create buttons in the right side of the Dashboard.

A simple action looks like this:

```json
{
  "name": "Example command",
  "enabled": true,
  "message": [
    {
      "prio": 3,
      "dst_id": "0xFE",
      "msg_id": "0x7F",
      "data": [
        16,
        0,
        0,
        0
      ]
    }
  ]
}
```

Pressing the button transmits the configured BEAN message.

### Action fields

| Field     | Description                                                                  |
| --------- | ---------------------------------------------------------------------------- |
| `name`    | Button text                                                                  |
| `enabled` | Whether the action is shown                                                  |
| `icon`    | Reserved/icon path; currently parsed but not applied to the generated button |
| `message` | One or more BEAN messages transmitted by the action                          |

An action can contain multiple messages:

```json
{
  "name": "Example sequence",
  "enabled": true,
  "message": [
    {
      "prio": 3,
      "dst_id": "0xFE",
      "msg_id": "0x20",
      "data": [1, 2, 3]
    },
    {
      "prio": 3,
      "dst_id": "0xFE",
      "msg_id": "0x21",
      "data": [4, 5, 6]
    }
  ]
}
```

---

# Action data operations

Dashboard actions have several special payload operations designed specifically for reverse-engineering vehicle messages.

Instead of replacing an entire frame, an action can take the most recently received frame with the same `dst_id` and `msg_id` and modify only selected bytes/bits.

## Literal value

A number replaces the corresponding byte completely:

```json
"data": [
  16,
  0,
  255
]
```

Hexadecimal strings may also be used:

```json
"data": [
  "0x10",
  "0x00",
  "0xFF"
]
```

## Keep the existing byte: `-1`

```json
"data": [
  -1,
  -1,
  "0x80",
  -1
]
```

`-1` means:

> copy this byte from the most recently received frame having the same `dst_id` and `msg_id`.

This is useful when only one byte of a large message needs to be changed.

## Set bits

```json
"set 0x10"
```

The selected bits are set while other bits are preserved:

```text
new_byte = old_byte | 0x10
```

Example:

```json
"data": [
  "set 0x10",
  -1,
  -1,
  -1
]
```

## Reset bits

```json
"reset 0x10"
```

The selected bits are cleared:

```text
new_byte = old_byte & ~0x10
```

Example:

```json
"data": [
  "reset 0x10",
  -1,
  -1,
  -1
]
```

## Toggle bits

```json
"toggle 0x80"
```

The selected bit is inverted.

If it was set, it is cleared. If it was clear, it is set.

Example:

```json
{
  "name": "Toggle door indication",
  "enabled": true,
  "message": [
    {
      "prio": 3,
      "dst_id": "0xFE",
      "msg_id": "0x7F",
      "data": [
        -1,
        -1,
        "toggle 0x80",
        -1
      ]
    }
  ]
}
```

---

## Important: actions using existing frame data

The following constructs depend on previously received traffic:

```text
-1
set ...
reset ...
toggle ...
```

BEAN Tools uses the latest captured frame having the same:

```text
dst_id + msg_id
```

as the template.

Therefore, before using such an action, **start the Logger and make sure that at least one frame with the target `dst_id/msg_id` has been received**.

This is especially important on a real vehicle because modifying one flag while unintentionally replacing unrelated bytes can produce unexpected behaviour.

For reverse engineering, a good workflow is:

```text
Capture original frame
        ↓
Identify changing byte/bit
        ↓
Create Dashboard action
        ↓
Use -1 for untouched bytes
        ↓
Use set/reset/toggle for the target flag
```

---

# Complete dashboard example

```json
{
  "name": "My Toyota",
  "version": 1,

  "metric": [
    {
      "name": "Coolant temperature",
      "enabled": true,
      "dst_id": "0xFE",
      "msg_id": "0x2C",
      "expr": "$0 / 2",
      "output": "$value °C"
    },

    {
      "name": "Headlights",
      "enabled": true,
      "dst_id": "0xFE",
      "msg_id": "0x7F",
      "expr": "$0 & 0x10",
      "match_value": {
        "0x00": "OFF",
        "0x10": "ON"
      },
      "output": "$value"
    },

    {
      "name": "Selector",
      "enabled": true,
      "dst_id": "0xFE",
      "msg_id": "0x40",
      "expr": "$0",
      "match_value": {
        "0x80": "P",
        "0x40": "R",
        "0x20": "N",
        "0x10": "D"
      },
      "output": "$value"
    }
  ],

  "action": [
    {
      "name": "Parking lights ON",
      "enabled": true,
      "message": [
        {
          "prio": 3,
          "dst_id": "0xFE",
          "msg_id": "0x7F",
          "data": [
            "set 0x10",
            -1,
            -1,
            -1
          ]
        }
      ]
    },

    {
      "name": "Parking lights OFF",
      "enabled": true,
      "message": [
        {
          "prio": 3,
          "dst_id": "0xFE",
          "msg_id": "0x7F",
          "data": [
            "reset 0x10",
            -1,
            -1,
            -1
          ]
        }
      ]
    }
  ]
}
```

Save it, for example, as:

```text
my_car.json
```

Then open the **Dashboard** tab and press **Open**.

As matching BEAN messages arrive, metric values will be updated automatically.

---

# Typical reverse-engineering workflow

A practical way to investigate an unknown BEAN message is:

1. Connect the BEAN interface.
2. Start the Logger.
3. Perform a known action in the vehicle.
4. Look for frames whose payload changes.
5. Use grouped Logger filtering to isolate a particular `DST/MSG`.
6. Save the capture if necessary.
7. Copy the capture to the Player and replay it.
8. Put interesting frames into the Toggler/frame database.
9. Toggle individual bits and observe vehicle behaviour.
10. Once a signal is understood, add it as a Dashboard metric or action.

This turns discovered raw messages into a reusable vehicle-specific dashboard.

---

# Data files

The repository currently contains several example data files under `data/`, including captures taken in different vehicle states, a frame database, and a Prius NHW20 dashboard definition.

These files are useful both as examples and as starting points for further BEAN reverse engineering.

---

# Current limitations

BEAN Tools is primarily a personal reverse-engineering utility rather than a polished end-user diagnostic application.

Some parts of the UI and file formats are experimental and may change.

In particular:

* dashboard JSON does not currently have a formal schema;
* action icons are parsed but are not currently displayed;
* dashboard expressions execute as JavaScript through `QJSEngine`;
* `.dat` and `.framedb` are internal binary formats and should not be considered portable/stable interchange formats;
* use of `-1`, `set`, `reset`, and `toggle` requires a valid previously received source frame.

Contributions and experiments are welcome.

## License

This project is free software distributed under the GNU General Public License (GPL).

See the LICENSE file for the full license text.

