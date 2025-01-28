/* 
  Capteur de température et d'humidité SHT-C3
  Capteur de pression BMP280
  Création d'une trame à transmettre en LoRa
  Transmition de la trame en LoRa
  
  Programme basique sur MKR WAN 1310
  IDE Visual Studio Code 1.95.3
  
  Constituants :
  -  Module Adafruit SHTC3

  Version 2 : 21/01/2025

  Benoit BOREK
  */

#include <Adafruit_BMP280.h>
#include <MKRWAN.h>
#include <Arduino.h>
#include <Wire.h> //Importation de la bibliothèque Wire pour pouvoir communiquer avec le capteur.
#define Addr 0x70 //Définition de la variable Addr avec la valeur héxadécimale 0x70.


Adafruit_BMP280 bmp;

#define BMP280_I2C_ADDRESS 0x76
//Partie d'initialisation du LoRa :
/*LoRaModem modem;




float puissance = pow(2, 16); //Définition d'une décimale ayant comme valeur 2 puissance 16.
                              // Cette variable est utilisé aux lignes 97 et 101.

float vitesse = 25.9;

float lattitude = 344223.0;
float longitude = 331234.6;
String date = "15012025";
String heure = "101420";


String appEui = "0000000000000000";
String appKey = "E1510F7D6408ACA459E58EF86975727A";



String lattitudeStr = String(lattitude, 1);
String longitudeStr = String(longitude, 1);*/



void setup() {
  Serial.begin(115200); //Initialisation de la communication PC <----> Arduino.
  while (!Serial);

/*if (!modem.begin(EU868)) {
    Serial.println("Erreur lors du lancement du module");
    while (1) {}
}




  Serial.print("La version du module est : ");
  Serial.println(modem.version());
  Serial.print("Le numéro EUI du module est : ");
  Serial.println(modem.deviceEUI());

  int connected = modem.joinOTAA(appEui, appKey);
  if(!connected){
    Serial.println("Quelque chose n'a pas fonctionner avec la connection.");
    while(1);
  }
  modem.minPollInterval(60);
  
  Wire.begin(); //Initialisation de la librairie Wire et de l'I2C.

  Wire.beginTransmission(Addr); //Initialisation de la communication Arduino <----> SHTC3 avec l'adresse 0x70.
  Wire.write(0x80); //Envoi sur 2 octets la commande de réinitialisation système du capteur.
  Wire.write(0x5D);
   Wire.endTransmission(); //Fin de la communication Arduino <---> SHTC3.

// Le code ci-dessous sert de debbug pour vérifier que l'Arduino arrive à envoyer des données au capteur.
// L'Arduino va envoyer une commande de reveille et regarde si la fonction endTransmission() renvoie un code d'erreur
  /*Serial.println("Allumage du capteur");

  Wire.beginTransmission(Addr);
  Wire.write(0x35);
  Wire.write(0x17);
  int error = Wire.endTransmission();

  if(error != 0){
    Serial.println("Je n'ait pas réussis à communiquer avec le capteur.");
  } else {
    Serial.println("Communication réussis");
  }
  delay(2000);
*/

  if (!bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial.println(F("Le BMP280 n'a pas été trouvé veuillez le brancher et redémarrer le programme. "
                      "Lancement du mode sans BMP, la pression sera 1000.1"));
    while (1) delay(10);
  }

    bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); 
  
}

void loop() {

  float pression = bmp.readPressure();
  Serial.println("Pression : ");
  Serial.println(pression);

 /* Wire.beginTransmission(Addr); //Envoi de la commande de reveil avec le code 0x3517 sur 2 octets.
  Wire.write(0x35);
  Wire.write(0x17);
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(Addr); //Envoi de la commande de convertion en "Normal mode" avec la température lu en premier.
  Wire.write(0x7C); //Envoi de 0x7CA2 sur 2 octets.
  Wire.write(0xA2);
  Wire.endTransmission();
  delay(1000); //Attente d'une seconde pour laisser le temps au capteur de convertir la température et l'humidité.

  
  Wire.requestFrom(Addr, 6); //Envoi d'une requête de données sur 6 octets.
  // Deux octets sont pour la température, deux octets pour l'humidité et deux octets pour les checksum.
  Serial.print("Octets en attente de lecture: "); //Affichage dans le moniteur série du nombre d'octets disponible à intégrer dans des variables.
  Serial.println(Wire.available());
  int T_MSB = Wire.read(); //Lecture du premier octet de température.
  int T_LSB = Wire.read(); //Lecture du deuxième octet de température.
  int checksum1 = Wire.read(); // Attribution du checksum à une variable checksum1.
  int H_MSB = Wire.read(); //Lecture du premier octet d'humidité.
  int H_LSB = Wire.read(); //Lecture du deuxième octet d'humidité.
  int checksum2 = Wire.read(); // Attribution du checksum à une variable checksum2.

// Code qui peut servir de debug en cas de problème, il affiche les données brut de l'humidité et de la température en binaire.
  /*Serial.println("Affichage des données de températures brute : "); //Affichage des données binaires de la température.
  Serial.print("T_MSB : ");
  Serial.println(T_MSB, BIN); 
  Serial.print("T_LSB : ");
  Serial.println(T_LSB, BIN); 

  Serial.println("Affichage des données d'humidité brute : "); //Affichage des données binaires de l'humidité.
  Serial.print("H_MSB : ");
  Serial.println(H_MSB, BIN); 
  Serial.print("H_LSB : ");
  Serial.println(H_LSB, BIN); *//*
  
  //Conversion de la température en valeur décimale.
  float Temp = (T_MSB*255) + T_LSB; 
  Temp = -45 + 175 * (Temp/puissance); 

  //Conversion de l'humidité en valeur décimale.
  float Hum = (H_MSB*255) + H_LSB;
  Hum = 100 * (Hum/puissance);
 // Hum = 100 + Hum;

  Serial.println(); //Affichage de la température et de l'humidité dans le moniteur série.
  Serial.print("Température convertie : ");
  Serial.println(Temp);
  Serial.println();
  Serial.print("Humidité convertie : ");
  Serial.println(Hum);
  Serial.println();


  Wire.beginTransmission(Addr); //Envoi sur 2 octets de la commande de mise en veille du capteur.
  Wire.write(0xB0); //Commande de mise en veille 0xB098.
  Wire.write(0x98);
  Wire.endTransmission();
 


  String pressionStr = String(pression, 1);
  String vitesseStr = String(vitesse, 1);
  String TempStr = String(Temp, 2);
  String HumStr = String(Hum, 2);

  String Trame = date + heure + lattitudeStr + longitudeStr + TempStr + HumStr + pressionStr + vitesseStr;
  Trame.replace(".", "");
  Serial.println("Trame : ");
  Serial.println(Trame);

// Visualiser le dump hexadécimale de la trame.
 /* Serial.println();
  Serial.print("- Sending: " + Trame + " - ");
  for (unsigned int i = 0; i < Trame.length(); i++) {
    Serial.print(Trame[i] >> 4, HEX);
    Serial.print(Trame[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();*//*

  int error;
  modem.beginPacket();
  modem.print(Trame);
  error = modem.endPacket(true);
  if(error < 0){
    Serial.println("La trame n'a pas été transmise");
  } else {
    Serial.println("Le message à était transmis");
  }




  delay(120000); //Attente de 2 sec avant de recommencer une nouvelle convertion.*/
  delay(2000);
}

