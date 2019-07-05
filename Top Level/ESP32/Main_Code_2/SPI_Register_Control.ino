//SPI Register Functions 

//SPI Read Register Function
byte SPIReadReg(byte reg_addr) {
  digitalWrite(PIN_NCS, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(reg_addr & 0x7f); //Convert to valid register
  // read data
  byte data = vspi->transfer(0x00);
  digitalWrite(PIN_NCS, HIGH);//end communication

  return data;
}

//SPI Write to Register Function
void SPIWriteReg(byte reg_addr, byte data) {
  digitalWrite(PIN_NCS, LOW);
  //send address of the register, with MSBit = 1 to say it's writing
  vspi->transfer(reg_addr | 0x80 );
  //send data
  vspi->transfer(data);
  digitalWrite(PIN_NCS, HIGH);//end communication
}
