'''
Created on 2 sept. 2018

@author: Boris
'''

import serial
import random
import time
import math

class CmdError(Exception):
    
    def __init__(self, code):
        self.code = code
    
class MemoryServer:
    
    def __init__(self):
        self.pollingTable = []
        for _ in range(0xff):
            self.pollingTable.append(0)
        
        self.dataByte = {}
        self.dataWord = {}
            
        self.speed = 0
        self.engSpeed = 0
        self.map = 0.0
        self.maf = 0
        self.estLoad = 0.0
        self.wgDuty = 0
        self.gear = 0
        self.boostTarget = 0.0
    
    def readByte(self, addr):
        if addr not in self.dataByte:
            return 127 + math.sin(time.time() * 1.0 * (1+addr)) * 127 #(0.5 - random.random()) * 0.1
        else:
            return self.dataByte[addr]
    
    def readWord(self, addr):
        if addr not in self.dataWord:
            return 1000 + math.sin(time.time() * 0.001 * (1+addr)) * 1000 #(0.5 - random.random()) * 0.1
        else:
            return self.dataWord[addr]

    def writeByte(self, addr, value):
        self.dataByte[addr] = value

    def writeWord(self, addr, value):
        self.dataWord[addr] = value

class FastComProtocolServer:
    
    def close(self):
        if self.port != None:
            self.port.close()
        
    def open(self, comPortName):
        self.port = serial.Serial(comPortName, 57600, 8, 'N', 1, 200)

    def readMessage(self):
        
        # Search start of message
        self.port.timeout = None

        msg = bytearray()
        while len(msg) == 0 or ((msg[0] & 0xf0) != 0xa0 and (msg[0] & 0xf0) != 0x50):
            b = self.port.read()
            if len(b) > 0:
                msg.clear()
                msg.append(b[0])
            
        #print("Received command {}".format(msg[0]))

        if (msg[0] & 0xf0) == 0x50:
            # byte or word read req
            size = 2
        elif (msg[0] & 0xf8) == 0xa0:
            # byte write req
            size = 3
        elif (msg[0] & 0xf8) == 0xa8:
            # word write req
            size = 4
        else:
            print("Framing error")
            return

        while len(msg) < size:
            b = self.port.read(size - len(msg))
            msg += b

        return msg
    
    def getAddr(self, msg):
        return (msg[0] & 0x0f) * 0x100 + msg[1]
    
    def clamp(self, value, low, hi):
        return max(min(value, hi), low)
    
    def processMessage(self, callback):
        msg = self.readMessage()
        addr = self.getAddr(msg)
        resp = bytearray()
        
        if (msg[0] & 0xf8) == 0x50:
            # single byte read
            resp.append(int(self.clamp(callback.readByte(addr), 0, 255)))
#            resp.append(random.randint(0, 128))
        elif (msg[0] & 0xf8) == 0x58:
            # word read
            value = self.clamp(callback.readWord(addr), 0 , 65535)
            resp.append((int(value) >> 8) & 0xff)
            resp.append(int(value) & 0xff)
        elif (msg[0] & 0xf8) == 0xa0:
            # byte write, just echo value
            callback.writeByte(addr, msg[2])
            resp.append(msg[2]);
        elif (msg[0] & 0xf8) == 0xa8:
            # word write, just echo value
            callback.writeWord(addr, msg[2] * 0x100 + msg[3])
            resp.append(msg[2]);
            resp.append(msg[3]);
        else:
            print("Invalid command")
        
        #print("Sending response of {} bytes for address {}".format(len(resp), addr))
        self.port.write(resp)

if __name__ == '__main__':
    comServer = FastComProtocolServer()
    comServer.open("COM8")
    memServer = MemoryServer()
    
    while True:
        comServer.processMessage(memServer)
    