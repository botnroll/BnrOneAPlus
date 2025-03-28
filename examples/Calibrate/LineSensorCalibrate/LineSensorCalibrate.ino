/**
 * This example was created by José Cruz on October 2016
 * Updated on January 2025 by Nino Pereira
 *
 * This code example is in the public domain.
 * http://www.botnroll.com
 *
 * Calibration of Line sensor
 * The calibration routine is called in Setup()
 * Reads and stores the maximum and minimum value for every sensor on vectors
 * sensor_value_max[8] and sensor_value_min[8].
 * Low values for white and high values for black.
 * The user is presented with the options:
 * - simple calibration
 * - full calibration
 * The simple calibration only updates the maximum and minimum values for each
 * sensor. The full calibration also updates the threshold (THRESHOLD) and
 * correction factor. The transition value from white to black (THRESHOLD) is
 * defined by the user: THRESHOLD is the lowest value above which a colour is
 * considered black. By default is suggested the highest of the lower values.
 *   THRESHOLD should be as low as possible as long as it assures a safe
 *   transition from white to black.
 *
 * To calibrate place the robot over the line with the line at the centre of the
 * sensor. The robot rotates for a few seconds acquiring the 8 sensor max and
 * min values.
 *
 * The registered values are displayed on the LCD.
 * Use the push buttons to see more values.
 *
 * THRESHOLD
 * If you have chosen the full calibration then you can define the
 * THRESHOLD value. To do that you are asked to check the sensor values on a
 * white background at different places on the track..
 * Take note of the highest value and give it some margin.
 * Set the THRESHOLD by pressing PB1 and PB2 on the robot.
 * Once finish, proceed to calibrate the correction factor.
 * Place the robot centred on top of the black line and press a button to start.
 * The robot rotates left and right until it finds the right value.
 * After that the robot stops. You can repeat the calibration otherwise if you
 * chose to finish the calibration is automatically saved to the config file.
 *
 * CORRECTION FACTOR:
 * Place the robot on the line and move it manually to the left so
 * that the value is about 100.
 * Change the correction factor to increase and decrease the range
 * of detection. The larger the correction factor the smaller the range of
 * detection. Note: Range of detection is the distance between the two positions
 * of the sensor corresponding to -100 and 100 respectively.
 *
 * <>
 * Calibração do sensor de linha
 * A rotina de calibração é chamada em Setup()
 * Lê e armazena o valor máximo e mínimo de cada sensor nos vectores
 * sensor_value_max[8] e sensor_value_min[8].
 * Valores baixos para o branco e valores altos para o preto.
 * São apresentadas ao utilizador as opções:
 * - calibração simples
 * - calibragem completa
 * A calibração simples apenas actualiza os valores máximo e mínimo de cada
 * sensor. A calibração completa também actualiza o limiar (THRESHOLD) e o fator
 * de correção. O valor de transição do branco para o preto (THRESHOLD) é
 * definido pelo utilizador: THRESHOLD é o valor mais baixo acima do qual uma
 * cor é considerada preta. Por defeito, é sugerido o valor mais alto dos
 * valores mais baixos. O valor THRESHOLD deve ser o mais baixo possível, desde
 * que garanta uma transição segura do branco para o preto.
 *
 * Para calibrar, coloque o robot sobre a linha com a linha no centro do sensor.
 * O robot roda durante alguns segundos, adquirindo os 8 valores máximos e
 * mínimos do sensor.
 *
 * Os valores registados são apresentados no LCD.
 * Utilize os botões de pressão para ver mais valores.
 *
 * THRESHOLD:
 * Se tiver escolhido a calibração completa, pode definir o valor
 * valor THRESHOLD. Para tal, é-lhe pedido que verifique os valores do sensor
 * num num fundo branco em diferentes pontos da via. Anote o valor mais alto e
 * dê-lhe alguma margem. Defina o THRESHOLD premindo PB1 e PB2 no robot.
 *
 * FACTOR DE CORRECÇÃO:
 * Coloque o robô sobre a linha e desloque-o manualmente para a esquerda
 * de modo que o valor lido esteja perto de 100.
 * Ajuste o factor de correcção de modo a aumentar ou diminuir
 * a amplitude para deteção da linha. Quanto maior o factor de correcção
 * menor é a amplitude de leitura da linha.
 * Nota: Amplitude de leitura da linha corresponde à distância entre
 * as posicões do sensor entre -100 e +100.
 *
 * Pode repetir a calibração, caso contrário, se optar por terminar,
 * a calibração é automaticamente guardada no ficheiro de configuração.
 */

#include <BnrOneAPlus.h>  // Bot'n Roll ONE A library
#include <SPI.h>          // SPI communication library required by BnrOne.cpp

// constants definitions
#define SSPIN 2  // Slave Select (SS) pin for SPI communication
#define M1 1     // Motor1
#define M2 2     // Motor2
#define VMAX 1000
#define MINIMUM_BATTERY_V 10.5  // safety voltage for discharging the battery

using uint = unsigned int;

BnrOneAPlus one;            //  object to control the Bot'n Roll ONE A
Config config;              // variable used to load and save config values
LineDetector lineDetector;  // variable used to detect line

class Time {
 public:
  void start() {
    // Set Timer1 in CTC mode with prescaler of 256
    TCCR1A = 0;  // No need to set anything for Timer1 in CTC mode
    TCCR1B = (1 << WGM12) |
             (1 << CS12);  // WGM12 for CTC mode, CS12 for 256 prescaler
    // Set compare value for 1-second interval
    OCR1A = 62500;  // (16e6 / (256 * 1)) - 1 = 62500
    // Enable Timer1 interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Enable global interrupts
    sei();
  }

  void update() { time_ += 1; }

  uint now() { return time_; }

 private:
  volatile uint time_ = 0;
};

Time time;

ISR(TIMER1_COMPA_vect) { time.update(); }

/**
 * @brief Prints array of integers in the terminal
 *
 * @param text
 * @param array
 */
void printArray(const String& text, const int array[8]) {
  Serial.begin(115200);
  Serial.println(text);
  for (int i = 0; i < 8; ++i) {
    Serial.print(array[i]);
    Serial.print("    ");
  }
  Serial.println();
  Serial.end();
}

/**
 * @brief Prints a text and value on the terminal (serial console)
 *
 * @tparam T
 * @param text
 * @param value
 */
template <class T>
void printValue(const String& text, const T value) {
  Serial.begin(115200);
  Serial.print(text);
  Serial.println(value);
  Serial.end();
}

/**
 * @brief Prints text to the terminal (serial console)
 *
 * @param text
 */
void printMsg(const String& text) {
  Serial.begin(115200);
  Serial.println(text);
  Serial.end();
}

/**
 * @brief Waits for a button to be pressed
 */
int waitButtonPress() {
  int button_pressed = 0;
  while (button_pressed == 0) {
    button_pressed = one.readButton();
    delay(50);
  }

  return button_pressed;
}

/**
 * @brief Waits for the button to be released
 */
void waitButtonRelease() {
  while (one.readButton() != 0) {
    delay(50);
  }
}

/**
 * @brief Initial instructions so that the user places the robot safely
 * on a flat surface ready to rotate on spot
 */
void prepareCalibration() {
  printMsg("Place robot on the floor ready to rotate on the spot");
  printMsg("Press a button when ready");
  one.lcd1(" Press a button ");
  one.lcd2("  to calibrate  ");
  waitButtonPress();
  one.lcd1(" Release button ");
  one.lcd2("   to start     ");
  printMsg("Release the button to start calibration");
  delay(1000);
  waitButtonRelease();
  printMsg("Calibration Started!");
  one.lcd1("  Calibration   ");
  one.lcd2("   started!     ");
}

/**
 * @brief Saves configuration to default file
 */
void saveConfig() {
  config.Save();
  config.Print();
}

/**
 * @brief Finds the min and max value for each sensor
 *
 * @param sensor_value_min
 * @param sensor_value_max
 */
void CalibrateMinMax(int sensor_value_min[8], int sensor_value_max[8]) {
  printMsg("Computing min and max for sensor readings...");
  one.move(15, -15);
  const auto start_time = time.now();
  while (time.now() < (start_time + 6)) {
    const auto reading = one.readLineSensor();
    printArray("Readings: ", reading);
    for (int i = 0; i < 8; ++i) {
      if (reading[i] > sensor_value_max[i]) {
        sensor_value_max[i] = reading[i];
      }
      if (reading[i] < sensor_value_min[i]) {
        sensor_value_min[i] = reading[i];
      }
    }
    printArray("Max: ", sensor_value_max);
    printArray("Min: ", sensor_value_min);
    delay(50);
  }
  one.stop();
  printMsg("Done");
}

/**
 * @brief Updates lcd info and waits for user to press and release a button
 */
void updateLcdInfo(const String& text,
                   const int value_1,
                   const int value_2,
                   const int value_3,
                   const int value_4) {
  one.lcd1(text);
  one.lcd2(value_1, value_2, value_3, value_4);
  // printMsg("Press and release button to continue");
  waitButtonPress();
  waitButtonRelease();
}

/**
 * @brief Displays calibration data on the lcd
 */
void displayCalibration(const int sensor_value_min[8],
                        const int sensor_value_max[8]) {
  one.lcd1("                ");
  one.lcd2(" Press a button ");
  waitButtonPress();
  waitButtonRelease();

  updateLcdInfo("Max1  2   3   4 ",
                sensor_value_max[0],
                sensor_value_max[1],
                sensor_value_max[2],
                sensor_value_max[3]);

  updateLcdInfo("Max5  6   7   8 ",
                sensor_value_max[4],
                sensor_value_max[5],
                sensor_value_max[6],
                sensor_value_max[7]);

  updateLcdInfo("Min1  2   3   4 ",
                sensor_value_min[0],
                sensor_value_min[1],
                sensor_value_min[2],
                sensor_value_min[3]);

  updateLcdInfo("Min5  6   7   8 ",
                sensor_value_min[4],
                sensor_value_min[5],
                sensor_value_min[6],
                sensor_value_min[7]);
}

/**
 * @brief During this stage user should test the robot on white surface and make
 * note of the highest reading. That value will be necessary to adjust the
 * threshold if necessary in the next stage.
 */
void takeNoteOfThreshold() {
  one.lcd1(" Test THRESHOLD ");
  one.lcd2(" on white color ");
  printMsg("Test threshold on white color. Make note of the highest value.");
  printMsg("Press and release a button to continue");
  waitButtonPress();
  waitButtonRelease();
  printMsg("When ready, please press a button to go to next stage.");
  lineDetector.LoadConfig();
  while (one.readButton() == 0) {
    const int* reading = one.readLineSensor();
    printArray("reading: ", reading);
    const int* normalised = lineDetector.NormaliseReadings(reading);
    printArray("normalised: ", normalised);
    one.lcd1(normalised[0], normalised[1], normalised[2], normalised[3]);
    one.lcd2(normalised[4], normalised[5], normalised[6], normalised[7]);
    delay(100);
  }
}

/**
 * @brief This step is optional. The threshold is calculated automatically
 * but if you want to make your own adjustment this allows you to do it.
 */
void adjustThreshold() {
  printMsg("Use PB1 and PB2 to increase or decrease the threshold.");
  printMsg("Use PB3 when ready to save result.");
  int threshold = config.GetThreshold();
  one.lcd1("  PB1++  PB2-- ");
  one.lcd2("threshold:", threshold);
  waitButtonRelease();
  int button = 0;
  while (button != 3) {
    button = one.readButton();
    if (button == 1) {
      threshold += 10;
      one.lcd2("threshold:", threshold);
      delay(100);
    }
    if (button == 2) {
      threshold -= 10;
      one.lcd2("threshold:", threshold);
      delay(100);
    }
  }
  config.SetThreshold(threshold);
  printValue("threshold = ", threshold);
  waitButtonRelease();
}

/**
 * @brief Calibration of the correction factor.
 *  The correction factor is the percentage by which we extend the
 *  range of the readings followed by cropping them out.
 *  This operation decreases the sensitivity region of the sensor
 *  but it gets rid of undesirable drops in readings near the extremeties.
 *  User should find the right correction factor to use in order to:
 *   - get rid of dropping values near the extremities
 *   - not to narrow the sensitivity too much
 *  The user should test each value by placing the robot on top of a black line
 *  and manually drag the robot left and right to test both extremities.
 *  Tipical values for the correction factor are between 0 and 10.
 *  start from zero and gradually increase it until you find the ideal
 *  conditions.
 *  The values at extremities should always remain at maximum values (-100 and
 * 100) Once you get that you should stop increasing the correction factor.
 */
void calibrateCorrectionFactor() {
  printMsg("Place robot on a black line");
  one.lcd1(" Find line edge ");
  one.lcd2("Any key to start");
  waitButtonRelease();
  waitButtonPress();
  waitButtonRelease();
  config.SetCorrectionFactor(0);
  int factor = 0;
  one.lcd1("  PB1++  PB2-- ");
  one.lcd2("  PB3 Finish   ");

  waitButtonPress();
  waitButtonRelease();

  bool exit = false;
  while (!exit) {
    int button = one.readButton();

    switch (button) {
      case 1:
        if (factor < 20) {
          ++factor;
          config.SetCorrectionFactor(factor);
          lineDetector.SetConfig(config);
        }
        waitButtonRelease();
        break;
      case 2:
        if (factor > 0) {
          --factor;
          config.SetCorrectionFactor(factor);
          lineDetector.SetConfig(config);
        }
        waitButtonRelease();
        break;
      case 3:
        exit = true;
        waitButtonRelease();
        break;
    }
    const auto reading = one.readLineSensor();
    int lineValue = lineDetector.ComputeLine(reading);
    one.lcd1("   Line: ", lineValue);
    one.lcd2(" Factor: ", factor);
  }
}

/**
 * @brief Displays info in terminal and robot lcd
 */
void calibrationDone() {
  printMsg("Calibration Done!");
  one.lcd1("Calibration Done");
  one.lcd2("                ");
  delay(2000);
}

/**
 * @brief Display menu for user interaction
 */
void displayMenu() {
  printMsg("If you wish to repeat press PB1 or PB2 on the robot.");
  printMsg("If you wish to continue press PB3.");
  one.lcd1("Repeat: PB1,PB2");
  one.lcd2("Continue: PB3   ");
  waitButtonPress();
}

/**
 * @brief Performs the calibration in 1 (simple) or 3 (full) main steps:
 * 1. CalibrateMinMax to find the minimum and maximum values for each sensor
 * 2. adjustThreshold to manually set the value of the threshold to distinguish
 * black and white
 * 3. adjust_correction_factor to manually set the value of the correction
 * factor to eliminate problematic readings at extremities of the line sensor
 */
void calibrateLine(bool fullCalibration = false) {
  prepareCalibration();
  static int sensor_value_min[8] = {
      1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
  static int sensor_value_max[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  while (one.readButton() != 3) {
    CalibrateMinMax(sensor_value_min, sensor_value_max);
    displayCalibration(sensor_value_min, sensor_value_max);
    displayMenu();
  }

  config.SetSensorMin(sensor_value_min);
  config.SaveSensorMin();
  config.SetSensorMax(sensor_value_max);
  config.SaveSensorMax();

  if (fullCalibration) {
    // full calibration: threshold and correction factor
    waitButtonRelease();
    while (one.readButton() != 3) {
      takeNoteOfThreshold();
      adjustThreshold();
      displayMenu();
    }
    config.SaveThreshold();
    // calibration of correction factor
    waitButtonRelease();
    while (one.readButton() != 3) {
      calibrateCorrectionFactor();
      displayMenu();
    }
    config.SaveCorrectionFactor();
  }
  saveConfig();
  calibrationDone();
}

/**
 * @brief Reads and prints saved config values on the terminal
 */
void viewCalibration() {
  config.Load();
  config.Print();
}

/**
 * @brief Asks user to choose between simple or full calibration
 */
int getUserOption() {
  one.lcd1("B1:Simple");
  one.lcd2("B2:Full  B3:Skip");
  waitButtonRelease();
  const int option = waitButtonPress();
  if (option == 2) {
    one.lcd1("                ");
    one.lcd2("B2:Full         ");
    delay(1000);
  } else if (option == 1) {
    one.lcd2("                ");
    delay(1000);
  }

  return option;
}

void setup() {
  one.spiConnect(SSPIN);                  // starts the SPI communication module
  one.stop();                             // stop motors
  one.setMinBatteryV(MINIMUM_BATTERY_V);  // battery discharge protection
  time.start();
  config.Load();
  config.Print();
  delay(1000);
  const auto option = getUserOption();
  if (option != 3) {
    bool do_full_calibration = false;
    if (option == 2) {
      do_full_calibration = true;
    }
    calibrateLine(do_full_calibration);  // calibrate line sensor <> Calibração
                                         // do sensor de linha
  }
  viewCalibration();  // read line calibrate values from EEPROM <> Ler valores
                      // de calibração da linha da EEPROM
}

void loop() {
  const int line = one.readLine();  // Read line <> Ler a linha
  printValue(" Line: ", line);
  // printMsg values on the LCD <> Apresenta valores no LCD
  one.lcd1("     Line:");
  one.lcd2("      ", line);
  delay(50);
}
