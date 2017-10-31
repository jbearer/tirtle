# tIRLtle

## Dependencies (and APT package names and versions)

* Bluez (`libbluetooth-dev`)
* OpenCV (`opencv-3.3.1` and `opencv_contrib-3.3.1`)
* Arduino SDK (`arduino`)
* Submodules (just run `git submodule init` to install)
    - [avr-arduino-cmake](https://github.com/queezythegreat/arduino-cmake)
    - [boost spirit](https://github.com/boostorg/spirit)

## Installation

First install the [dependencies](#dependencies-and-apt-package-names-and-versions). Then:

* Build the client
```bash
cd client
mkdir build
cd build
cmake -DCMAKE_EXTRA_MODULES_PATH=<path/to/cmake/contrib/modules> ..
make
```

* Build the server
```bash
cd server
mkdir build
cd build
cmake ..
make
```

* To flash an executable onto the Arduino
```bash
cd server/build
make <name>-flash
```

e.g. `make server-flash`

* To open a serial port to the Arduino
```bash
sudo screen /dev/ttyACM0 -b 9600
```

## Architecture

The tirtle system consists of two programs, a client and a server.

### The tirtle server

The server runs on an Arduino embedded in the chassis of the tirtle robot. It listens for commands
via a Bluetooth module and responds by controlling the two stepper motors which drive the wheels of
the robot.

The server currently recognizes two commands:

* `void load_image(path_t *paths, length_t num_paths);`

  Register the given set of paths as the image to draw. This command should be sent once at the
  beginning of a tirtle session, and should not be sent again until the server is restarted.

* `void set_position(point_t loc, angle_t angle);`

  Used to inform the tirtle of its corrent position and orientation. The tirtle will compare this
  information with its desired target location and orientation (based on the paths loaded by
  `load_image`) and adjust its course as necessary.

### The tirtle client

The client program runs on a PC or a Raspberry PI which is attached to a camera. The camera watches
the tirtle as it moves around the canvas, and uses computer vision algorithms to continually
estimate the tirtle's position and orientation. Each time a new position and orientation is
calculated, the client sends it to the server via the `set_position` command.

The client also provides an interface which can be used to program the tirtle in a manner similar to
[turtle graphics](https://docs.python.org/2/library/turtle.html). This interface is exposed via the
`tirtle` class. The `draw` method on that class can be used to send a programmed path to the server
by calling `load_image`.

### Common interface

The client communicates with the server using an RPC protocol built on Bluetooth. The protocol is
defined in several shared header files located in the `common` folder. These files define the basic
data structures which will need to be serialized and sent over Bluetooth (in terms of fixed-width
types, to account for differences in implementation-defined data type widths between the Arduino and
the client hardware). The files also specify ID numbers for each valid RPC method.

The RPC protocl is simple: first, the ID number of the desired method is sent over the Bluetooth
connection. That is followed by the arguments to the method. The server waits for procedure IDs and
then attempts to parse the appropriate arguments based on the ID it receives. All data types
involved in the protocol have continguous memory layouts. Arrays are always serialized by first
writing the length of the array, and then each element of the array in order.
