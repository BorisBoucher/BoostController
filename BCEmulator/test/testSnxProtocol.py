'''
Created on 15 sept. 2018

@author: Boris
'''
from threading import Thread
import unittest

import serial

from BCEmularor import SnxProtocolServer, MemoryServer


class Test(unittest.TestCase):
    
    class Server(Thread):
        
        def __init__(self, memoryServer):
            self.memServer = memoryServer
            Thread.__init__(self)
            
        def __enter__(self):
            self.comServer = SnxProtocolServer()
            self.comServer.open("COM8")
            self.start()
            
        def __exit__(self, type, value, traceback):
            self.join()
            self.comServer.close()
            self.comServer = None

        def run(self):
            self.comServer.processMessage(self.memServer)
            
    def setUp(self):
#        self.server = Test.Server()
        self.port = serial.Serial("COM7", 115200, 8, 'N', 1, 200)
        self.memServer = MemoryServer()

    def tearDown(self):
        #self.server.close()
        self.server = None
        self.port.close()
        self.port = None

    def connectPort(self):
        pass
            
    def addChecksum(self, b):
        crc = 0
        for c in b[1:]:
            crc += c
        b += bytearray([crc & 0xff])
        
    def escapeBytes(self, b):
        ret = bytearray()
        ret.append(b[0])
        for c in b[1:]:
            if c == 2:
                ret += bytearray([0x03, 0xf2])
            elif c == 3:
                ret += bytearray([0x03, 0xf3])
            else:
                ret.append(c)
                
        return ret
        
    def readResponse(self):
        ret = bytearray(self.port.in_waiting)
        self.port.readinto(ret)
        return ret

    def requestResponse(self, req, resp):
        self.connectPort()

        with Test.Server(self.memServer) as server:
            
            # send read block : addr 10, 5 bytes
            msg = bytearray(req)
            self.addChecksum(msg)
            msg = self.escapeBytes(msg)
            self.port.write(msg)
            
            expect = bytearray(resp)
            self.addChecksum(expect)
            expect = self.escapeBytes(expect)
            
            # run the server
#                pass
            #self.server.start()
            #self.server.join()
        
        # get the response
        ret = self.readResponse()
        self.assertEqual(ret, expect)

    def testReadBlock(self):
        # send read block : addr 0x000a, 5 bytes
        self.requestResponse(b'\x02\x04R\x00\x0a\x05', b'\x02\x08R\x00\x0a\x0a\x0b\x0c\x0d\x0e')

    def testReadBytes(self):
        # send read bytes : addr 0x000a, 0x0014, 0x001e
        self.requestResponse(b'\x02\x07r\x00\x0a\x00\x14\x00\x1e', b'\x02\x0ar\x00\x0a\x0a\x00\x14\x14\x00\x1e\x1e')

    def testWriteBlock(self):
        # send write block : addr 0x0120, 5 bytes : 1, 2, 3, 4, 5
        self.requestResponse(
            bytearray([2, 8, ord('W'), 0x01, 0x20, 0x01, 0x02, 0x03, 0x04, 0x05]),
            bytearray([2, 1, ord("A")]))
        # read back the written values : addr 0x0120, 5 bytes
        self.requestResponse(b'\x02\x04R\x01\x20\x05', b'\x02\x08R\x01\x20\x01\x02\x03\x04\x05')

    def testWriteBytes(self):
        # send write bytes : addr 0x0125, 0x0130, 0x0135, bytes : 10, 20, 30
        self.requestResponse(
            bytearray([2, 10, ord('w'), 0x01, 0x25, 10, 0x01, 0x30, 20, 0x01, 0x35, 30]),
            bytearray([2, 1, ord("A")]))
        # read back the written values : addr 0x0125, 0x130, 0x135
        self.requestResponse(
            bytearray([2, 7, ord('r'), 0x01, 0x25, 0x01, 0x30, 0x01, 0x35]),
            bytearray([2, 10, ord('r'), 0x01, 0x25, 10, 0x01, 0x30, 20, 0x01, 0x35, 30]))

if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testName']
    unittest.main()