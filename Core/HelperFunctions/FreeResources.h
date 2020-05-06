//
// Created by Adina Neda on 22/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_FREERESOURCES_H
#define PART2_PROJCODE_FREERESOURCES_H

#include <iostream>
#include "../EventQueue/EventQueue.h"
#include "../ManageAvailableResources/ResourceQueues.h"


using namespace std;

class FreeResources {
public:
    /**
     * Task finished
     * So the event is popped from the Event Queue
     * Resource is freed and added back to the Resource Queue
     * @param event_queue - queue of Events
     * @param resource_queues - queue of free available resources
     */
    static void free_resources_from_eventqueue(EventQueue *event_queue, ResourceQueues &resource_queues);
};


#endif //PART2_PROJCODE_FREERESOURCES_H
