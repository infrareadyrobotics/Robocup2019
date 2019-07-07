void kick()
{
    if (millis() - kickerTimer > 4000)
    {
        kickerTimer = millis();
        digitalWrite(4, 1);
        delay(30);
        digitalWrite(4, 0);
        Serial.print(" kick ");
    }
}