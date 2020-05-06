# **Project title**: Resource Allocation in an In-Network Computing Enabled Environment

Repository for the Part II Individual Project, University of Cambridge 

Project takes as input 2 input files: incoming tasks and available resources. 
Input files can be found in `./InputHandling/InputFiles/AvailableResources` and `./InputHandling/InputFiles/Tasks`.

The structure of a file defining incoming tasks must be of the form:

| Task ID  | Arrival Time | Number of Operation | Deadline | Memory Requested (MB) |
| --- | --- | --- | --- | --- |

Note: the deadline is defined as the maxmimum wait time before a scheduling decision must be made for a task.

The structure of a file defining available resources must be of the form:

| Resource Type Name | Number of Resources of that Type | Number of Operations/second | Available Memory (MB) | Power (W) |
| --- | --- | --- | --- | --- |

Instructions to run the basic functionality provided by the project:

1. `cd` into the main directory of the project (directory with main.cpp)
2. compile the project: `make all`
3. To pseudo-randomly generate a new input csv file:
    `./part2proj GenerateInput number_tasks max_arrival_time max_number_operations_requested max_wait_time_in_queue max_MB_memory_requested percentage_tasks_MB_memory_out_of_total`

    Example:
    `./part2proj GenerateInput 5 1 40 3 3200 0.3`

4. To run statistics on an input file:
    `./part2proj RunInputStats tasks_file_name_without_extension`

    Example:
    `./part2proj RunInputStats tasksfile1`

5. Run all scheduling policies and compute statistics about the scheduling ana output files:
    `./part2proj tasks_file_name_without_extension available_resources_file_name_without_extension priority`
    
    Example:
    `./part2proj SchedulingAll tasksfile1 resources10thousandcombined SW`
    Priority can be `SW` or `CPU`.

6. Run only one scheduling policy:
    `./part2proj Scheduling scheduling_policy_name tasks_file_name_without_extension available_resources_file_name_without_extension priority`
   
   Example:
       `./part2proj Scheduling edf tasksfile1 resources10thousandcombined SW`
       Schduling policy can be: `fcfsqueue`, `offline`, `edf`
       Priority can be `SW` or `CPU`.

7. Run `make clean` to delete *.o and *.d files

