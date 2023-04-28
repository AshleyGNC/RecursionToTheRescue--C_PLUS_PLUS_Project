#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "Disasters.h"
#include <iostream>
#include "vector.h"
#include <Vector>
#include "map.h"
#include "set.h"
#include "stack.h"

using namespace std;


/* * * * Doctors Without Orders * * * */

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
bool canAllPatientsBeSeenhelper(Vector<Doctor> d,
		Vector<Patient> &p, Map<string, Set<string>> &schedule, int patient) {

	if(patient == p.size()){
		return true;
	}else{
		for(int i=0; i< d.size(); i++){
			//choose
			if(d[i].hoursFree >= p[patient].hoursNeeded){
				d[i].hoursFree -= p[patient].hoursNeeded;

			//backtrack
				if(canAllPatientsBeSeenhelper(d, p, schedule, patient+1)){
					Doctor Doctor = d[i];
					Patient Patient = p[patient];
					schedule[Doctor.name].add(Patient.name);
					return true;
				}

			//unchoose
				d[i].hoursFree += p[patient].hoursNeeded;
			}
		}
	}
	return false;
}

bool canAllPatientsBeSeen(const Vector<Doctor> &doctors,
		const Vector<Patient> &patients, Map<string, Set<string>> &schedule){
	Vector<Doctor> d = doctors;
	Vector<Patient> p = patients;

	return canAllPatientsBeSeenhelper(d, p, schedule, 0);

}
/* * * * Disaster Planning * * * */

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
bool canBeMadeDisasterReadyHelper(const Map<string, Set<string>> &roadNetwork, int numCities, Set<string> &locations, Set<string> uncovered_cities){

	if(uncovered_cities.isEmpty()){
		return true;
	}
	if(numCities == 0){
		return false;
	}else{
		string uncoverCity = uncovered_cities.first();
		Set<string> original_locations = locations;
		for(string try_city : roadNetwork[uncoverCity] + uncoverCity){
			locations.add(try_city);
			if(canBeMadeDisasterReadyHelper(roadNetwork, numCities - 1, locations, uncovered_cities - (roadNetwork[try_city] + try_city))) return true;
			locations = original_locations;
		}

		return false;
	}
}


bool canBeMadeDisasterReady(const Map<string, Set<string>> &roadNetwork,
		int numCities, Set<string> &locations) {

	Set<string> uncovered_cities;
	for(string key: roadNetwork.keys()){
		uncovered_cities.add(key);
	}
	if(uncovered_cities.size() == 1){
		locations += uncovered_cities;
		return true;
	}
	return canBeMadeDisasterReadyHelper(roadNetwork, numCities, locations, uncovered_cities);

}
