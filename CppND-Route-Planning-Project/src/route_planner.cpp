#include "route_planner.h"
#include <algorithm>
#include <iostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;


    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    this->start_node = &m_Model.FindClosestNode(start_x, start_y);
    this->end_node = &m_Model.FindClosestNode(end_x, end_y);

}

// TODO 3: Implement the CalculateHValue method.
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance( *(this->end_node) );
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
void RoutePlanner::AddNeighbors(RouteModel::Node* current_node) {
    // update current_node's neighbors vector
    current_node->RouteModel::Node::FindNeighbors();
    for (auto neighbor: current_node->neighbors) {
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        neighbor->h_value = this->CalculateHValue(neighbor);
        neighbor->visited = true;
        this->open_list.push_back(neighbor);
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
bool RoutePlanner::Compare(RouteModel::Node* node_a, RouteModel::Node* node_b){
    // list sorted with the lowest value at the end
    return node_a->g_value + node_a->h_value > node_b->g_value + node_b->h_value;
}


RouteModel::Node *RoutePlanner::NextNode() {
    // sort the open list with lowest sum at the end of the list
    std::sort(this->open_list.begin(), this->open_list.end(), RoutePlanner::Compare);
    auto next = this->open_list.back();
    this->open_list.pop_back();
    return next;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    RouteModel::Node *current_node_pointer = current_node;

    while(current_node_pointer){
        // add current node to the path
        path_found.push_back(*current_node_pointer);
        // calculate distance from the current node to its parent and add it to distance variable
        if (current_node_pointer->parent) this->distance += current_node_pointer->distance(*current_node_pointer->parent);
        // update pointer
        current_node_pointer = current_node_pointer->parent;
    }

    std::reverse(path_found.begin(), path_found.end());
    // Multiply the distance by the scale of the map to get meters.
    distance *= m_Model.MetricScale();
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = this->start_node;
    current_node->visited = true;
    RouteModel::Node *next_node = nullptr;

    //initialize the open list
    this->AddNeighbors(current_node);

    while(!this->open_list.empty()){
        if (current_node == this->end_node) {
            // Construct final path
            m_Model.path = ConstructFinalPath(current_node);
            break;
        }
        // Sort open list and return the next node.m_Model.path
        next_node = this->NextNode();
        current_node = next_node;
        // continue A* Search
        this->AddNeighbors(current_node);
    }
}

