#define isMaster
String address = "0013,EF,006BBF";//ABCD,EF,GHIJKL

HardwareSerial bluetoothSerial(1);
void setup()
{
    // put your setup code here, to run once:

    bluetoothSerial.begin(38400, SERIAL_8N1, 14, 13);
    Serial.begin(115200);
    delay(100);
    // stuff("AT+ORGL"); //factory reset

    // delay(2000);
    // stuff("AT+RNAME?");
#ifdef isMaster
    stuff("AT+ROLE=1");
    stuff("AT+NAME=\"Infraready1\"");

#else
    stuff("AT+ROLE=0");
    stuff("AT+NAME=\"Infraready2\"");
#endif
    stuff("AT+CMODE=1");
    stuff("AT+UART=38400,1,0");
    stuff("AT+PSWD=\"1234\"");
    stuff("AT+INIT");
#ifdef isMaster
    stuff("AT+PAIR=" + address + ",5");
    delay(5000);
    stuff("AT+BIND=" + address);
    stuff("AT+LINK=" + address);
#endif
    // delay(1000);
    // stuff("AT+RESET");
}

void loop()
{
    while (bluetoothSerial.available())
    {
        Serial.write(bluetoothSerial.read());
    }
}
void stuff(String thing)
{
    bluetoothSerial.print(String(thing + "\r\n"));
    delay(100);
    while (bluetoothSerial.available())
    {
        Serial.write(bluetoothSerial.read());
    }
}