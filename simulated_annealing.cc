#include <math.h>
#include <climits>
#include "simulated_annealing.h"
using namespace std;


//********************************************************************************************************************
// Constructor for Simulated_Annealing to set up fields of class, including initial_state, distance lookup table,
//	initialize the vector which keep track of current path, the cost for the path, and set the initial temperature.
//********************************************************************************************************************
Simulated_Annealing::Simulated_Annealing(int total_nodes, vector<City> graph,
	map<pair<string, string>, double> table) : total_num_of_nodes(total_nodes) {
	initial_state = graph[0].name; // setting initial state
	distance_table = table; // setup the distance table for lookup

	if (graph.size() > 1) {
		for (int i = 0; i < graph.size(); i++) { // skip the initial state
			cur_path.push_back(graph[i].name); // add all other nodes to to_visit list
		} // for
	} // if
	cur_path.push_back(initial_state); // add the initial_state to the end of the path because of a cycle
	cur_cost = get_cost(cur_path); // calculating the cost
	cur_temperature = TEMPERATURE_INITIAL; // set the initial temperature
}


//********************************************************************************************************************
// Function to perform the actual annealing
// It updates the path and cost based on the acceptance probability, and updates the current temperature every time
//	in while loop. At the end, the temperature reaches the desired temperature and the path is more close (possibly)
//	the optimal solution.
//********************************************************************************************************************
void Simulated_Annealing::annealing() {
	const double TEMPERATURE_MIN = 0.0001; // minimum (desired) temperature to be annealing
	//const double annealing_rate = 0.5; // the annealing rate for each time (Linear)
	//const double annealing_rate = 0.999; // the annealing rate for each time (Exponential)
	const double annealing_rate = 0.00001; // the annealing rate for each time (Logarithmic)
	double aggregated_rate = exp(1); // aggregated annealing rate for logarithmic
	while (cur_temperature > TEMPERATURE_MIN) { // until reach the desired temperature
		vector<string> new_path = exchange_edge(); // get next new path
		double new_cost = get_cost(new_path); // calculate the new cost for the new path
		if (probability(new_cost) > (((double)rand())/RAND_MAX)) { // if it's acceptable
			cur_path = new_path; // update the current path
			cur_cost = new_cost; // update the current cost
		} // if
		//cout << cur_cost << endl;
		// ***************** Linear ***************************
		//cur_temperature -= annealing_rate;
		// ***************** Exponential **********************
		//cur_temperature *= annealing_rate; // lower temperature by multipling annealing rate (update temperature)
		// ***************** Logarithmic **********************
		aggregated_rate += annealing_rate;
		cur_temperature /= log(aggregated_rate);
	} // while
}


//********************************************************************************************************************
// Function to calculate the cost of a given path by looking up the distance table.
//********************************************************************************************************************
double Simulated_Annealing::get_cost(vector<string> path) {
	double new_cost = 0; // to store the return value
	for (int i = 0; i < path.size()-1; i++) {
		new_cost += distance_table[make_pair(path[i], path[i+1])]; // accumulating the cost of the path
	} // for
	return new_cost;	
}


//********************************************************************************************************************
// Function to calculate the acceptance probability.
// 1. If the cur_cost > new_cost, new_path is better, so probability > 1, always update the path;
// 2. The worse the new_cost, the lower the probability to update the path;
// 3. The lower the temperature, the lower the probability to chooes a "bad move".
//********************************************************************************************************************
double Simulated_Annealing::probability(double new_cost) {
	return exp((cur_cost - new_cost)/cur_temperature);
}


//********************************************************************************************************************
// Function to randomly swap two nodes and modify the local minimum path with the new path if the new cost is smaller.
//********************************************************************************************************************
void Simulated_Annealing::random_exchange_node(vector<string> &local_min_path, double &local_min_cost,
                                               int first_node_index, int second_node_index) {
	vector<string> new_path = cur_path; // copy the current path
	new_path[first_node_index] = cur_path[second_node_index]; // update the first random index node
	new_path[second_node_index] = cur_path[first_node_index]; // update the second random index node
	double new_cost = get_cost(new_path); // calculate the new cost for the new path
	if (new_cost < local_min_cost) { // smaller cost
		local_min_cost = new_cost; // update local_min_cost
		local_min_path = new_path; // update local_min_path
	} // if
}


//********************************************************************************************************************
// Function to randomly swap two edges and modify the local minimum path with the new path if the new cost is smaller.
//********************************************************************************************************************
void Simulated_Annealing::random_exchange_edge(vector<string> &local_min_path, double &local_min_cost,
	                                           int smaller_index, int larger_index) {
	vector<string> new_path = cur_path; // copy the current path
	for (int i = smaller_index; i < larger_index; i++) {
		// insert the nodes between smaller index and larger index in the reverse order
		new_path[i] = cur_path[larger_index-(i-smaller_index)-1];
	} // for
	double new_cost = get_cost(new_path); // calculate the new cost for the new path
	if (new_cost < local_min_cost) { // smaller cost
		local_min_path = new_path; // update local_min_cost
		local_min_cost = new_cost; // update local_min_path
	} // if
}


//********************************************************************************************************************
// Function to perform a local search and get next random (possibly "good") path.
//********************************************************************************************************************
vector<string> Simulated_Annealing::exchange_edge() {
	vector<string> local_min_path;
	double local_min_cost = INT_MAX;

	for (int i = 0; i < LOCAL_SEARCH_MAX; i++) {
		int first_node_index = 0; // for the first random node index
		int second_node_index = 0; // for the second random node index
		while (first_node_index == 0) { // until it's not the first one
			first_node_index = rand() % total_num_of_nodes; // random an index
		} // while
		// until it's not the first one and it's not the same as first_node_index
		while (second_node_index == 0 || second_node_index == first_node_index) {
			second_node_index = rand() % total_num_of_nodes; // random an index
		} // while

		int smaller_index, larger_index; // to get the smaller index and larger index for random_exchange_edge to use
		if (first_node_index > second_node_index) {
			larger_index = first_node_index;
			smaller_index = second_node_index;
		} else {
			larger_index = second_node_index;
			smaller_index = first_node_index;
		} // if

		// perform two local search operators
		random_exchange_node(local_min_path, local_min_cost, first_node_index, second_node_index);
		random_exchange_edge(local_min_path, local_min_cost, smaller_index, larger_index);
	} // for
	return local_min_path;
}


//********************************************************************************************************************
// Function to print out the stats, including the final path and the total cost.
//********************************************************************************************************************
void Simulated_Annealing::print_path() {
	cout << "TSP path: ";
	for (int i = 0; i < cur_path.size(); i++) { // output each node in the path
		cout << cur_path[i];
		if (i != cur_path.size()-1) cout << " -> ";
	} // for
	cout << endl; // new line
	cout << "Total cost: " << cur_cost << endl; // output the total cost of the path
}

