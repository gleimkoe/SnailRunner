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
			std::cout << "Distanzsensor vorne: " << robot->threshold_distance << std::endl;
			std::cout << "Distanzsensor rechts: " << robot->threshold_distance_no_side << std::endl;

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
			std::cout << "[3] - Distanzsensor vorne" << std::endl;
			std::cout << "[4] - Distanzsensor rechts" << std::endl;
            // std::cout << "[3] - grau" << std::endl;

            std::cin >> threshold_choice;

            if (threshold_choice == 1)
            {
                unsigned temp = 0;
                std::cout << std::endl;
                std::cout << " --- Unterer Graubereich --- " << std::endl;
                std::cout << "aktueller Wert: " << robot->threshold_grey_low << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 3000)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 3000);

                robot->threshold_grey_low = temp;
            }

            else if (threshold_choice == 2)
            {
                unsigned temp = 0;
                std::cout << std::endl;
                std::cout << " --- Oberer Graubereich --- " << std::endl;
                std::cout << "aktueller Wert: " << robot->threshold_grey_high << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 3000)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 3000);

                robot->threshold_grey_high = temp;
            }

            else if (threshold_choice == 3)
            {
                unsigned temp = 0;
                std::cout << " --- Distanzsensor vorne --- " << std::endl;
                std::cout << "aktueller Wert: " << robot->threshold_distance << std::endl;
                std::cout << "neuer Wert:     ";

                do
                {
                    if (temp > 30)
                    {
                        std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
                        std::cout << "neuer Wert:     ";
                    }

                    std::cin >> temp;
                } while (temp > 3000);

                robot->threshold_distance = temp;
            }

			else if (threshold_choice == 4)
			{
				unsigned temp = 0;
				std::cout << " --- Distanzsensor rechts --- " << std::endl;
				std::cout << "aktueller Wert: " << robot->threshold_distance_no_side << std::endl;
				std::cout << "neuer Wert:     ";

				do
				{
					if (temp > 30)
					{
						std::cout << "Wert liegt nicht im gueltigen Bereich." << std::endl;
						std::cout << "neuer Wert:     ";
					}

					std::cin >> temp;
				} while (temp > 3000);

				robot->threshold_distance_no_side = temp;
			}
            break;
        }

        case 3: // calibration
        {
            unsigned color_choice = 0;
            int measurement_amount = 100;
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

			//DURCHSCHNITT WEISS UND IN WEISS VARIABLE SPEICHERN
			// drive, take measurements, sum up and save average to while variable
            robot->forward(0.5, METER);
            for (int i = 0; i < measurement_amount; i++)//=
            {
                values.push_back(robot->colourdown().value());
				WaitUntilIsOver(20);
            }

            for (int i = 0; i < measurement_amount; i++)//=
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

			//DURCHSCHNITT SCHWARZ UND IN SCHWARZ VARIABLE SPEICHERN
			// drive, take measurements, sum up and save average to while variable
            robot->forward(0.5, METER);
            for (int i = 0; i < measurement_amount; i++)//=
            {
				values.push_back(robot->colourdown().value());
				WaitUntilIsOver(20);
            }

            for (int i = 0; i < measurement_amount; i++)//=
            {
                sum += values.at(i);
            }

            average = sum / (measurement_amount);
            robot->black = average;

			dummy = "nichtok";

			

            std::cout << "Durchschnitt schwarz: " << average << std::endl;

			// Werte zuruecksetzen fuer naechste Kalibrierung
			dummy = "0";
			sum = 0;
			average = 0;
			values.clear();

			std::cout << "Der Roboter muss auf grauen Untergrund gestellt werden. Bereit? Tippe ok." << std::endl;
			while (dummy != "ok")
			{
				std::cin >> dummy;
			}

			dummy = "nichtok";

			//DURCHSCHNITT GRAU UND IN GRAU VARIABLE SPEICHERN
			// drive, take measurements, sum up and save average to while variable
			robot->forward(0.5, METER);
			for (int i = 0; i < measurement_amount; i++)//=
			{
				values.push_back(robot->colourdown().value());
				WaitUntilIsOver(20);
			}

			for (int i = 0; i < measurement_amount; i++)//=
			{
				sum += values.at(i);
			}

			average = sum / (measurement_amount);
			robot->grey = average;

			dummy = "nichtok";



			std::cout << "Durchschnitt grau: " << average << std::endl;
			std::cout << "Schwellwerte werden berechnet... "  << std::endl;

            //robot->grey =(robot->black + robot->white)/2;
           robot->threshold_grey_low = (robot->grey + robot->white)/2 + 500;
           robot->threshold_grey_high = (robot->grey + robot->black)/2;

		   std::cout << "Grau/Weiss: " << robot->threshold_grey_low << std::endl;
		   std::cout << "Grau/Schwarz: "<< robot->threshold_grey_high << std::endl;
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

            std::ofstream file("threshold_values.txt");

            file << robot->threshold_grey_low << std::endl;
            file << robot->threshold_grey_high << std::endl;
			file << robot->threshold_distance << std::endl;
			file << robot->threshold_distance_no_side << std::endl;


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

                robot->threshold_grey_low =  1500;
                robot->threshold_grey_high = 1600;
				robot->threshold_distance = 10;
				robot->threshold_distance_no_side = 30;

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

				std::getline(file, temp);
				robot->threshold_distance = stoi(temp);
				std::cout << "Distanzsensor vorne: " << temp << std::endl;

				std::getline(file, temp);
				robot->threshold_distance_no_side = stoi(temp);
				std::cout << "Distanzsensor rechts: " << temp << std::endl;



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

			bool temp = 1;

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
//                              FUNCTION 4 - LOGGING
//
/**********************************************************************************/


/**********************************************************************************/
//								KEEPING TRACK OF TIME
/**********************************************************************************/



// sets start time. needs to called when round is started aka when the relay got handed over
void startTimer(SnailRunner* robot)
{
	robot->start_time = std::clock();
}

// gets called when an event like obstacle or offtrail happens
double getElapsedTime(SnailRunner* robot)
{
	std::clock_t current_time = std::clock();
	return static_cast<double>(current_time - robot->start_time) / CLOCKS_PER_SEC;
}

// sets endtime. needs to be called when relay is handed over
void endTimer(SnailRunner* robot)
{
	robot->end_time = std::clock();
}

// returns elapsed time of a round based on start_time and end_time
double getDuration(SnailRunner* robot)
{
	double elapsed_time = static_cast<double>(robot->end_time - robot->start_time) / CLOCKS_PER_SEC;
	return elapsed_time;
}

// helperfunction to get timestamp as string
std::string getTimeStamp()
{
	std::time_t now = time(nullptr);
	tm* local_time = localtime(&now);

	std::string output;
	int hours, minutes, seconds;

	hours = local_time->tm_hour;
	minutes = local_time->tm_min;
	seconds = local_time->tm_sec;

	
	if (hours > 9)
	{
		//std::cout << hours << ":";
		output += to_string(hours) + ":";
	}
	else
	{
		//std::cout << "0" << hours << ":";
		output += "0" + to_string(hours) + ":";
	}

	if (minutes > 9)
	{
		//std::cout << minutes << ":";
		output += to_string(minutes) + ":";
	}
	else
	{
		//std::cout << "0" << minutes << ":";
		output += "0" + to_string(minutes) + ":";
	}
	
	if (seconds > 9)
	{
		//std::cout << seconds;
		output += to_string(seconds);
	}
	else
	{
		//std::cout << "0" << seconds;
		output += "0" + to_string(seconds);
	}

	return output;
}



/**********************************************************************************/
//								KEEPING TRACK OF TRACK
/**********************************************************************************/

// Methods to keep track of the track
void resetDistance(SnailRunner* robot)
{
	robot->lapdistance = 0;
}

// gets called when back on state on_trail
void resetEncoder(SnailRunner* robot)
{
	robot->left().encoder().clear();
}

// gets called when in state off_trail
void addtoDistance(SnailRunner* robot)
{
	robot->lapdistance += robot->left().encoder().value();
}



/**********************************************************************************/
//								LOG FUNCTIONS
/**********************************************************************************/

// LOG STARTING CONDITIONS
/*
*********************************
Position: 	      Startlaeufer
Fahrtrichtung:    Uhrzeigersinn
Rundenanzahl:	  2
Startzeit:        11:54:23
*********************************
*/
void logStartConditions(SnailRunner* robot, std::ofstream &file)
{
    file << "*********************************" << std::endl;
    file << "Position: 	  ";
    if (robot->start_position == 1)
    {
		file << "Startlaeufer" << std::endl;
    }
	else //(robot->start_position == -1)
    {
		file << "Weiterlaeufer" << std::endl;
    }

    file << "Fahrtrichtung:    ";
    if (robot->direction == 1)
    {
        file << "Uhrzeigersinn" << std::endl; // maybe left
    }
    else
    {
        file << "gegen den Uhrzeigersinn" << std::endl; // maybe right
    }

	file << "Rundenanzahl:     " << robot->lap_amount << std::endl;
	file << "Startzeit:        " << getTimeStamp();
    file << "\n*********************************" << std::endl << std::endl << std::endl;
}

// LOG LAP BANNER
/*
---------------------------------
			  Runde 2
---------------------------------
*/
void logLapBanner(SnailRunner* robot, std::ofstream &file)
{
    file << "---------------------------------" << std::endl;
    file << "             Runde "				<< robot->current_lap << std::endl;
    file << "---------------------------------" << std::endl;
}

// LOG OFF_TRAIL
/*
Abweichung   10.432s
*/
void logOFF_TRAIL(SnailRunner* robot, std::ofstream &file)
{
	robot->offtrail_count++;
	file << "Abweichung	  " << getElapsedTime(robot) << "s" << endl;
}

//logObstacle
/*
Hindernis   20.746s
*/
void logObstacle(SnailRunner* robot, std::ofstream &file)
{
	robot->obstacle_count++;
	file << "Hindernis         " << getElapsedTime(robot) << "s" << endl;
}

// LOG CORNER
/*
2. Ecke            15.534
*/
void logCorner(SnailRunner* robot, std::ofstream &file)
{
	file << robot->current_corner << ". Ecke           " << getElapsedTime(robot) << endl;
}

// TODO - funktioniert die Streckenmessung?
/*
1. Runde abgeschlossen:
Zeit:             43.54s
Abweichungen:     5
Strecke:          256cm
*/
void logLapConclusion(SnailRunner* robot, std::ofstream &file)
{
    file << std::endl << robot->current_lap  << ". Runde abgeschlossen:" << std::endl;
    file << "Zeit:             " << getElapsedTime(robot) << "s" <<std::endl;
    file << "Abweichungen:     " << robot->offtrail_count << std::endl;
	file << "Hindernisse:      " << robot->obstacle_count << std::endl;
	file << "Strecke:          " << ((robot->lapdistance / 60.0) / 2.0)*M_PI*0.05/*((1.0 / 75.0)* robot->lapdistance*M_PI*0.05 / 360.0) / 100.0 */<< "m" << std::endl << std::endl << std::endl;

	std::cout << std::endl << std::endl;
}
