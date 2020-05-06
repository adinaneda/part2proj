//
// Created by Adina Neda on 24/04/2020.
// Released under license: Apache-2.0 License
//

#ifndef PART2_PROJCODE_PATHS_H
#define PART2_PROJCODE_PATHS_H

#include <filesystem>
namespace fs = std::__fs::filesystem;
/**
 * For simplicity, we store the main directories of the project in a variable name
 * and then use it throughout the project
 */
const string core_project_dir = string(fs::current_path()) + "/Core/";
const string input_handling_dir = string(fs::current_path()) + "/InputHandling/";
const string output_handling_dir = string(fs::current_path()) + "/OutputHandling/";

const string available_resources_dir = input_handling_dir + "InputFiles/AvailableResources/";
const string tasks_dir = input_handling_dir + "InputFiles/Tasks/";

const string fcfs_output_dir = output_handling_dir + "OutputFiles/FCFSqueueSchedulingOutput/";
const string offline_output_dir = output_handling_dir + "OutputFiles/OfflineSchedulingOutput/";
const string edf_output_dir = output_handling_dir + "OutputFiles/EDFSchedulingOutput/";
const string eval_dir = output_handling_dir + "OutputFiles/EvaluationStatistics/";

#endif //PART2_PROJCODE_PATHS_H
