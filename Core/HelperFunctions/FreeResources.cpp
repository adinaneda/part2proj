//
// Created by Adina Neda on 22/03/2020.
// Released under license: Apache-2.0 License
//

#include "FreeResources.h"

/**
 * Task finished
 * So the event is popped from the Event Queue
 * Resource is freed and added back to the Resource Queue
 * @param event_queue - queue of Events
 * @param resource_queues - queue of free available resources
 */
void FreeResources::free_resources_from_eventqueue(EventQueue *event_queue, ResourceQueues &resource_queues) {
    string resource_id_top = event_queue->getResourceIDTop(); // of the form SW1_0, CPU1_1

    // we've finished a task, add the resource back to unallocated resources queue
    resource_queues.add_resource_to_unallocated(resource_id_top);

    // pop event from event queue
    event_queue->popEvent();
}

