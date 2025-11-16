#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <set>
#include <string>
#include "User.h"
#include "Queue.h"
#include "Stack.h"
#include "PriorityQueue.h"

class Graph {
private:
    // Adjacency List: userId -> vector of friend IDs (with weights)
    std::map<int, std::vector<std::pair<int, int>>> adjacencyList;
    
    // User data storage: userId -> User object
    std::map<int, User> users;
    
    int userIdCounter;
    
    // Helper function for DFS
    void DFSUtil(int userId, std::set<int>& visited, std::vector<int>& component);
    
    // Dijkstra's algorithm helper
    int minDistance(std::map<int, int>& dist, std::set<int>& sptSet);

public:
    Graph();
    
    // Graph operations
    int addUser(const std::string& name, const std::string& email, const std::string& profile = "");
    bool addFriendship(int userId1, int userId2, int weight = 1);
    bool removeFriendship(int userId1, int userId2);
    
    // Query operations
    std::vector<int> getFriends(int userId);
    User getUser(int userId);
    std::vector<User> getAllUsers();
    
    // Graph Algorithms
    
    // BFS - Shortest path (unweighted)
    std::vector<int> BFSShortestPath(int start, int end);
    
    // DFS - Find connected components
    std::vector<std::vector<int>> findConnectedComponents();
    
    // Dijkstra's Algorithm - Shortest path (weighted)
    std::pair<std::vector<int>, int> dijkstraShortestPath(int start, int end);
    
    // Mutual friends
    std::vector<int> findMutualFriends(int userId1, int userId2);
    
    // Friend suggestions (based on mutual connections)
    std::vector<std::pair<int, int>> suggestFriends(int userId, int limit = 5);
    
    // Centrality measures
    int getDegreeCentrality(int userId);
    std::vector<std::pair<int, int>> getMostConnectedUsers(int limit = 10);
    
    // Graph statistics
    struct Statistics {
        int totalUsers;
        int totalEdges;
        double averageConnections;
        int connectedComponents;
    };
    Statistics getStatistics();
    
    // Graph data for visualization
    struct GraphData {
        std::vector<std::pair<int, std::string>> nodes; // userId, name
        std::vector<std::pair<int, int>> edges; // from, to
    };
    GraphData getGraphData();
    
    // Check if path exists
    bool hasPath(int start, int end);
};

#endif
