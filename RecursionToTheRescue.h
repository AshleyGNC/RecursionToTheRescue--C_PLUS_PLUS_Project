#ifndef RecursionToTheRescue_Included
#define RecursionToTheRescue_Included

#include <string>
#include <ostream>
#include "vector.h"
#include "set.h"
#include "map.h"


/* * * * * Doctors Without Orders * * * * */

/**
 * A type representing a doctor and the number of hours they can work.
 */
struct Doctor {
    std::string name; // Their name, for testing purposes.
    int hoursFree;    // How many hours they have free to work.
};

/**
 * A type representing a patient and the number of hours they need to be seen for.
 */
struct Patient {
    std::string name; // Their name, for testing purposes.
    int hoursNeeded;  // How many hours they must be seen for.
};

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<std::string, Set<std::string>>& schedule);




/* * * * * Disaster Preparation * * * * */

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
bool canBeMadeDisasterReady(const Map<std::string, Set<std::string>>& roadNetwork,
                            int numCities,
                            Set<std::string>& locations);


/* * * * * Printing Routines * * * * */

/* These functions allow you to print out our structs using cout. You don't need to use these
 * functions, but they might be helpful during testing. They're implemented in Printing.cpp
 * in case you're curious how they work.
 */
std::ostream& operator<< (std::ostream& out, const Doctor& doc);
std::ostream& operator<< (std::ostream& out, const Patient& patient);


#endif
