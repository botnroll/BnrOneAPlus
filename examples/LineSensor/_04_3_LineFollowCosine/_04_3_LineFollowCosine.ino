/**
 * This example was created by José Cruz on February 2025
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * IMPORTANT!!!!
 * Before you use this example you MUST calibrate the line sensor. Use example
 * CalibrateLineSensor first!!!
 *
 * Line reading provides a linear value between -100 to 100
 * Line follow:
 * Motors speed varies according to a cosine function of the line value.
 * Function Gain can be adjusted.
 * You can adjust the speed limit of the wheel that is outside the curve.
 * Press push button 3 (PB3) to enter control configuration menu.
 *
 * <>
 *
 * IMPORTANTE!!!!+
 * Antes de usar este exemplo é necessário calibrar o sensor de linha. Usar
 * exemplo CalibrateLineSensor antes deste!!!
 *
 * Leitura da linha que devolve um valor linear entre -100 e +100 da posição da
 * linha. Seguimento de linha: Os motores variam com a linha de acordo com a
 * função coseno do valor da linha. O ganho da função pode ser ajustado. Ajuste
 * do limite de velocidade da roda que está no exterior da curva. Botão 3 (PB3)
 * entra no menu de configuração do controlo.
 *
 * https://www.desmos.com/calculator
 *
 */

 #include <BnrOneAPlus.h>  // Bot'n Roll ONE A library
 #include <EEPROM.h>   // EEPROM reading and writing
 #include <SPI.h>      // SPI communication library required by BnrOne.cpp
 BnrOneAPlus one;  // declaration of object variable to control the Bot'n Roll ONE A
 
 // constants definitions
 #define SSPIN 2                 // Slave Select (SS) pin for SPI communication
 #define ML 1                    // Left Motor
 #define MR 2                    // Right Motor
 #define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery
 
 int speed = 55; //Robot overall speed <> Velocidade do robô para seguir a linha
 int speedML = 0, speedMR = 0; //Wheel 
 
 //lineGain behaviour -> Lower Gain, higher output
 //Comportamento do lineGain -> Baixando o ganho aumenta a saida
 float lineGain = 2.0;   //Line gain <> Ganho da linha
 
 //wheelBoostFactor behaviour ->  Lower Gain, higher output
 //Comportamento do wheelBoostFactor -> Baixando o ganho aumenta a saida
 float wheelBoostFactor = 5.0; //Speed increase factor for outside wheel <>
                                //Fator para o incremento da velocidade da roda exterior
 int wheelBoost = 4;   //Outside wheel maximum speed on a curve
                       //Velocidade máxima da roda exterior numa curva
 int minSpeedLim = -1;   //Inside wheel minimum speed on a curve
                         //Velocidade minima da roda interior numa curva
 
                         
   /****** Line Following with Cosine Function ************
   ******* Seguimento de linha com função coseno **********
   ********************************************************/
  void lineFollowCosine()
  {
   //Read the line value from the line sensor <> Ler o valor da linha do sensor de linha
   int line = one.readLine();
 
   //Apply line gain to the line value <> Aplicar o ganho da linha ao valor da linha
   float angleDegree=(float)line*lineGain;
 
   //Angle cannnot exceed 180º <> O angulo não pode exceder 180º
   if(angleDegree>180) angleDegree=180;
   if(angleDegree<-180) angleDegree=-180;
 
   //Conversion factor from degree to radians <> Factor de conversão de graus para radianos
   const float convertDegreeToRadians=3.1459/180.0; //rad=degree*Pi/180
 
   //Convert Degree to Radians <> Converter graus em radianos
   float angRad=convertDegreeToRadians*angleDegree;
 
   //If line is on the left side of the sensor <> Se a linha está do lado esquerdo do sensor
   if(line<=0){
     //Inside wheel decreases speed according to cosine function
     //Roda interior diminui a velocidade de acordo com a função coseno
     speedML=(int)(cos(angRad)*speed);
 
     //Outside wheel maintains or increases speed according to wheelBoostFactor
     //Roda exterior mantém ou aumenta a velocidade de acordo com o factor wheelBoostFactor
     speedMR=speed+(int)(((float)(speed-speedML))/wheelBoostFactor);
 
     //Limit the outside wheel boost <> Limitar o aumento de velocidade da roda exterior
     if(speedMR>speed+wheelBoost) speedMR=speed+wheelBoost;
 
     //Limit inside wheel minimum speed <> Limitar velocidade mínima da roda interior
     if(speedML < minSpeedLim) speedML= minSpeedLim;
   }
 
   //If line is on the right side of the sensor <> Se a linha está do lado direito do sensor
   else if(line>0){
     speedMR=(int)(cos(angRad)*speed);
     speedML=speed+(int)(((float)(speed-speedMR))/wheelBoostFactor);
     if(speedML>speed+wheelBoost) speedML=speed+wheelBoost;
     if(speedMR < minSpeedLim) speedMR = minSpeedLim;
   }
  }

 void menu() {
   int temp_var = 0;
   float temp = 0.0;
   one.stop();
   one.lcd1("  Menu Config:");
   one.lcd2("PB1+ PB2-  PB3ok");
   // Wait PB3 to be released <> Espera que se largue o botão 3
   while (one.readButton() == 3); 
   while (one.readButton() == 0); 
   while (one.readButton() == 3); 
   
   //***** Overall speed <> Velocidade para seguir a linha ******
   temp_var = speed;
   while (one.readButton() != 3) {
     one.lcd2("   speed:", temp_var);
     delay(125);
 
     if (one.readButton() == 1) {
       ++temp_var;
     }
     if (one.readButton() == 2) {
       --temp_var;
     }
   }
   // Wait PB3 to be released <> Espera que se largue o botão 3
   while (one.readButton() == 3);
   speed = temp_var;
 
   //**** Outside wheel maximum speed <> Velocidade maxima da roda exterior
   temp_var = wheelBoost;
   while (one.readButton() != 3) {
     one.lcd2(" Wheel Boost:", temp_var);
     delay(125);
 
     if (one.readButton() == 1) {
       ++temp_var;
     }
     if (one.readButton() == 2) {
       --temp_var;
     }
   }
   // Wait PB3 to be released <> Espera que se largue o botão 3
   while (one.readButton() == 3);
   wheelBoost = temp_var;
 
   //**** Line gain <> Ganho da linha  ****
   temp =(int)(wheelBoostFactor * 1000.0);
   temp_var = (int)temp;
   while (one.readButton() != 3) {
     one.lcd2("BoostFactor:", temp_var);
     delay(125);
 
     if (one.readButton() == 1) {
       temp_var += 100;
     }
     if (one.readButton() == 2) {
       temp_var -= 100;
     }
   }
   while (one.readButton() == 3);  // Wait PB3 to be released <> Espera que se largue o botão 3
   wheelBoostFactor = (float)temp_var / 1000.0;
 
  //**** Inside wheel minimum speed on a curve <> Velocidade minima da roda interior numa curva
   temp_var = minSpeedLim;
   while (one.readButton() != 3) {
     one.lcd2(" minSpeedLim:", temp_var);
     delay(125);
 
     if (one.readButton() == 1) {
       ++temp_var;
     }
     if (one.readButton() == 2) {
       --temp_var;
     }
   }
   // Wait PB3 to be released <> Espera que se largue o botão 3
   while (one.readButton() == 3);
   minSpeedLim = temp_var;
 
   //**** Line gain <> Ganho da linha  ****
   temp =(int)(lineGain * 1000.0);
   temp_var = (int)temp;
   while (one.readButton() != 3) {
     one.lcd2(" Line Gain:", temp_var);
     delay(125);
 
     if (one.readButton() == 1) {
       temp_var += 50;
     }
     if (one.readButton() == 2) {
       temp_var -= 50;
     }
   }
   while (one.readButton() == 3);  // Wait PB3 to be released <> Espera que se largue o botão 3
   lineGain = (float)temp_var / 1000.0;
 
   //**** Configuration end <> Termina Configuração *****
   writeMenuEEPROM();  // Write control values to EEPROM <> Escrever valores de
                       // controlo na EEPROM
   one.lcd1("Line  Following!");
   one.lcd2("www.botnroll.com");
   delay(250);
 }
 
 // Write values on EEPROM <> Escrever valores na EEPROM
 void writeMenuEEPROM() {
   byte eeprom_address = 20;
   int temp_var = 0;
 
   temp_var = speed;
   EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
 
   temp_var = wheelBoost;
   EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
 
   temp_var = (int)(lineGain * 1000.0);
   EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
   EEPROM.write(eeprom_address, lowByte(temp_var));
   ++eeprom_address;
 
   temp_var = minSpeedLim;
   EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
   EEPROM.write(eeprom_address, lowByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
 
   temp_var = (int)(wheelBoostFactor * 1000.0);
   EEPROM.write(eeprom_address, highByte(temp_var));  // Guardar em EEPROM
   ++eeprom_address;
   EEPROM.write(eeprom_address, lowByte(temp_var));
   ++eeprom_address;
 }
 
 //Test if value is withn limits <> Testa se o valor está dentro dos limites
 template <typename T>
 boolean isWithinLimits(const T valor,const T min, const T max){
  if(valor > max) return false;
  if(valor < min) return false;
  return true;
}

 // Read values from EEPROM <> Ler valores da EEPROM
 void readMenuEEPROM() {
   byte eeprom_address = 20;
   int temp_var = 0;
 
   speed = (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
 
   wheelBoost = (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
 
   temp_var = 0;
   temp_var = (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   temp_var = temp_var << 8;
   temp_var += (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   lineGain = (float)temp_var / 1000.0;
 
   temp_var = 0;
   temp_var = (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   temp_var = temp_var << 8;
   temp_var += (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   minSpeedLim = temp_var;
 
   temp_var = 0;
   temp_var = (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   temp_var = temp_var << 8;
   temp_var += (int)EEPROM.read(eeprom_address);
   ++eeprom_address;
   wheelBoostFactor = (float)temp_var / 1000.0;
 
   if (!isWithinLimits<byte>(speed,0,100)) speed = 50;
   if (!isWithinLimits<byte>(wheelBoost,0,100)) wheelBoost = 4;
   if (!isWithinLimits<float>(lineGain,0.0,10.0)) lineGain = 2.0;
   if (!isWithinLimits<int>(minSpeedLim,-100,100)) minSpeedLim = -2;
   if (!isWithinLimits<float>(wheelBoostFactor,0.0,20.0)) wheelBoostFactor = 4.0;
  }

  void loop() {
    // Call follow line function <> Chama a função de seguir linha
    lineFollowCosine();
  
    //Move motors <> Mover motores
    one.move(speedML,speedMR); 
  
    // Configuration menu <> Menu de configuração
    if (one.readButton() == 3) menu();  // PB3 to enter menu <> PB3 para entrar no menu
  }
  
  void setup() {
   Serial.begin(115200);    // sets baud rate to 115200bps for printing values at
                           // serial monitor.
   one.spiConnect(SSPIN);  // starts the SPI communication module
   one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery safety voltage
   one.stop();                             // stop motors
   one.lcd1("Line Follow COS ");
   one.lcd2(" Press a button ");
   if(one.readButton() == 0) //Skip read EEPROM is necessary
     readMenuEEPROM();  // read control values from EEPROM <> Ler valores de
                      // controlo da EEPROM
   // Wait a button to be pressed <> Espera que pressione um botão
   while (one.readButton() == 0);
   // Wait for button release <> Espera que largue o botão
   while (one.readButton() != 0);
   delay(125); //Debounce delay
   one.lcd2("www.botnroll.com");
 
 }