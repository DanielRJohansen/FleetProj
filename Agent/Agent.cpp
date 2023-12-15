#include <httplib.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "Message.h"

using Command = std::string;
using AgentStatus = std::string;
using Event = bool;

using namespace std;

class RemoteAgent {
private:
    std::string serverAddress;
    int serverPort;

    int agentId{-1};  // Assigned by server once the agent connects

    // Default communication tunnel
    Command messageServer(string messageBody) {
        httplib::Client cli(serverAddress.c_str(), serverPort);

        AgentMessage message(agentId, messageBody);

        const int maxTries = 3;

        for (int i = 0; i < maxTries; i++) {
            // Send a message to the server
            const auto result = cli.Post("/messageServer?id=" + to_string(agentId), message.data(), "text/plain");

            if (result && result->status == 200) {
                std::string command = result->body;
                std::cout << "Received command: " << command << std::endl;
                return command;
            }
            else {
                std::cout << "Failed to receive command from server " << std::to_string(i)<< "\n";
            }
        }
        
        // Throw that an error occured. Implement handling at some point
        //throw std::runtime_error("Failed to get response from server");

        exit(1);
    }

    void notifyServer(AgentStatus status) {
        httplib::Client cli(serverAddress.c_str(), serverPort);

        const auto result = cli.Post("/commandExecuted?id=" + to_string(agentId), status, "text/plain");
        // Dont care about result for now
    }

    // This function does not handle the case where something goes wrong in executing .. yet.
    AgentStatus executeCommand(Command cmd) {
        // Execute the command..
        AgentStatus status = "success";

        notifyServer(status);
        return status;
    }

    AgentStatus doSomething() {
        return "Something caught fire";
    }

    void run() {
        while (true) {

            AgentStatus status = doSomething();

            if (status != "") {
                // Notify server that something has happened, and wait for a command
                const auto command = messageServer(status);
                if (command != "nop") {
                    executeCommand(command);
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }

public:
    RemoteAgent(const std::string& serverAddr, int serverPrt)
        : serverAddress(serverAddr), serverPort(serverPrt)
    {
        printf("Starting agent {%d}..\n", agentId);
        // Notify the server that the agent is alive. The server will reply with a unique ID to the agent.
        // In a real life scenario this greeting should carry alot more information.
        const Command greeting = messageServer("AgentBootingUp");
        agentId = std::stoi(greeting);

        std::cout << "..Agent " + std::to_string(agentId) + " connected\n";
        
        // Simulate some more setup
        std::this_thread::sleep_for(std::chrono::seconds(2));


        run();
    }
};





int main() {
    RemoteAgent agent("localhost", 8080);
    return 0;
}