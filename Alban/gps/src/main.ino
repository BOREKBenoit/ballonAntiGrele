const int joursParMois[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int nombre = 1;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);  // Utilisation du port série matériel du MKR 1310 (RX = 0, TX = 1)

    Serial.println("- Activation du GPS 🛰️");
    Serial1.println("AT+CGNSPWR=1");  // Activation du GPS
    delay(500);

    Serial1.println("AT+CGNSINF");    // Demande des données GPS
    Serial.println("- Requête AT+CGNSINF envoyée 📡");
}

void loop()
{
    unsigned long secondes = 0;
    String trame;
    
    if (Serial1.available())
    {
        String data = Serial1.readStringUntil('\n');
        if (data.startsWith("+CGNSINF:"))
        {
            int startIndex = 0;
            for (int i = 0; i < 2; i++)
            {
                startIndex = data.indexOf(',', startIndex + 1);
                if (startIndex == -1) return;
            }
            int pos1 = startIndex + 1;
            int pos2 = data.indexOf(',', pos1);
            int pos3 = data.indexOf(',', pos2 + 1);
            int pos4 = data.indexOf(',', pos3 + 1);

            String utc = data.substring(pos1, pos2);

            int jour = utc.substring(6, 8).toInt();
            int mois = utc.substring(4, 6).toInt();
            int annee = utc.substring(0, 4).toInt();
            int heure = utc.substring(8, 10).toInt();
            int minute = utc.substring(10, 12).toInt();
            int seconde = utc.substring(12, 14).toInt();
            String latitude = data.substring(pos2 + 1, pos3);
            String longitude = data.substring(pos3 + 1, pos4);
            
            // Calcul des secondes depuis 1970
            for (int i = 1970; i < annee; i++)
            {
                if ((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0))
                {
                    secondes += 366 * 86400;
                }
                else
                {
                    secondes += 365 * 86400;
                }
            }

            for (int i = 0; i < (mois - 1); i++)
            {
                secondes += (unsigned long)joursParMois[i] * 86400;
            }

            if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0))
            {
                if (mois > 2)
                {
                    secondes += 86400;
                }
            }

            secondes += (unsigned long)(jour - 1) * 86400;
            secondes += (unsigned long)heure * 3600;
            secondes += (unsigned long)minute * 60;
            secondes += (unsigned long)seconde;
            
            trame += secondes;
            trame += " ";         // à commenter
            trame += latitude;
            trame += " ";         // à commenter
            trame += longitude;

            // Supprimer les points en remplaçant "." par ""
            trame.replace(".", "");

            Serial.println("---------------------------------------");
            Serial.print(nombre);
            Serial.print(") ");
            nombre++;
            Serial.println(trame);
            Serial.println("---------------------------------------");
            delay(5000);
            Serial1.println("AT+CGNSINF");
        }
    }
}
