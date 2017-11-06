#include <fstream>                              // to choose the input source (stdin or command line infile)
#include <cmath>                                // for sqrt and pow functions
#include <cstdlib>                              // for c_str and atoi
#include <unistd.h>                             // for getpid()
#include "simulated_annealing.h"                // include the simulated_annealing header
using namespace std;                            // direct access to std


//********************************************************************************************************************
// Program for usage error handling
//********************************************************************************************************************
static void usage(char *argv[]) {
    cerr << "Usage: " << argv[0] << " [input-file]" << endl;
    exit(EXIT_FAILURE); // TERMINATE
} // usage


//********************************************************************************************************************
// Function to calculate the Euclidean distance between two cities.
//********************************************************************************************************************
static double calculate_distance(City a, City b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}


//********************************************************************************************************************
// Main driver for the program.
//********************************************************************************************************************
int main(int argc, char *argv[]) {
    istream *infile = &cin;                     // default value
    switch (argc) {
        case 2:
            try {
                infile = new ifstream(argv[1]);
            } catch(...) {
                cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
                exit(EXIT_FAILURE);
            } // try
            // FALL THROUGH
        case 1:                                 // all defaults
            break;
        default:                                // wrong number of options
            usage(argv);
    } // switch

    string first_line; // to store the first line of the input, which contains the number of city
    *infile >> first_line; // read in
    int num_of_city = atoi(first_line.c_str()); // convert the number of citt string to integer

    vector<City> cities; // to store all cities
    map<pair<string, string>, double> distance_table; // a table to store the distances between any two cities

    for (int i = 0; i < num_of_city; i++) { // read in all cities and store in the 
        string city_name;
        int city_x, city_y;
        *infile >> city_name;
        *infile >> city_x;
        *infile >> city_y;
        City my_city (city_name, city_x, city_y);
        cities.push_back(my_city); // added to cities list
    } // for

    for (int i = 0; i < num_of_city; i++) {
        for (int j = 0; j < num_of_city; j++) {
            double dis = calculate_distance(cities[i], cities[j]); // calculate the distance of every two cities
            distance_table.insert(make_pair(make_pair(cities[i].name, cities[j].name), dis)); // added to table
        } // for
    } // for

    srand(getpid()); // pid as random number seed

    // Create an simulated annealing object
    Simulated_Annealing tsp(num_of_city, cities, distance_table); // calling the tsp

    tsp.annealing(); // perform the simulated annealing

    tsp.print_path(); // print the path and the cost of the path

    if (infile != &cin) delete infile;          // close file, do not delete cin!
} // main

