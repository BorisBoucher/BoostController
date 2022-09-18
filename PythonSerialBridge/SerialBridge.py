

import argparse
from time import monotonic_ns
#from py import std
import serial
import time
import random
import curses
from curses.textpad import rectangle

convert_table = \
{
    0:    ("MAP",  1.0/50.0,      "bar"),
    1:    ("TPS",  1.0,           "%"),
    2:    ("WG",   1.0,           "%"),
    3:    ("GEAR", 1.0,           ""),
    4:    ("LOAD", 1.0,           "%"),
    6:    ("TGT",  1.0/50.0,      "bar"),
    7:    ("OUT",  1.0/50.0,      "bar"),
    92:   ("OILP", 1.0/20.0,      "bar"),
    93:   ("FUELP",1.0/40.0,      "bar"),
    1035: ("TACK", 1.0,           "RPM"),
    1036: ("SPD",  1.0,           "km/h"),
    1037: ("TPS'", 1.0/100.0,     "%/s"),
    1038: ("MAF",  1.0,           "Hz")
}

def updateDisplay(stdscr, data: dict):
    stdscr.clear()
    stdscr.box()

#    data[21] = [12, 34]
#    data[1023] = [100]
#    data[1023].append(int(random.random()*256))
#    data[int(random.random()*16)] = [int(random.random()*256)]

    now = monotonic_ns()

    y = 2
    for key, (buf, date) in sorted(data.items()):
        buf_str = ''.join('{:02X} '.format(x) for x in buf)
        val_str = ''
        value = None
        if key in convert_table:
            value = 0
            if len(buf) == 2:
                value = int.from_bytes([buf[0], buf[1]], 'little', signed=True)
            else:
                value = buf[0]
            # for x in reversed(buf):
            #     value = value * 256 + x
            value = value * convert_table[key][1]
            val_str = f'{convert_table[key][0]:4}{(value):10.2f} {convert_table[key][2]}'
        else:
            val_str = '           ---'
        stdscr.addstr(y, 4, f'{key:5} \u2551  {buf_str:6} \u2551')
        
        if now - date > 1000000000:
            stdscr.attron(curses.A_STANDOUT)
        stdscr.addstr(y, 4+5+3+6+3, f'{val_str}')
        stdscr.attroff(curses.A_STANDOUT)
        y = y + 1

        if y > curses.LINES-2:
            # no more space available
            break;

    rectangle(stdscr, 1, 3, y, 45)
#    stdscr.addstr(1, 5, "CLT     75.8°" + chr(0x2592))
#    stdscr.addstr(2, 5, "OIL     95.8°")

    stdscr.refresh()
    k = stdscr.getch()

    if k != -1:
        # process keystroke
        exit()
        pass


def main(stdscr):
    parser = argparse.ArgumentParser()

    parser.add_argument('-b', help='Baud rate', type=int, dest='speed', default=115200)
    parser.add_argument('com', help='Select COM port to use')

    args = parser.parse_args()

    port = None

    conn_win = curses.newwin(5, 40, 5, 5)
    conn_win.addstr(f'Opening serial port [{args.com}]')
    conn_win.refresh()


    while port == None:
        try:
            port = serial.Serial(args.com, args.speed)
        except serial.SerialException:
            print('.', end='')
            time.sleep(0.5)

    port.timeout = 0.2

    conn_win.clear()
    conn_win.refresh()
#    print(f'Opened port {args.com} at {args.speed} baud.')


    curses.curs_set(False)
    stdscr.clear()
    stdscr.nodelay(True)

    # stdscr.addstr(1, 5, "Hello")

    # stdscr.refresh()
    # stdscr.getkey()


    in_buf = bytearray()

    DATA_RQ = 0
    DATA_TX = 1


    def read_buffer(port: serial.Serial, size: int, in_buf: bytearray) -> bool:
        while len(in_buf) < size:
            recv = port.read(size - len(in_buf))
            if len(recv) == 0:
                # timeout
              #  exit()
                return False
            in_buf.extend(recv)

        return True


    def verify_checksum(buffer: bytearray) -> bool:
        sum = 0

        for b in buffer:
            sum += b

        return (sum & 0xff) == 0

    data = {}

    last_screen_update = monotonic_ns()

    while True:

        now = monotonic_ns()
        if now - last_screen_update > 100000000:
            updateDisplay(stdscr, data)
            last_screen_update = now

        # read min header (4 bytes)
        if not read_buffer(port, 4, in_buf):
            # timeout, clear and retry
            in_buf = bytearray()
            continue

        if len(in_buf) >= 4 and in_buf[0] != 0x02:
            # invalid header, expect SOT as first byte
            in_buf.pop(0)
            continue
        
        # decode header
        cmd = (in_buf[1] >> 3) & 0b11
        size = ((in_buf[1] >> 5) & 0b111) + 1
        id = ((in_buf[1] & 0b111) * 256) + in_buf[2]

        if cmd == DATA_TX:
            # read data transmit
            if not read_buffer(port, 4 + size, in_buf):
                # timeout, clear and retry
                in_buf = bytearray()
                continue

            if not verify_checksum(in_buf):
                # bad checksum, clear and retry
                in_buf = bytearray()
                continue

            # Store data in reception dictionary
            if id in convert_table:
                data[id] = (in_buf[3:-1], now)

            in_buf = bytearray()

        elif cmd == DATA_RQ:
            if not verify_checksum(in_buf):
                # bad checksum, clear and retry
                in_buf = bytearray()
                continue

            # TODO
        else:
            # invalid reception
            in_buf = bytearray()

curses.wrapper(main)