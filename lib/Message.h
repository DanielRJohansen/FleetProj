#include <string>

using std::string;

struct AgentMessage {
	AgentMessage(int sender_id, string body) : sender_id(sender_id), body(body) {}	// Used by Agent only
	AgentMessage(string message) { unpackMessage(); } // Used by server only

	int sender_id{-1}; // id of sender agent
	string body{};
	
	//TODO: implement
	void unpackMessage() {};

	// Pack the message into a single block, including sender id
	//TODO: implement
	string data() {
		return body;	
	}

};

struct ServerMessage {
	ServerMessage(int target, string body) : target(target), body(body) {}
	const int target; // id of sender agent
	const string body;
};