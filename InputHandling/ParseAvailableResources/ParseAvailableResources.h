//
// Created by Adina Neda on 24/12/2019.
// Released under license: Apache-2.0 License
//

#ifndef TESTPROJ_PARSEAVAILABLERESOURCES_H
#define TESTPROJ_PARSEAVAILABLERESOURCES_H

#include "../../Core/ManageAvailableResources/ResourceQueues.h"
#include "../../Auxiliaries/SplitString.h"

class ParseAvailableResources {
public:
    /**
     * Parse available resources from file and add store them in the resource queue
     * @param resource_queues
     * @param file_path
     */
    static void read_available_resources(ResourceQueues &resource_queues, ifstream& file_path);
};


#endif //TESTPROJ_PARSEAVAILABLERESOURCES_H
