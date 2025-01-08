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
 *    Installer la bibliothèque
 *
 * Copyright             :
 *    © 2024 Alban de Farcy de Pontfarcy
 **************************************************************************/

 #include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Fonction pour initialiser la connexion série
bool initSerial(const string& port, int baudRate, int& serialHandle) {
#ifdef _WIN32
    serialHandle = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (serialHandle == INVALID_HANDLE_VALUE) {
        cerr << "Erreur : Impossible d'ouvrir le port série !" << endl;
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(serialHandle, &dcbSerialParams)) {
        cerr << "Erreur : Impossible d'obtenir les paramètres du port série !" << endl;
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serialHandle, &dcbSerialParams)) {
        cerr << "Erreur : Impossible de configurer le port série !" << endl;
        return false;
    }
#else
    serialHandle = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (serialHandle < 0) {
        cerr << "Erreur : Impossible d'ouvrir le port série !" << endl;
        return false;
    }

    termios tty = {0};
    if (tcgetattr(serialHandle, &tty) != 0) {
        cerr << "Erreur : Impossible d'obtenir les attributs du port série !" << endl;
        return false;
    }

    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits de données
    tty.c_iflag = 0;                           // Pas de contrôle
    tty.c_oflag = 0;                           // Pas de traitement
    tty.c_lflag = 0;                           // Pas de mode canonique

    if (tcsetattr(serialHandle, TCSANOW, &tty) != 0) {
        cerr << "Erreur : Impossible de configurer le port série !" << endl;
        return false;
    }
#endif
    return true;
}

// Fonction pour envoyer des commandes AT au module
void sendATCommand(int serialHandle, const string& command) {
    string fullCommand = command + "\r\n";
#ifdef _WIN32
    DWORD bytesWritten;
    WriteFile(serialHandle, fullCommand.c_str(), fullCommand.size(), &bytesWritten, NULL);
#else
    write(serialHandle, fullCommand.c_str(), fullCommand.size());
#endif
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Fonction pour lire les données GPS du module
string readSerial(int serialHandle) {
    char buffer[256];
    string result;

#ifdef _WIN32
    DWORD bytesRead;
    ReadFile(serialHandle, buffer, sizeof(buffer), &bytesRead, NULL);
    result.assign(buffer, bytesRead);
#else
    int bytesRead = read(serialHandle, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        result.assign(buffer, bytesRead);
    }
#endif
    return result;
}

int main() {
    string port = "/dev/ttyS0"; // Remplacez par le port série utilisé (par exemple, COM3 sous Windows)
    int baudRate = 9600;       // Baud rate recommandé pour SIM868
    int serialHandle;

    if (!initSerial(port, baudRate, serialHandle)) {
        return 1;
    }

    cout << "Initialisation du module SIM868..." << endl;

    // Activer le GPS
    sendATCommand(serialHandle, "AT+CGNSPWR=1");
    this_thread::sleep_for(chrono::seconds(2));

    // Vérifier l'état GPS
    sendATCommand(serialHandle, "AT+CGNSINF");

    // Lire les données GPS
    while (true) {
        string response = readSerial(serialHandle);
        if (!response.empty()) {
            cout << "Données GPS : " << response << endl;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

#ifdef _WIN32
    CloseHandle(serialHandle);
#else
    close(serialHandle);
#endif

    return 0;
}
