/**
 * @file        CommandRouter.h
 * @title       Command Router Service
 * @description Defines the `CommandRouter` singleton, a core service for
 *              registering and executing text-based commands from any source
 *              (Serial, MQTT, etc.).
 *
 * @author      Giorgi Magradze
 * @date        2025-08-25
 * @version     0.3.0
 */
#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>

// Define a type for the command handler function.
// It accepts a vector of string arguments and returns a result string.
using CommandHandler = std::function<std::string(const std::vector<std::string>& args)>;

/**
 * @class CommandRouter
 * @brief A central service for routing text-based commands to modules.
 */
class CommandRouter {
public:
    /**
     * @brief Gets the singleton instance of the CommandRouter.
     * @return A reference to the CommandRouter instance.
     */
    static CommandRouter& getInstance();

    /**
     * @brief Registers a command handler for a specific module instance.
     * @details Modules should call this in their `registerCommands()` method.
     * @param instanceName The unique name of the module instance registering the command.
     * @param command The name of the command (e.g., "on", "set_temp").
     * @param handler The function to execute when the command is called.
     * @return True if registration was successful, false if the command is already registered for this instance.
     */
    bool registerCommand(const std::string& instanceName, const std::string& command, CommandHandler handler);

    /**
     * @brief Executes a command string.
     * @details This is the main entry point. It parses the string, finds the
     *          correct handler, and executes it.
     * @param commandString The full command string (e.g., "my_led on", "sensor get_value").
     * @return A string containing the result or an error message from the command.
     */
    std::string execute(const std::string& commandString);

private:
    CommandRouter() {} // Singleton
    
    // Internal structure to hold the registered command
    struct RegisteredCommand {
        std::string instanceName;
        std::string command;

        // Custom comparator for map key
        bool operator<(const RegisteredCommand& other) const {
            if (instanceName < other.instanceName) return true;
            if (instanceName > other.instanceName) return false;
            return command < other.command;
        }
    };
    
    // A map where the key is a combination of instance name and command,
    // and the value is the handler function.
    std::map<RegisteredCommand, CommandHandler> _commandRegistry;
};