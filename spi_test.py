#/usr/bin/python

import wiringpi as wp

SPI_CHANNEL = 0
SPI_SPEED = 8000000 #asking for 8MHz, will get 7. something

def main():
    setupresp = wp.wiringPiSetupGpio()
    print 'SetupGpio returned ' + str(setupresp)
    setupresp = wp.wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED)
    print 'SPISetup returned ' + str(setupresp)
    
    #writeData = str('FF')
    #writeData = { bytes(255), bytes(255) }
    writeData = bytes('FF')
    #writeData = bytearray.fromhex('FF FF')
    #test_all_on = "FF"
    #hex_data = test_all_on.decode("hex")
    #print hex_data
    #writeData = hex_data
    print 'writing...'
    recvData = wp.wiringPiSPIDataRW(SPI_CHANNEL, writeData)
    print 'got back ' + str(recvData)
    #recvData = wp.wiringPiSPIDataRW(SPI_CHANNEL, writeData)
    #print 'got back ' + str(recvData)


if __name__ == "__main__":
    main()
