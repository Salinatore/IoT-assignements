#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
#define BUTTON4_PIN 5

#define LED1_PIN 10
#define LED2_PIN 11
#define LED3_PIN 12
#define LED4_PIN 13
#define LEDS_RED_PIN 9

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

bool gameneedtostart = false;
bool gameactivation = false;
int seq[4];              
int curretanswer = 0;   

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LEDS_RED_PIN, OUTPUT);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);

  randomSeed(analogRead(A0)); 
  Serial.begin(9600);
  Serial.print("Press Btn1 start ");// inizializza casualità
}

void generateSequence(int seq[4]) {
  int pool[4] = {1, 2, 3, 4};
  for (int i = 3; i > 0; --i) {
    int j = random(i + 1);
    int tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
  }
  for (int i = 0; i < 4; i++) {
    seq[i] = pool[i];
    Serial.print(String(seq[i]));
  }
}

void showSequenceLCD(int seq[4]) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sequence:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    lcd.print(seq[i]);
  }
}

void ligthupled(int seq) {
  switch (seq) {
    case 1: digitalWrite(LED1_PIN, HIGH); break;
    case 2: digitalWrite(LED2_PIN, HIGH); break;
    case 3: digitalWrite(LED3_PIN, HIGH); break;
    case 4: digitalWrite(LED4_PIN, HIGH); break;
  }
}

void ligthdownled(int seq) {
  switch (seq) {
    case 1: digitalWrite(LED1_PIN, LOW); break;
    case 2: digitalWrite(LED2_PIN, LOW); break;
    case 3: digitalWrite(LED3_PIN, LOW); break;
    case 4: digitalWrite(LED4_PIN, LOW); break;
  }
}

int numberbutton() {
  if (digitalRead(BUTTON1_PIN) == LOW) return 1;
  if (digitalRead(BUTTON2_PIN) == LOW) return 2;
  if (digitalRead(BUTTON3_PIN) == LOW) return 3;
  if (digitalRead(BUTTON4_PIN) == LOW) return 4;
  return 0;
}

void loop() {
  //attesa start gioco
  if (!gameneedtostart) {
    lcd.setCursor(0, 0);
    Serial.println("Press Btn1 start ");
    if (digitalRead(BUTTON1_PIN) == LOW) {
      gameneedtostart = true;
      gameactivation = false;
      delay(300);
    }
    return;
  }
  //il gioco è iniziato set up iniziale
  if (gameneedtostart && !gameactivation) {
    gameactivation = true;
    curretanswer = 0;
    //genera la sequenza rnadomica
    generateSequence(seq);
    //fa visualizzare la sequenza nel lcd
    showSequenceLCD(seq);
    delay(1000);
    //fa visualizzare la sequenza nel led
    for (int i = 0; i < 4; i++) {
      ligthupled(seq[i]);
      delay(400);
      ligthdownled(seq[i]);
      delay(200);
    }

    //lcd.clear();
  }
  //leggere il pulsante premuto
  int pressed = numberbutton();
  
  if (pressed > 0) {
    //accendi led del bottone premuto
    ligthupled(pressed);
    delay(200);
    ligthdownled(pressed);
	//verifica se il bottone premuto corrisponde a quello giusto 
    //nel caso vai al elemento successivo della sequenza
    if (pressed == seq[curretanswer]) {
      curretanswer++;
      //se ho passato tutta la sequenza vittoria
      if (curretanswer >= 4) {
        lcd.clear();
        Serial.print("WIN!");
        gameneedtostart = false;
        gameactivation = false;
        delay(1000);
      }
    } else if(pressed != seq[curretanswer]){
      //errore nel bottone premuto
      lcd.clear();
      Serial.print("LOSE!");
      digitalWrite(LEDS_RED_PIN, HIGH);
      delay(500);
      digitalWrite(LEDS_RED_PIN, LOW);
      gameneedtostart = false;
      gameactivation = false;
      delay(1000);
    }

   //ricordasi il bounce dei bottoni
  }
}