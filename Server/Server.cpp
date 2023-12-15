#include <httplib.h>
#include <iostream>
#include <unordered_map>


using namespace std;

class PublicServer {
private:
    httplib::Server server;
    std::unordered_map<std::string, std::string> agentCommands;

    int nextAgentId = 0;

    void setupEndpoints() {
        // Endpoint to register agents and send commands
        server.Post("/messageServer", [&](const httplib::Request& req, httplib::Response& res) {
            const int agentId = std::stoi(req.get_param_value("id"));
            std::string command = getCommandForAgent(agentId, "");


            res.set_content(command, "text/plain");
            //std::cout << "Sent command to agent " << agentId << ": " << command << std::endl;
            });

        // Endpoint to receive success notification from agents
        server.Post("/commandExecuted", [&](const httplib::Request& req, httplib::Response& res) {
            std::string agentId = req.get_param_value("id");
            std::string executionStatus = req.body;

            if (executionStatus == "success") {
                std::cout << "Agent " << agentId << " executed the command successfully." << std::endl;
                res.set_content("Acknowledged command execution.", "text/plain");
            }
            else {
                std::cout << "Agent " << agentId << " reported an error in command execution." << std::endl;
                res.set_content("Command execution error reported.", "text/plain");
            }
            });
    }

    std::string getCommandForAgent(int agentId, std::string agentMessage) {
        // In a real scenario, the command might depend on the agent's ID or state
        // For simplicity, we'll return a fixed command
        std::string command{};

        if (agentId == -1) {
            printf("Setting agent id to %d\n", nextAgentId);
            command = std::to_string(nextAgentId);
            nextAgentId++;
        }
        else {
            command = "Some random task";
        }

        return command;
    }

public:
    PublicServer() {
        // Server should first consult the database to iniate its state (but currently there is no state)
        //setupServerState();

        // Setup the httpserver for multithreading
		server.new_task_queue = []() { return new httplib::ThreadPool(8); };

        setupEndpoints();

        std::cout << "Starting API server on port 8080..." << std::endl;
        server.listen("localhost", 8080);
    }
};

int main() {

    // Put in try-catch block to automatically reset the server
    PublicServer apiServer{};

    return 0;
}