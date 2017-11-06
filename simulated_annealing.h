#include <string>
#include <stdlib.h>
#include <cstring>
#include "city.h"


//********************************************************************************************************************
// Class for Simulated Annealing
//********************************************************************************************************************
class Simulated_Annealing {
	enum { TEMPERATURE_INITIAL = 10000, LOCAL_SEARCH_MAX = 10 }; // enum for the initial temperature
	string initial_state; // to store the initial state
	const int total_num_of_nodes; // to store total number of nodes
	map<pair<std::string, std::string>, double> distance_table; // table has Euclidean distance b/w any pair of nodes
	vector<std::string> cur_path; // to store the current path
	double cur_cost; // to store the total cost of current path
	double cur_temperature; // to hold the current temperature
	double get_cost(vector<std::string> new_path); // function to get the cost of a path
	double probability(double new_cost); // function to return probability of acceptance based on the new cost
	void random_exchange_node(vector<string> &local_min_path, double &local_min_cost,
                              int first_node_index, int second_node_index); // randommly exchange two nodes
	void random_exchange_edge(vector<string> &local_min_path, double &local_min_cost,
	                          int smaller_index, int larger_index); // randomly exchange two edges
	vector<string> exchange_edge(); // function randomly swap two nodes and return the new path
public:
	Simulated_Annealing(int total_nodes, vector<City> graph, map<pair<string, string>, double> table); // constructor
	void annealing(); // perform the simulated annealing
	void print_path(); // print the path and cost of the path
};
