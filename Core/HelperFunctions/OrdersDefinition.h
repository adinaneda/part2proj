//
// Created by Adina Neda on 29/03/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_ORDERSDEFINITION_H
#define PART2_PROJCODE_ORDERSDEFINITION_H

/**
 * File defines some of the orders by which data structures used in the project sort their elements.
 */

/**
 * Sort by arrival time, then deadline
 */
struct MinPriorityStartTimeEndTimeOrder {
    bool operator()(const Task& lhs, const Task& rhs) {
        if(lhs.scheduled_time == rhs.scheduled_time){
            return lhs.deadline > rhs.deadline;
        }
        else return lhs.scheduled_time > rhs.scheduled_time;
    }
};

/**
 * Sort by deadline
 */
struct DeadlineQueueOrder {
    bool operator()(const Task& lhs, const Task& rhs) {
        return lhs.deadline > rhs.deadline;
    }
};

#endif //PART2_PROJCODE_ORDERSDEFINITION_H
