#define THRESHOLD_ONE
#define THRESHOLD_TWO
#define SENSOR_IN_ONE 2

bool WATER_TURBID = false;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int sensorValue1 = analogRead(A0);
    int sensorValue2 = analogRead(A1);

    float voltage1 = sensorValue1 * (5.0 / 1024.0);
    float voltage2 = sensorValue2 * (5.0 / 1024.0);

    Serial.println("Sensor Output1 (V):");
    Serial.println(voltage1);
    // Serial.println(sensorValue1);

    Serial.println("Sensor Output2 (V):");
    Serial.println(voltage 2);
    // Serial.println(sensorValue2);

    if (sensorValue1 > THRESHOLD_ONE)
    {
        WATER_TURBID = true;
        valveoperation();
    }

    void valveoperation() {
        digitalWrite(SENSOR_IN_ONE, HIGH);
    }

    Serial.println();
    delay(1000);
}