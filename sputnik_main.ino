#include "MPU6050.h"
#include <Wire.h> // подключаем I2C библиотеку
#include <MS5611.h>
#define addr 0x1E // I2C 7-битный адрес датчика HMC5883
MPU6050 mpu;
MS5611 ms5611;
long P01,P02;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t temp;
void gps(){
  
}
void setup() {
  Wire.begin();
  Serial1.begin(9600);
  Serial.begin(9600);
  mpu.initialize();
  Serial.println("**********************Stratolaunch Software***************************");
  Serial.println("StraatoSat OBC v5.2");
  Serial.println("Software programming by Arslan A");
  Serial.println("Created by Ilyas G");
  Serial.println("Sunbattery by Ruslan G");
  Serial.println("Version 1.0 Beta");
  Serial1.println("**********************Stratolaunch Software***************************");
  Serial1.println("Version 1.0 Beta");
  while(!ms5611.begin(MS5611_ULTRA_HIGH_RES))
  {
    delay(500);
    
  }
  Serial1.println("ms5611: connect!"); Serial.println("ms5611: connect!");
  P01 = ms5611.readPressure();
  P02 = ms5611.readPressure(true);
  

  // Задаём режим работы датчика HMC5883:
    Wire.beginTransmission(addr);
    Wire.write(0x00); // выбираем регистр управления CRA (00)
    Wire.write(0x70); // записываем в него 0x70 [усреднение по 8 точкам, 15 Гц, нормальные измерения]
    Wire.write(0xA0); // записываем в регистр CRB (01) 0xA0 [чувствительность = 5]
    Wire.write(0x00); // записываем в регистр Mode (02) 0x00 [бесконечный режим измерения]
    Wire.endTransmission();
    Serial1.println("HMC5883: connect!"); Serial.println("HMC5883: connect!");
  // состояние соединения
  Serial1.println(mpu.testConnection() ? "MPU6050 OK" : "MPU6050 FAIL");
  pinMode(37, OUTPUT); pinMode(36, OUTPUT); pinMode(35, OUTPUT); pinMode(34, OUTPUT); pinMode(33, OUTPUT); pinMode(32, OUTPUT); pinMode(31, OUTPUT);
  delay(1000);}
void barometr(){  // Барометр
  Serial1.println("--MS5611---------------------------------");
  Serial.println("--MS5611---------------------------------");
  long P1 = ms5611.readPressure();
  float A1 = ms5611.getAltitude(P1,P01);
  // Read true temperature & Pressure (with compensation)
  long P2 = ms5611.readPressure(true);
  float A2 = ms5611.getAltitude(P1, P02);
  // Output
  Serial.print("No compensation: "); Serial.print(A1); Serial.print("m, with compensation: "); Serial.print(A2); Serial.println("m");
  Serial1.print("No compensation: "); Serial1.print(A1); Serial1.print("m, with compensation: "); Serial1.print(A2); Serial1.println("m"); }
void light(){
  digitalWrite(37, HIGH); delay(50);
  digitalWrite(36, HIGH); delay(50);
  digitalWrite(35, HIGH); delay(50);
  digitalWrite(34, HIGH); delay(50);
  digitalWrite(33, HIGH); delay(50);
  digitalWrite(32, HIGH); delay(50);
  digitalWrite(31, HIGH); delay(50);
  digitalWrite(37, LOW); delay(50);
  digitalWrite(36, LOW); delay(50);
  digitalWrite(35, LOW); delay(50);
  digitalWrite(34, LOW); delay(50);
  digitalWrite(33, LOW); delay(50);
  digitalWrite(32, LOW); delay(50);
  digitalWrite(31, LOW); delay(50);}
void xyz_data(){  //3-осевой цифровой компас
  Serial1.println("--HMC5883---------------------------------");
  Serial.println("--HMC5883---------------------------------");
  Wire.beginTransmission(addr);
  Wire.write(0x03); // переходим к регистру 0x03
  Wire.endTransmission();
  Wire.requestFrom(addr, 6); // запрашиваем 6 байтов
  while(Wire.available()){ 
    int h = Wire.read(); // старший байт значения по оси X
    int l = Wire.read(); // младший байт значения по оси X
    int x = word(h, l);  // объединяем в двухбайтовое число

    int y = Wire.read(); // старший байт значения по оси Y
    y = y << 8; // сдвигаем влево на 8 битов
    y = y | Wire.read(); // объединяем с младшим байтом по OR

    int z = Wire.read() << 8; // читаем байт и сдвигаем влево на 8 битов
    z |= Wire.read(); // сокращённый синтаксис операции OR
    Serial.print("X = "); Serial.print(x, DEC); Serial.print("; Y = "); Serial.print(y, DEC); Serial.print("; Z = "); Serial.print(z, DEC); Serial.println(";");
    Serial1.print("X = "); Serial1.print(x, DEC); Serial1.print("; Y = "); Serial1.print(y, DEC); Serial1.print("; Z = "); Serial1.print(z, DEC); Serial1.println(";");}}
void gxyz_data(){ //положениe в пространстве
  Serial1.println("--MPU6050---------------------------------");
  Serial.println("--MPU6050---------------------------------");
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("x: "); Serial.print((float)ax/ 32768 * 2);
  Serial.print("; y: "); Serial.print((float)ay/ 32768 * 2);
  Serial.print("; z: "); Serial.print((float)az/ 32768 * 2);
  Serial.print("; gx: "); Serial.print((float)gx/ 32768 * 250);
  Serial.print("; gy: "); Serial.print((float)gy/ 32768 * 250);
  Serial.print("; gz: "); Serial.print((float)gz/ 32768 * 250); Serial.println(";");

  Serial1.print("x: "); Serial1.print((float)ax/ 32768 * 2);
  Serial1.print("; y: "); Serial1.print((float)ay/ 32768 * 2);
  Serial1.print("; z: "); Serial1.print((float)az/ 32768 * 2);
  Serial1.print("; gx: "); Serial1.print((float)gx/ 32768 * 250);
  Serial1.print("; gy: "); Serial1.print((float)gy/ 32768 * 250);
  Serial1.print("; gz: "); Serial1.print((float)gz/ 32768 * 250); Serial1.println(";");}
void loop() {
  Serial1.println("Next--->");
  Serial.println("");
  barometr();
  light(); 
  gxyz_data(); 
  xyz_data();
  delay(10000);
}