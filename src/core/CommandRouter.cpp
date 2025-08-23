/**
 * @file        CommandRouter.cpp
 * @title       Command Router Implementation
 * @description Implements the logic for registering, parsing, and executing commands.
 */
#include "CommandRouter.h"
#include "Logger.h" // For logging
#include <sstream>   // For splitting the command string

CommandRouter& CommandRouter::getInstance() {
    static CommandRouter instance;
    return instance;
}

bool CommandRouter::registerCommand(const std::string& instanceName, const std::string& command, CommandHandler handler) {
    RegisteredCommand cmd = {instanceName, command};
    if (_commandRegistry.count(cmd)) {
        NEXTINO_CORE_LOG(LogLevel::Warn, "CmdRouter", "Command '%s' is already registered for instance '%s'. Overwriting.", command.c_str(), instanceName.c_str());
    }
    _commandRegistry[cmd] = handler;
    NEXTINO_CORE_LOG(LogLevel::Debug, "CmdRouter", "Registered command '%s' for instance '%s'.", command.c_str(), instanceName.c_str());
    return true;
}

std::string CommandRouter::execute(const std::string& commandString) {
    std::stringstream ss(commandString);
    std::string segment;
    std::vector<std::string> segments;
    while(std::getline(ss, segment, ' ')) {
       segments.push_back(segment);
    }

    if (segments.size() < 2) {
        return "ERROR: Invalid command format. Expected '<instance_name> <command> [args...]'.";
    }

    std::string instanceName = segments[0];
    std::string command = segments[1];
    std::vector<std::string> args(segments.begin() + 2, segments.end());

    RegisteredCommand cmdToFind = {instanceName, command};

    auto it = _commandRegistry.find(cmdToFind);
    if (it != _commandRegistry.end()) {
        // Command found, execute the handler
        NEXTINO_CORE_LOG(LogLevel::Info, "CmdRouter", "Executing command '%s' for instance '%s'", command.c_str(), instanceName.c_str());
        return it->second(args); // Call the stored lambda/function
    } else {
        NEXTINO_CORE_LOG(LogLevel::Warn, "CmdRouter", "Command '%s' not found for instance '%s'", command.c_str(), instanceName.c_str());
        return "ERROR: Command not found.";
    }
}