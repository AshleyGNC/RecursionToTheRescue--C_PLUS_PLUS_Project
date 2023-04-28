#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "RecursionToTheRescue.h"
#include "DoctorsWithoutOrders.h"
#include "Disasters.h"
#include "Common.h"
#include "console.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"
using namespace std;

/* Everything in here is private to this file. *
namespace {   

    /* * * * * General I/O Logic * * * *
    * Prompts the user for a filename. This should eventually be switched to use openFileDialog,
    * but it seems that doing so has some kind of data race on Macs?
    */

    string haveUserChooseFile(ifstream& input, const string& prompt, const string& directory) {
        while (true) {
            string filename = getLine(prompt);
            string fullname = directory + "/" + filename;

            // First, try to open the file using the updated path name.
            {
                ifstream fileReader(fullname);
                if (fileReader) {
                    input = std::move(fileReader);
                    return fullname;
                }
            }

            // Try to open the file just using the name given.
            {
                ifstream fileReader(filename);
                if (fileReader) {
                    input = std::move(fileReader);
                    return filename;
                }
            }

            cerr << "Sorry, I can't open that file." << endl;
        }
    }

    /* Given a filename and an expected suffix, checks whether the file
     * ends with that suffix. If not, issues a warning and asks if the
     * user wants to proceed anyway.
     */
    bool proceedWithTest(const string& filename, const string& suffix) {
        if (endsWith(getTail(filename), suffix)) return true;

        cerr << "Warning: This doesn't look like a test case for this part" << endl;
        cerr << "of the assignment. Test cases for this part of the assignment" << endl;
        cerr << "should have the " << suffix << " suffix." << endl;

        return getYesOrNo("Do you want to continue anyway? ");
    }

    /* * * * * Logic for Doctors Without Orders * * * * */

    /* Prompts the user to choose a hospital test case, returning the contents of that test. */
    HospitalTestCase userChooseHospitalTest() {
        while (true) {
            ifstream input;
            string filename = haveUserChooseFile(input, "Enter test file name: ", "DoctorsWithoutOrders");

            if (!filename.empty() && input && proceedWithTest(filename, ".dwo")) {
                return loadHospitalTestCase(input);
            }
        }
    }

    /* Prints out a human-readable representation of the schedule. */
    void printScheduleNicely(const Set<string>& toSee, const HospitalTestCase& testCase, int hoursFree) {
        /* Convert the patients into a map from patients to hours. */
        Map<string, int> patientsToHours;
        for (const auto& patient: testCase.patients) {
            patientsToHours[patient.name] = patient.hoursNeeded;
        }

        /* List all the patients with their hours needed */
        int totalHours = 0;
        for (const auto& patient: toSee) {
            cout << "    Patient " << patient << " (" << pluralize(patientsToHours[patient], "hour") << " needed)" << endl;
            totalHours += patientsToHours[patient];
        }

        if (totalHours > hoursFree) {
            cerr << "  This doctor's demands exceed their available hours!" << endl;
        }
    }

    /* Given a schedule and the list of all doctors and patients, reports whether all patients
     * were actually seen.
     */
    bool allPatientsSeen(const HospitalTestCase& testCase, const Map<string, Set<string>>& schedule) {
        Set<string> seen;
        for (const string& doctor: schedule) {
            seen += schedule[doctor];
        }

        Set<string> allPatients;
        for (const auto& patient: testCase.patients) {
            allPatients += patient.name;
        }

        return seen == allPatients;
    }

    /* Given a test case, runs the student code on that test case, reporting the result and any
     * mismatches with the file.
     */
    void processHospitalTestCase(const HospitalTestCase& testCase) {
        Map<string, Set<string>> schedule;
        if (canAllPatientsBeSeen(testCase.doctors, testCase.patients, schedule)) {
            /* Report the answer either to stdout or to stderr depending
             * on whether they got the answer right
             */
            cout << "Good news! Everyone can indeed be seen." << endl;
            for (const auto& doctor: testCase.doctors) {
                cout << "  Doctor " << doctor.name << " (" << pluralize(doctor.hoursFree, "hour") << " free) sees these patients: " << endl;
                printScheduleNicely(schedule[doctor.name], testCase, doctor.hoursFree);
            }

            if (!allPatientsSeen(testCase, schedule)) {
                cerr << "Not all patients were scheduled to be seen!" << endl;
            }
            if (!testCase.isPossible) {
                cerr << "The test case reports that there should not be a way to schedule everyone!" << endl;
            } else {
                cout << "The test case confirms your result that there should be a way to schedule everyone." << endl;
            }
        } else {
            cout << "Oh no! Not all patients can be seen." << endl;

            if (testCase.isPossible) {
                cerr << "The test case reports that a schedule should exist!" << endl;
            } else {
                cout << "The test case confirms your result that there should not be a way to schedule everyone." << endl;
            }
        }
    }

    /* Displays the doctors and patients in a given hospital test case. */
    void showHospitalTest(const HospitalTestCase& test) {
        cout << "Here are the available doctors: " << endl;
        for (const auto& doctor: test.doctors) {
            cout << "   Doctor " << doctor.name << " (" << pluralize(doctor.hoursFree, "hour") << " free)" << endl;
        }
        cout << "Here are the patients that need to be seen: " << endl;
        for (const auto& patient: test.patients) {
            cout << "  Patient " << patient.name << " (" << pluralize(patient.hoursNeeded, "hour") << " needed)" << endl;
        }
    }

    /* Prompts the user for a hospital test, runs it, then asks to continue. */
    void testCanPatientsBeSeen() {
        do {
            HospitalTestCase testCase = userChooseHospitalTest();
            showHospitalTest(testCase);
            processHospitalTestCase(testCase);
        } while (getYesOrNo("Another test case? "));
    }

    /* * * * * Disaster Preparation Test Logic * * * * */

    /* Prompts the user to choose a disaster test file, returning the contents of whatever
     * file was chosen.
     */
    DisasterTest userChooseDisasterTest() {
        while (true) {
            ifstream input;
            string filename = haveUserChooseFile(input, "Enter test file name: ", "DisasterPlanning");

            if (!filename.empty() && input && proceedWithTest(filename, ".dst")) {
                return loadDisaster(input);
            }
        }
    }

    /* Confirms that all cities are indeed covered by a set of test cities. */
    bool allCitiesCovered(const Set<string>& stockpileCities, const Map<string, Set<string>>& network) {
        Set<string> covered = stockpileCities;
        for (const string& city: stockpileCities) {
            covered += network[city];
        }

        Set<string> allCities;
        for (const string& city: network) {
            allCities += city;
        }

        return allCities == covered;
    }

    /* Runs the student code on the given disaster test, showing the result and comparing it against
     * our reference answer.
     */
    void runOneDisasterTest(const DisasterTest& test, int numCities) {
        Set<string> allCities;
        for (string city : test.network) allCities.add(city);
        setCitiesToStockpile(allCities, false);
        Set<string> stockpileCities;
        if (canBeMadeDisasterReady(test.network, numCities, stockpileCities)) {
            cout << "Yes! Your function reports that you can prepare for a disaster." << endl;
            cout << "Specifically, stockpile in these cities: " << endl;
            for (const string& city: stockpileCities) {
                cout << "  " << city << endl;
            }

            setCitiesToStockpile(stockpileCities, true);

            if (numCities < test.minCitiesNeeded) {
                cerr << "Your answer uses fewer cities than the test case says should be possible." << endl;
            }
            if (stockpileCities.size() > numCities) {
                cerr << "You've used more than " << pluralize(numCities, "city", "cities") << " in your solution!" << endl;
            }
            if (!allCitiesCovered(stockpileCities, test.network)) {
                cerr << "Your answer does not provide disaster coverage to all cities." << endl;
            }
        } else {
            cout << "Unfortunately, that's not enough cities to stockpile." << endl;
            if (numCities >= test.minCitiesNeeded) {
                cerr << "The test case says that this should be possible." << endl;
            } else {
                cout << "This matches our test case." << endl;
            }
        }
    }

    /* Given a disaster test, loops while the student wants to run that test case on a bunch of
     * different numbers.
     */
    void processDisasterTest(const DisasterTest& test) {
        /* Pull up the map so we can see what we're looking at. */
        displayNetwork(test);
        cout << "Stockpiles are displayed in yellow, covered cities in white." << endl;

        while (true) {
            string input = getLine("Enter a number of cities, or press ENTER to end: ");
            if (trim(input).empty()) break;

            /* The input might not be numeric. If it isn't, reprompt. */
            int numCities;
            try {
                numCities = stringToInteger(input);
            } catch (...) {
                cout << "Please enter an integer." << endl;
                continue;
            }

            runOneDisasterTest(test, numCities);
        }
    }

    /* Prompts the user to try out the disaster prep code on a number of test cases. */
    void testCanBeMadeDisasterReady() {
        do {
            auto testCase = userChooseDisasterTest();
            processDisasterTest(testCase);
        } while (getYesOrNo("Another test case?" ));
    }


    /* * * * * Main Driver Logic * * * * */

    /* Enumerated type representing all the different menu options. */
    enum MenuOption {
        LargestInvalidOption,
        TestDoctorsWithoutOrders,
        TestDisasterPreparation,
        Quit,
        NumOptions
    };

    /* Invokes the proper testing code for a given selection. */
    void processMenuSelection(MenuOption option) {
        switch (option) {
        case TestDoctorsWithoutOrders:
            testCanPatientsBeSeen();
            break;
        case TestDisasterPreparation:
            testCanBeMadeDisasterReady();
            break;
        case Quit:
            cout << "Have a nice day!" << endl;
            exit(0);
        default:
            error("Unknown menu option?");
        }
    }

    /* Displays the menu of what's available. */
    void printMenu() {
        cout << "Enter your selection: " << endl;
        cout << "  " << TestDoctorsWithoutOrders         << ": Test Doctors Without Orders" << endl;
        cout << "  " << TestDisasterPreparation          << ": Test Disaster Preparation" << endl;
        cout << "  " << Quit                             << ": Quit" << endl;
    }

    /* Prompts the user for a menu selection, reprompting as necessary. */
    MenuOption getMenuSelection() {
        while (true) {
            MenuOption option = MenuOption(getInteger("Enter your choice: "));
            if (option > LargestInvalidOption && option < NumOptions) {
                return option;
            }
            cerr << "Sorry, that choice was invalid." << endl;
        }
    }
}
int main(){
    while (true) {
        printMenu();
        processMenuSelection(getMenuSelection());
    }
    return 0; // Unreachable; silences warnings
}
