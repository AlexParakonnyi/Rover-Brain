#include <ArduinoJson.h>
#include <battery.h>
#include <voltage_data.h>

StaticJsonDocument<300> voltageJson;
StaticJsonDocument<300> chargingJson;

// Массив для хранения 5 значений
const int numReadings = 50;
float readings[numReadings];
int timeDelay = 25;

void initChargerPins() {
  pinMode(IN19, INPUT);
  pinMode(IN20, INPUT);
}

float getAverageVoltage() {
    
    float totalVal = 0;

    for (int i = 0; i < numReadings; i++) {
        totalVal = totalVal + readings[i];
    }

    Serial.print("Total val: ");
    Serial.println(totalVal);
    Serial.print("Num: ");
    Serial.println(numReadings);

    float averageValue = totalVal / numReadings;
    return averageValue;
}

// Функция для поиска V_real по заданному V_corrected с учётом диапазона
float findVRealFromCorrected(float targetCorrected) {
  int lowerIndex = -1;
  int upperIndex = -1;

  // Ищем диапазон, в который попадает targetCorrected
  for (int i = 0; i < dataSize; i++) {
    float minCorrected = voltageData[i][1]; // Минимальное значение
    float maxCorrected = voltageData[i][2]; // Максимальное значение

    if (targetCorrected >= minCorrected && targetCorrected <= maxCorrected) {
      lowerIndex = i;
      upperIndex = i; // Если попадает в диапазон, используем эту пару
      break;
    }
    // Если targetCorrected больше максимума текущей пары, но меньше минимума следующей
    if (i < dataSize - 1 && targetCorrected > maxCorrected && targetCorrected < voltageData[i + 1][1]) {
      lowerIndex = i;
      upperIndex = i + 1;
      break;
    }
  }

  // Если не найдено точного диапазона, используем крайние точки
  if (lowerIndex == -1) {
    lowerIndex = 0;
    upperIndex = 0;
  }
  if (upperIndex == -1) {
    upperIndex = dataSize - 1;
  }

  float minCorrected = voltageData[lowerIndex][1];
  float maxCorrected = voltageData[upperIndex][2];
  float lowerReal = voltageData[lowerIndex][0];
  float upperReal = voltageData[upperIndex][0];

  // Если targetCorrected вне диапазона, возвращаем ближайшее V_real
  if (targetCorrected < minCorrected) {
    return lowerReal;
  }
  if (targetCorrected > maxCorrected) {
    return upperReal;
  }

  // Линейная интерполяция внутри диапазона
  if (lowerIndex == upperIndex) {
    // Интерполяция между minCorrected и maxCorrected для одной пары
    float interpolatedReal = lowerReal + (targetCorrected - minCorrected) * (upperReal - lowerReal) / (maxCorrected - minCorrected);
    return round(interpolatedReal * 10) / 10.0; // Округление до одного знака
  } else {
    // Интерполяция между двумя соседними парами
    float interpolatedReal = lowerReal + (targetCorrected - minCorrected) * (upperReal - lowerReal) / (voltageData[upperIndex][1] - minCorrected);
    return round(interpolatedReal * 10) / 10.0;
  }
}

 float *getArray() {
    // Собираем значения с интервалом 10 мс
  for (int i = 0; i < numReadings; i++) {
    int sensorValue = analogRead(IN19); // Читаем значение ADC

    readings[i] = sensorValue;

    if (i < numReadings - 1) {
      delay(timeDelay);
    }
  }

  return readings;
}

char *getVoltageJson() {

  float *arr = getArray();
  float averageBatteryVoltage = getAverageVoltage();

  // Serial.print("Filtered Average Battery Voltage: ");
  // Serial.print(averageBatteryVoltage, 2);
  // Serial.println(" V");

  // Заполняем JSON
  // voltageJson["is charging"] = digitalRead(IN20);
  voltageJson["value"] = averageBatteryVoltage;
  voltageJson["battery"] = findVRealFromCorrected(averageBatteryVoltage);

  static char jsonBuffer[512];  // Увеличиваем буфер, JSON большой 256 -> 512 -> 1024
  serializeJson(voltageJson, jsonBuffer);

  return jsonBuffer;
}

char *getChargingJson() {

  // Заполняем JSON
  chargingJson["charging"] = digitalRead(IN20);

  static char jsonBuffer[512];  // Увеличиваем буфер, JSON большой 256 -> 512 -> 1024
  serializeJson(chargingJson, jsonBuffer);

  return jsonBuffer;
}
