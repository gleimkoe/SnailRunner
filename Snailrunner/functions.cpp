#include "functions.h"

/**********************************************************************************/
//
//                              FUNCTION 1 - CALIBRATION
//
/**********************************************************************************/

void calibrationMenu(SnailRunner* robot)
{
    bool calibration_done = false;

    while (!calibration_done)
    {
        system("cls");
        unsigned choice = 0;

        std::cout << "--- SCHWELLWERT MENU ---" << std::endl;
        std::cout << "[1] - aktuelle Schwellwerte anzeigen" << std::endl;
        std::cout << "[2] - Schwellwerte aendern" << std::endl;
        std::cout << "[3] - automatische Kalibrierung" << std::endl;
        std::cout << "[4] - Schwellwerte speichern" << std::endl;
        std::cout << "[5] - Schwellwerte laden" << std::endl;
        std::cout << "[6] - Beenden" << std::endl;
        std::cout << "Eingabe: ";

        std::cin >> choice;

        switch (choice)
        {
        case 1: // show current threshold values
        {
            std::cout << std::endl;
            std::cout << "--- Aktuelle Schwellwerte ---" << std::endl;
            std::cout << "Untere Graugrenze:   " << robot->threshold_grey_low << std::endl;
            std::cout << "oberere Graugrenze: " << robot->threshold_grey_high << std::endl;
            // std::cout << "grau:    " << grey << std::endl;
            std::cout << std::endl;

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            std::string dummy;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 2: // change threshold values manually
        {
            int threshold_choice = 0;
            std::cout << std::endl;
            std::cout << "--- Schwellwerte manuell aendern ---" << std::endl;
            std::cout << "Welche Werte sollen geaendert werden?" << std::endl;
            std::cout << "[1] - unterer Graubereich" << std::endl;
            std::cout << "[2] - oberer Graubereich" << std::endl;
            // std::cout << "[3] - grau" << std::endl;

            std::cin >> threshold_choice;

            if (threshold_choice == 1)
            {
                int temp = 0;
                std::cout << std::endl;
                std::cout << " --- Unterer Graubereich --- " << std::endl;
                std::cout << "aktueller Wert: " << robot->threshold_grey_low << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 1000)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 1000);

                robot->threshold_grey_low = temp;
            }

            else if (threshold_choice == 2)
            {
                int temp = 0;
                std::cout << std::endl;
                std::cout << " --- Oberer Graubereich --- " << std::endl;
                std::cout << "aktueller Wert: " << robot->threshold_grey_high << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 1000)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 1000);

                robot->threshold_grey_high = temp;
            }
            /*
            else if (threshold_choice == 3)
            {
                int temp = 0;
                std::cout << " --- Grau --- " << std::endl;
                std::cout << "aktueller Wert: " << white << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 1000)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 1000);

                grey = temp;
            }
            */
            break;
        }

        case 3: // calibration
        {
            unsigned color_choice = 0;
            int measurement_amount = 5;
            std::string dummy;
            std::vector<int> values;
            unsigned sum = 0;
            unsigned average = 0;

            std::cout << std::endl;
            std::cout << "--- Kalibrierung ---" << std::endl;
            std::cout << "Der Roboter muss auf weissen Untergrund gestellt werden. Bereit? Tippe ok." << std::endl;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            // Fahren, Messwerte aufnehmen, summieren, Durchschnitt bilden und und in white-Variable speichern

            robot->forward(0.5, METER);
            for (int i = 0; i < measurement_amount; i++)
            {
                // values.push_back(robot->colourdown().value());
                values.push_back(robot->colourdown().value());
                values.push_back(robot->colourdown().value());
                values.push_back(robot->colourdown().value());
                values.push_back(robot->colourdown().value());
                values.push_back(robot->colourdown().value());
            }

            // get average of measured values
            for (int i = 0; i < measurement_amount; i++)
            {
                sum += values.at(i);
            }

            average = sum / (measurement_amount);
            robot->white = average;

            std::cout << "Durchschnitt weiss: " << average << std::endl;

            // Werte zuruecksetzen fuer naechste Kalibrierung
            dummy = "0";
            sum = 0;
            average = 0;
            values.clear();

            std::cout << "Der Roboter muss auf schwarzen Untergrund gestellt werden. Bereit? Tippe ok." << std::endl;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

			dummy = "nichtok";

            robot->forward(0.5, METER);
            for (int i = 0; i < measurement_amount; i++)
            {
				values.push_back(robot->colourdown().value());
				values.push_back(robot->colourdown().value());
				values.push_back(robot->colourdown().value());
				values.push_back(robot->colourdown().value());
				values.push_back(robot->colourdown().value());
            }

            // get average of measured values
            for (int i = 0; i < measurement_amount; i++)
            {
                sum += values.at(i);
            }

            average = sum / (measurement_amount);
            robot->black = average;

            std::cout << "Durchschnitt schwarz: " << average << std::endl;

            std::cout << "Schwellwerte werden berechnet... " << average << std::endl;

            robot->grey =(robot->black + robot->white)/2;
            robot->threshold_grey_low = robot->grey - 200;
            robot->threshold_grey_high = robot->grey + 200;

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 4: // save threshold values into file
        {
            std::cout << std::endl;
            std::cout << "--- Speichern in Datei ---" << std::endl;
            std::cout << "Die aktuellen Schwellwerte werden in einer Datei (threshold_values.txt) gespeichert." << std::endl;

            std::ofstream file;

            file.open("threshold_values.txt"); // falls Datei nicht vorhanden wird sie hier erstellt

            file << robot->threshold_grey_low << std::endl;
            file << robot->threshold_grey_high << std::endl;

            file.close();

            std::cout << "Werte wurden gespeichert." << std::endl;

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            std::string dummy;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 5: // read threshold values from file
        {
            std::cout << std::endl;
            std::cout << "--- Aus Datei einlesen ---" << std::endl;

            std::ifstream file("threshold_values.txt");
            if (!file)
            {
                std::cerr << "Error: Datei konnte nicht geoeffnet werden. Standardwerte werden geladen." << std::endl;

                robot->threshold_grey_low = 1300;
                robot->threshold_grey_high = 1600;
                // grey = 0;

                std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
                std::string dummy;
                while (dummy != "ok")
                {
                    std::cin >> dummy;
                }
            }
            else // read from file line by line and store values in variables
            {
                std::string temp;
                std::getline(file, temp);
                robot->threshold_grey_low = stoi(temp);
                std::cout << "untere Graugrenze: " << temp << std::endl;

                std::getline(file, temp);
                robot->threshold_grey_high = stoi(temp);
                std::cout << "obere Graugrenze: " << temp << std::endl;
                /*
                                std::getline(file, temp);
                                grey = stoi(temp);
                */

                file.close();

                std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
                std::string dummy;
                while (dummy != "ok")
                {
                    std::cin >> dummy;
                }
            }

            break;
        }

        case 6: // end calibration
        {
            calibration_done = true;
            break;
        }

        default: // capture wrong input
        {
            std::cout << "Ungueltige Eingabe" << std::endl;
            break;
        }
        }
    }
}

/**********************************************************************************/
//
//                              FUNCTION 2 - SETTINGS
//
/**********************************************************************************/

void settingsMenu(SnailRunner* robot)
{
    bool settings_done = false;

    while (!settings_done)
    {
        system("cls");
        unsigned choice = 0;

        std::cout << "--- EINSTELLUNGEN ---" << std::endl;
        std::cout << "[1] - Startposition" << std::endl;
        std::cout << "[2] - Fahrtrichtung" << std::endl;
        std::cout << "[3] - Rundenanzahl" << std::endl;
        std::cout << "[4] - Beenden" << std::endl;
        std::cout << "Eingabe: ";

        std::cin >> choice;

        switch (choice)
        {
        case 1: // runner/follower
        {
            std::cout << std::endl;
            std::cout << "--- STARTPOSITION ---" << std::endl;
            std::cout << "Welche Position hat der Runner?" << std::endl;
            std::cout << "[1] - Startlaeufer" << std::endl;
            std::cout << "[0] - Weiterlaeufer" << std::endl;

            std::cin >> robot->start_position;

            if (robot->start_position)
            {
                std::cout << "Der Runner startet als Startlaeufer" << std::endl;
            }
            else
            {
                std::cout << "Der Runner startet als Weiterlaeufer" << std::endl;
            }

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            std::string dummy;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 2: // direction - clockwise/counterclockwise
        {
            std::cout << std::endl;
            std::cout << "--- RICHTUNG WAEHLEN ---" << std::endl;
            std::cout << "Soll der Runner im Uhrzeigersinn oder gegen den Uhrzeigersinn fahren?" << std::endl;
            std::cout << "[1] - im Uhrzeigersinn" << std::endl;
            std::cout << "[0] - gegen den Uhrzeigersinn" << std::endl;

			bool temp = 0;

            std::cin >> temp;

            if (temp)
            {
				robot->direction = 1;
                std::cout << "Der Runner faehrt jetzt im Uhrzeigersinn" << std::endl;
            }
            else
            {
				robot->direction = -1;
                std::cout << "Der Runner faehrt jetzt gegen den Uhrzeigersinn" << std::endl;
            }

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            std::string dummy;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 3: // amount of laps
        {
            unsigned temp = 1;
            std::cout << std::endl;
            std::cout << "--- RUNDENANZAHL ---" << std::endl;
            std::cout << "Wie viele Runden soll der Runner fahren?" << std::endl;
            std::cout << "Rundenanzahl: ";

            do
            {
                if (temp > 2 || temp == 0)
                {
                    std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                    std::cout << "Rundenanzahl: ";
                }

                std::cin >> temp;
            } while (temp > 2 || temp == 0);

            robot->lap_amount = temp;

            std::cout << "\nDer Runner faehrt nun " << robot->lap_amount << " Runden." << std::endl;

            std::cout << "Tippe 'ok' um ins Hauptmenue zu gelangen." << std::endl;
            std::string dummy;
            while (dummy != "ok")
            {
                std::cin >> dummy;
            }

            break;
        }

        case 4: // end menu
        {
            settings_done = true;
            break;
        }

        default:
        {
            std::cout << "Ungueltige Eingabe" << std::endl;
            break;
        }
        }
    }
}

/**********************************************************************************/
//
//                              FUNCTION 3 - RELAY RUN
//
/**********************************************************************************/

/**********************************************************************************/
//
//                              FUNCTION 4 - LOGGING
//
/**********************************************************************************/


//helperfunction to get timestamp

void getTimeStamp()
{
	std::time_t now = time(nullptr);
	tm* local_time = localtime(&now);

	int hours, minutes, seconds;

	hours = local_time->tm_hour;
	minutes = local_time->tm_min;
	seconds = local_time->tm_sec;

	
	if (hours > 9)
	{
		std::cout << hours << ":";
	}
	else
	{
		std::cout << "0" << hours << ":";
	}

	if (minutes > 9)
	{
		std::cout << minutes << ":";
	}
	else
	{
		std::cout << "0" << minutes << ":";
	}
	
	if (seconds > 9)
	{
		std::cout << seconds;
	}
	else
	{
		std::cout << "0" << seconds;
	}
}


//Methods to keep track of the track
void resetDistance(SnailRunner* robot)
{
	robot->lapdistance = 0;
}

//gets called when back on state on_trail
void resetEncoder(SnailRunner* robot)
{
	robot->left().encoder().clear();
}

//gets called when in state off_trail
void addtoDistance(SnailRunner* robot)
{
	robot->lapdistance += robot->left().encoder().value();
}


// TODO - Uhrzeitdummy muss mit Funktion ersetzt werden, die die Uhrzeit zurückgibt
void logStartConditions(SnailRunner* robot)
{
    std::cout << "*********************************" << std::endl;
    std::cout << "Position: 	  ";
    if (robot->direction)
    {
        std::cout << "Startlaeufer" << std::endl;
    }
    else
    {
        std::cout << "Weiterlaeufer" << std::endl;
    }

    std::cout << "Fahrtrichtung:    ";
    if (robot->start_position)
    {
        std::cout << "Uhrzeigersinn" << std::endl; // maybe left
    }
    else
    {
        std::cout << "gegen den Uhrzeigersinn" << std::endl; // maybe right
    }
    std::cout << "Rundenanzahl:     " << robot->lap_amount << std::endl;
	std::cout << "Startzeit:        ";
	getTimeStamp();
    std::cout << "\n*********************************" << std::endl
              << std::endl
              << std::endl;
}

// DONE
void logLapBanner(SnailRunner* robot)
{
    std::cout << "---------------------------------" << std::endl;
    std::cout << "             Runde " << robot->current_lap << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

// TODO - Uhrzeitdummy muss mit Funktion ersetzt werden, die die Uhrzeit zurückgibt
void logOFF_TRAIL(SnailRunner* robot)
{
	std::cout << "Abweichung	  "; getTimeStamp();
              //<< "11:45:56" << std::endl;
}

// TODO - Uhrzeitdummy muss mit Funktion ersetzt werden, die die Uhrzeit zurückgibt
void logCorner(SnailRunner* robot)
{
	std::cout << robot->current_corner << ". Ecke           "; getTimeStamp();
              //<< "11:45:45" << std::endl;
}

// TODO - offtrail_count muss im state OFF_TRAIL oder so inkrementiert werden
// TODO - Uhrzeitdummy muss mit Funktion ersetzt werden, die die Uhrzeit zurückgibt
// TODO - Strecke muss irgendwie gemessen werden. WTF wie? voll aufwändig. permanent die Impulse vom Motor zählen und daraus die Strecke berechnen?
void logLapConclusion(SnailRunner* robot)
{
    std::cout << std::endl
              << robot->current_lap << ". Runde abgeschlossen:" << std::endl;
    std::cout << "Zeit:             "
              << "1 Min. 12 Sek." << std::endl;
    std::cout << "Abweichungen:     " << robot->offtrail_count << std::endl;
    std::cout << "Strecke:          "
              << ((1 / 75)*robot->lapdistance*M_PI*0.05 / 360) / 100 << std::endl
              << std::endl
              << std::endl;
}
