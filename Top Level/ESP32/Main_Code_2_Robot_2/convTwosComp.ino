//Convert to int from two's complement

int convTwosComp(int b) { //Convert from 2's complement
  if (b & 0x80) {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}
