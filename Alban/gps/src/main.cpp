/**************************************************************************
 * Nom du Programme      : gps.cpp
 * Auteur                : Alban de Farcy de Pontfarcy
 * Date de création      : 08/01/2025
 * Dernière modification : 08/01/2025
 * Version               : 1.0
 * Description           :
 *    Ce programme a pour but de faire fontionner un gps (GNSS)
 *
 * Pré-requis            :
 *    Installer les bibliothèque : 
 *                              - iostream
 *                              - fstream
 *                              - string
 *                              - sstream
 *                              - thread
 *                              - chrono
 * Copyright             :
 *    © 2024 Alban de Farcy de Pontfarcy
 **************************************************************************/

#include <SoftwareSerial.h>

using namespace std;

// Définir les broches de communication pour SoftwareSerial
const int RXPin = 10; // Broche RX de l'Arduino connectée à TX du SIM868
const int TXPin = 11; // Broche TX de l'Arduino connectée à RX du SIM868

// Créer une instance de SoftwareSerial
SoftwareSerial sim868(RXPin, TXPin);

// Fonction pour envoyer une commande AT au module SIM868
void sendATCommand(const String &command, int delayMs)
{
    sim868.println(command);
    delay(delayMs);
    while (sim868.available())
    {
        Serial.write(sim868.read());
    }
}

// Initialisation
void setup()
{
    Serial.begin(9600); // Initialiser le port série pour le débogage
    sim868.begin(9600); // Initialiser le port série pour le SIM868

    Serial.println("Initialisation du module SIM868...");
    
    // Vérifier la communication avec le module
    sendATCommand("AT", 1000);
    
    // Activer le GPS
    sendATCommand("AT+CGPSPWR=1", 1000);
    
    // Configurer le GPS en mode de base
    sendATCommand("AT+CGPSRST=0", 1000);
}

void loop()
{
    // Demander les données GPS
    sendATCommand("AT+CGPSINF=0", 1000);

    // Lecture et affichage des données GPS
    if
    {
        sim868.available()
        {
            Serial.println("Données GPS reçues:");
            while
            {
                sim868.available()
                {
                    Serial.write(sim868.read());
                }
            }
        }
    }
    
    delay(5000); // Attendre avant de demander à nouveau les données
}
