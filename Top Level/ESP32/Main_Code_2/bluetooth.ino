void bluetoothWrite(BluetoothData bluetoothData, BluetoothData& remoteBluetoothData)
{
  BluetoothData temp = remoteBluetoothData;
  bluetoothSerial.write("(");
  bluetoothSerial.write(bluetoothData.role);
  bluetoothSerial.write(bluetoothData.started);
  bluetoothSerial.write(bluetoothData.haveBall);
  bluetoothSerial.write(bluetoothData.role + bluetoothData.started + bluetoothData.haveBall);
  bluetoothSerial.write(")\r\n");

  bluetoothSerial.readStringUntil('(');
  temp.role = bluetoothSerial.read();
  temp.started = bluetoothSerial.read();
  temp.haveBall = bluetoothSerial.read();
  if(temp.role + temp.started + temp.haveBall == bluetoothSerial.read()){
    remoteBluetoothData = temp;
  }
}