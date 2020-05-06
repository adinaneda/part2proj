//
// Created by Adina Neda on 24/12/2019.
//

#include "ParseAvailableResources.h"

/**
 * Parse available resources from file and add store them in the resource queue
 * @param resource_queues
 * @param file_path
 */
void ParseAvailableResources::read_available_resources(ResourceQueues &resource_queues, ifstream& file_path) {
    if (file_path.is_open()) {
        string str;
        while(!file_path.eof() && getline(file_path, str)) {
            // Find the first index character that's not an escape sequence
            string::size_type begin = str.find_first_not_of(" \f\t\v\r");
            //Skips blank lines and line starting with #
            if (begin == string::npos || string("#").find(str[begin]) != string::npos) continue;

            vector<string> tokens = SplitString::split(str, ",");
            string resource_type_name = tokens.at(0);
            int resource_count =  stoi(tokens.at(1));
            int operation_per_second = stoi(tokens.at(2));
            float memory = stof(tokens.at(3));
            float power = stof(tokens.at(4));

            // Push resource to the queue
            // Type of resource is handled by ResourceQueues class
            resource_queues.push_resource(resource_type_name, resource_count, operation_per_second, memory, power);
        }
    }else {
        cout << "Error opening the input type file in ParseAvailableResources! :( " << endl;
    }
}
