// Inclure le code de la bibliothèque:
#include <LiquidCrystal_I2C.h>
// Initialiser la bibliothèque avec les dimensions de l'interface
LiquidCrystal_I2C lcd(0x27, 20, 4);

int ledPinVert = 10;
int ledPinRouge = 11;

#include <Keypad.h>
const byte ROWS = 4;  //quatre rangées
const byte COLS = 4;  //quatre colonnes
//définir les symboles sur les boutons des claviers
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };  //connecter aux broches de rangée du clavier
byte colPins[COLS] = { 5, 4, 3, 2 };  //se connecter aux broches de colonne du clavier

//initialiser une instance de la classe NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//Code d'accès à saisir pour déverrouiller 
char code[8] = { '1', '2', '3', '4', '5', '6','7','A'};


char codesaisie[8] = { '*', '*', '*', '*', '*', '*','*', '*'};
byte EtapeSaisiecode;
boolean codesaisieok;
char buffer[20];

void setup() {
  Serial.begin(9600);
  // Initialisation de l'interface LCD
  lcd.init();
  // Active le rétro-éclairage du LCD
  lcd.backlight();

  pinMode(ledPinVert, OUTPUT);
  digitalWrite(ledPinVert, LOW);
  pinMode(ledPinRouge, OUTPUT);
  digitalWrite(ledPinRouge, LOW);
}

void loop() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    if (customKey == '#') {
      if (EtapeSaisiecode > 0) {
        EtapeSaisiecode--;
      }
    } else {
      codesaisie[EtapeSaisiecode] = customKey;
      EtapeSaisiecode++;
    }
    Serial.print("Touche saisie : ");
    Serial.println(customKey);
    Serial.print("Code saisie : ");
    Serial.println(codesaisie);
  }

  lcd.setCursor(1, 0);
  lcd.print("- Code Carburant -");
  switch (EtapeSaisiecode) {
    case 0:
      lcd.setCursor(2, 2);
      sprintf(buffer, " * * * * * * * * ");
      lcd.print(buffer);
      break;
    case 1:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c * * * * * * * ", codesaisie[0]);
      lcd.print(buffer);
      break;
    case 2:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c * * * * * *", codesaisie[0], codesaisie[1]);
      lcd.print(buffer);
      break;
    case 3:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c * * * * *", codesaisie[0], codesaisie[1], codesaisie[2]);
      lcd.print(buffer);
      break;
    case 4:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c %c * * * *", codesaisie[0], codesaisie[1], codesaisie[2], codesaisie[3]);
      lcd.print(buffer);
      break;
    case 5:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c %c %c * * *", codesaisie[0], codesaisie[1], codesaisie[2], codesaisie[3], codesaisie[4]);
      lcd.print(buffer);
      break;
    case 6:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c %c %c %c * *", codesaisie[0], codesaisie[1], codesaisie[2], codesaisie[3], codesaisie[4], codesaisie[5]);
      lcd.print(buffer);
      break;
    case 7:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c %c %c %c %c *", codesaisie[0], codesaisie[1], codesaisie[2], codesaisie[3], codesaisie[4], codesaisie[5], codesaisie[6]);
      lcd.print(buffer);
      break;
    case 8:
      lcd.setCursor(2, 2);
      sprintf(buffer, " %c %c %c %c %c %c %c *", codesaisie[0], codesaisie[1], codesaisie[2], codesaisie[3], codesaisie[4], codesaisie[5], codesaisie[6], codesaisie[7]);
      lcd.print(buffer);
      EtapeSaisiecode++;
      break;
    case 9:
      //Vérification du code saisie par rapport au code accès
      for (int i = 0; i <= 7; i++) {
        if (codesaisie[i] != code[i]) {
          codesaisieok = 0;
          break;
        } else {
          codesaisieok = 1;
        }
      }

      if (codesaisieok == 1) {
        lcd.setCursor(3, 2);
        sprintf(buffer, "  livreson   ");
        lcd.print(buffer);
        digitalWrite(ledPinVert, HIGH);
        delay(5000);
      } else {
        lcd.setCursor(3, 2);
        sprintf(buffer, "   Refuser   ");
        lcd.print(buffer);
        digitalWrite(ledPinRouge, HIGH);
        delay(5000);
      }
      digitalWrite(ledPinVert, LOW);
      digitalWrite(ledPinRouge, LOW);
      codesaisieok = 0;
      EtapeSaisiecode = 0;
      break;
  }
}
