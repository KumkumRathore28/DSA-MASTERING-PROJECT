#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <map>
#include <regex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "Graph.h"
#include "HTTPParser.h"
#include "JSONParser.h"

class SocialNetworkServer {
private:
    Graph graph;
    int port;
    
    void initializeSampleData() {
        int alice = graph.addUser("Alice", "alice@example.com", "Software Engineer");
        int bob = graph.addUser("Bob", "bob@example.com", "Data Scientist");
        int charlie = graph.addUser("Charlie", "charlie@example.com", "Designer");
        int diana = graph.addUser("Diana", "diana@example.com", "Manager");
        int eve = graph.addUser("Eve", "eve@example.com", "Developer");
        int frank = graph.addUser("Frank", "frank@example.com", "Analyst");
        int grace = graph.addUser("Grace", "grace@example.com", "Product Manager");
        int henry = graph.addUser("Henry", "henry@example.com", "Consultant");
        
        graph.addFriendship(alice, bob, 1);
        graph.addFriendship(alice, charlie, 2);
        graph.addFriendship(bob, charlie, 1);
        graph.addFriendship(bob, diana, 1);
        graph.addFriendship(charlie, diana, 3);
        graph.addFriendship(diana, eve, 1);
        graph.addFriendship(eve, frank, 2);
        graph.addFriendship(frank, grace, 1);
        graph.addFriendship(grace, henry, 2);
        graph.addFriendship(henry, alice, 1);
    }
    
    std::pair<int, int> extractTwoIds(const std::string& path, const std::string& prefix) {
        std::string rest = path.substr(prefix.length());
        size_t slashPos = rest.find('/');
        if (slashPos == std::string::npos) {
            return {-1, -1};
        }
        int id1 = std::stoi(rest.substr(0, slashPos));
        int id2 = std::stoi(rest.substr(slashPos + 1));
        return {id1, id2};
    }
    
    std::string handleGetAllUsers() {
        std::vector<User> users = graph.getAllUsers();
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < users.size(); ++i) {
            if (i > 0) json << ",";
            std::vector<int> friendIds = graph.getFriends(users[i].getId());
            json << "{\"id\":" << users[i].getId() 
                 << ",\"name\":\"" << users[i].getName() 
                 << "\",\"email\":\"" << users[i].getEmail() 
                 << "\",\"profile\":\"" << users[i].getProfile() 
                 << "\",\"friendsCount\":" << friendIds.size() << "}";
        }
        json << "]";
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleGetUser(int userId) {
        User user = graph.getUser(userId);
        if (user.getId() == -1) {
            return HTTPParser::buildResponse(404, "{\"error\":\"User not found\"}");
        }
        
        std::vector<int> friendIds = graph.getFriends(userId);
        std::ostringstream json;
        json << "{\"id\":" << user.getId() 
             << ",\"name\":\"" << user.getName() 
             << ",\"email\":\"" << user.getEmail() 
             << "\",\"profile\":\"" << user.getProfile() 
             << "\",\"friends\":[";
        for (size_t i = 0; i < friendIds.size(); ++i) {
            if (i > 0) json << ",";
            json << friendIds[i];
        }
        json << "]}";
        
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleCreateUser(const std::string& body) {
        std::regex nameRegex("\"name\"\\s*:\\s*\"([^\"]+)\"");
        std::regex emailRegex("\"email\"\\s*:\\s*\"([^\"]+)\"");
        std::regex profileRegex("\"profile\"\\s*:\\s*\"([^\"]+)\"");
        
        std::smatch nameMatch, emailMatch, profileMatch;
        std::string name = "User", email = "user@example.com", profile = "";
        
        if (std::regex_search(body, nameMatch, nameRegex)) {
            name = nameMatch[1].str();
        }
        if (std::regex_search(body, emailMatch, emailRegex)) {
            email = emailMatch[1].str();
        }
        if (std::regex_search(body, profileMatch, profileRegex)) {
            profile = profileMatch[1].str();
        }
        
        int userId = graph.addUser(name, email, profile);
        User user = graph.getUser(userId);
        
        std::ostringstream json;
        json << "{\"id\":" << user.getId() 
             << ",\"name\":\"" << user.getName() 
             << "\",\"email\":\"" << user.getEmail() 
             << "\",\"profile\":\"" << user.getProfile() << "\"}";
        
        return HTTPParser::buildResponse(201, json.str());
    }
    
    std::string handleAddFriendship(const std::string& body) {
        std::regex id1Regex("\"userId1\"\\s*:\\s*(\\d+)");
        std::regex id2Regex("\"userId2\"\\s*:\\s*(\\d+)");
        
        std::smatch match1, match2;
        if (!std::regex_search(body, match1, id1Regex) || !std::regex_search(body, match2, id2Regex)) {
            return HTTPParser::buildResponse(400, "{\"error\":\"Invalid request\"}");
        }
        
        int userId1 = std::stoi(match1[1].str());
        int userId2 = std::stoi(match2[1].str());
        
        if (graph.addFriendship(userId1, userId2)) {
            return HTTPParser::buildResponse(200, "{\"message\":\"Friendship added\"}");
        } else {
            return HTTPParser::buildResponse(400, "{\"error\":\"Failed to add friendship\"}");
        }
    }
    
    std::string handleGetPath(int userId1, int userId2) {
        std::vector<int> path = graph.BFSShortestPath(userId1, userId2);
        
        if (path.empty()) {
            return HTTPParser::buildResponse(200, "{\"path\":[],\"distance\":-1}");
        }
        
        std::ostringstream json;
        json << "{\"path\":[";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i > 0) json << ",";
            json << path[i];
        }
        json << "],\"distance\":" << (path.size() - 1) << "}";
        
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleGetStatistics() {
        Graph::Statistics stats = graph.getStatistics();
        std::ostringstream json;
        json << "{\"totalUsers\":" << stats.totalUsers 
             << ",\"totalEdges\":" << stats.totalEdges 
             << ",\"averageConnections\":" << stats.averageConnections 
             << ",\"connectedComponents\":" << stats.connectedComponents << "}";
        
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleGetGraphData() {
        Graph::GraphData data = graph.getGraphData();
        
        std::ostringstream json;
        json << "{\"nodes\":[";
        for (size_t i = 0; i < data.nodes.size(); ++i) {
            if (i > 0) json << ",";
            json << "{\"id\":" << data.nodes[i].first 
                 << ",\"label\":\"" << data.nodes[i].second << "\"}";
        }
        json << "],\"edges\":[";
        for (size_t i = 0; i < data.edges.size(); ++i) {
            if (i > 0) json << ",";
            json << "{\"from\":" << data.edges[i].first 
                 << ",\"to\":" << data.edges[i].second << "}";
        }
        json << "]}";
        
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleGetComponents() {
        std::vector<std::vector<int>> components = graph.findConnectedComponents();
        
        std::ostringstream json;
        json << "[";
        for (size_t i = 0; i < components.size(); ++i) {
            if (i > 0) json << ",";
            json << "[";
            for (size_t j = 0; j < components[i].size(); ++j) {
                if (j > 0) json << ",";
                json << components[i][j];
            }
            json << "]";
        }
        json << "]";
        
        return HTTPParser::buildResponse(200, json.str());
    }
    
    std::string handleRequest(const std::string& rawRequest) {
        HTTPRequest req = HTTPParser::parse(rawRequest);
        std::string method = req.method;
        std::string path = req.path;
        
        // Remove query string
        size_t queryPos = path.find('?');
        if (queryPos != std::string::npos) {
            path = path.substr(0, queryPos);
        }
        
        // Route handling
        if (method == "GET") {
            if (path == "/api/users") {
                return handleGetAllUsers();
            } else if (path.find("/api/users/") == 0) {
                int userId = std::stoi(path.substr(11));
                return handleGetUser(userId);
            } else if (path.find("/api/path/") == 0) {
                std::pair<int, int> ids = extractTwoIds(path, "/api/path/");
                return handleGetPath(ids.first, ids.second);
            } else if (path == "/api/statistics") {
                return handleGetStatistics();
            } else if (path == "/api/graph") {
                return handleGetGraphData();
            } else if (path == "/api/components") {
                return handleGetComponents();
            }
        } else if (method == "POST") {
            if (path == "/api/users") {
                return handleCreateUser(req.body);
            } else if (path == "/api/friendships") {
                return handleAddFriendship(req.body);
            }
        }
        
        return HTTPParser::buildResponse(404, "{\"error\":\"Not found\"}");
    }
    
    void handleClient(int clientSocket) {
        char buffer[8192] = {0};
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead > 0) {
            std::string request(buffer, bytesRead);
            std::string response = handleRequest(request);
            send(clientSocket, response.c_str(), (int)response.length(), 0);
        }
        
        close(clientSocket);
    }

public:
    SocialNetworkServer(int port = 8080) : port(port) {
        initializeSampleData();
    }
    
    void start() {
        #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif
        
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            return;
        }
        
        int opt = 1;
        #ifdef _WIN32
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        #else
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        #endif
        
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error binding socket" << std::endl;
            return;
        }
        
        if (listen(serverSocket, 10) < 0) {
            std::cerr << "Error listening" << std::endl;
            return;
        }
        
        std::cout << "🚀 Social Network Graph System Server running on port " << port << std::endl;
        std::cout << "📊 Sample data initialized" << std::endl;
        std::cout << "🌐 Server ready at http://localhost:" << port << std::endl;
        
        while (true) {
            struct sockaddr_in clientAddr;
            socklen_t clientLen = sizeof(clientAddr);
            
            int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
            if (clientSocket < 0) {
                continue;
            }
            
            std::thread(&SocialNetworkServer::handleClient, this, clientSocket).detach();
        }
        
        close(serverSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
    }
};

int main() {
    SocialNetworkServer server(8080);
    server.start();
    return 0;
}
