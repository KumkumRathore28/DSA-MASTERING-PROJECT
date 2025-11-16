#include "Graph.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <sstream>

Graph::Graph() : userIdCounter(1) {}

int Graph::addUser(const std::string& name, const std::string& email, const std::string& profile) {
    int userId = userIdCounter++;
    User user(userId, name, email, profile);
    users[userId] = user;
    adjacencyList[userId] = std::vector<std::pair<int, int>>();
    return userId;
}

bool Graph::addFriendship(int userId1, int userId2, int weight) {
    if (userId1 == userId2 || users.find(userId1) == users.end() || users.find(userId2) == users.end()) {
        return false;
    }
    
    // Check if already friends
    for (const auto& pair : adjacencyList[userId1]) {
        if (pair.first == userId2) {
            return false; // Already friends
        }
    }
    
    adjacencyList[userId1].push_back({userId2, weight});
    adjacencyList[userId2].push_back({userId1, weight});
    return true;
}

bool Graph::removeFriendship(int userId1, int userId2) {
    auto it1 = adjacencyList.find(userId1);
    auto it2 = adjacencyList.find(userId2);
    
    if (it1 != adjacencyList.end()) {
        it1->second.erase(
            std::remove_if(it1->second.begin(), it1->second.end(),
                [userId2](const std::pair<int, int>& p) { return p.first == userId2; }),
            it1->second.end()
        );
    }
    
    if (it2 != adjacencyList.end()) {
        it2->second.erase(
            std::remove_if(it2->second.begin(), it2->second.end(),
                [userId1](const std::pair<int, int>& p) { return p.first == userId1; }),
            it2->second.end()
        );
    }
    
    return true;
}

std::vector<int> Graph::getFriends(int userId) {
    std::vector<int> friends;
    if (adjacencyList.find(userId) != adjacencyList.end()) {
        for (const auto& pair : adjacencyList[userId]) {
            friends.push_back(pair.first);
        }
    }
    return friends;
}

User Graph::getUser(int userId) {
    if (users.find(userId) != users.end()) {
        return users[userId];
    }
    return User(); // Return empty user
}

std::vector<User> Graph::getAllUsers() {
    std::vector<User> allUsers;
    for (const auto& pair : users) {
        allUsers.push_back(pair.second);
    }
    return allUsers;
}

// BFS - Breadth First Search for shortest path (unweighted graph)
std::vector<int> Graph::BFSShortestPath(int start, int end) {
    if (start == end) {
        return {start};
    }
    
    Queue<int> queue;
    std::map<int, int> parent;
    std::set<int> visited;
    
    queue.enqueue(start);
    visited.insert(start);
    parent[start] = -1;
    
    while (!queue.isEmpty()) {
        int current = queue.front();
        queue.dequeue();
        
        if (adjacencyList.find(current) != adjacencyList.end()) {
            for (const auto& neighbor : adjacencyList[current]) {
                int friendId = neighbor.first;
                
                if (friendId == end) {
                    // Reconstruct path
                    std::vector<int> path;
                    int node = end;
                    while (node != -1) {
                        path.push_back(node);
                        node = parent[node];
                    }
                    std::reverse(path.begin(), path.end());
                    return path;
                }
                
                if (visited.find(friendId) == visited.end()) {
                    visited.insert(friendId);
                    parent[friendId] = current;
                    queue.enqueue(friendId);
                }
            }
        }
    }
    
    return {}; // No path found
}

// DFS - Depth First Search for connected components
void Graph::DFSUtil(int userId, std::set<int>& visited, std::vector<int>& component) {
    visited.insert(userId);
    component.push_back(userId);
    
    if (adjacencyList.find(userId) != adjacencyList.end()) {
        for (const auto& neighbor : adjacencyList[userId]) {
            int friendId = neighbor.first;
            if (visited.find(friendId) == visited.end()) {
                DFSUtil(friendId, visited, component);
            }
        }
    }
}

std::vector<std::vector<int>> Graph::findConnectedComponents() {
    std::set<int> visited;
    std::vector<std::vector<int>> components;
    
    for (const auto& pair : users) {
        int userId = pair.first;
        if (visited.find(userId) == visited.end()) {
            std::vector<int> component;
            DFSUtil(userId, visited, component);
            components.push_back(component);
        }
    }
    
    return components;
}

// Dijkstra's Algorithm - Shortest path in weighted graph
int Graph::minDistance(std::map<int, int>& dist, std::set<int>& sptSet) {
    int min = INT_MAX;
    int minIndex = -1;
    
    for (const auto& pair : users) {
        int userId = pair.first;
        if (sptSet.find(userId) == sptSet.end() && dist[userId] <= min) {
            min = dist[userId];
            minIndex = userId;
        }
    }
    
    return minIndex;
}

std::pair<std::vector<int>, int> Graph::dijkstraShortestPath(int start, int end) {
    if (start == end) {
        return {{start}, 0};
    }
    
    std::map<int, int> dist;
    std::map<int, int> parent;
    std::set<int> sptSet;
    
    // Initialize distances to infinity
    for (const auto& pair : users) {
        dist[pair.first] = INT_MAX;
        parent[pair.first] = -1;
    }
    
    dist[start] = 0;
    
    for (size_t count = 0; count < users.size(); ++count) {
        int u = minDistance(dist, sptSet);
        
        if (u == -1) break;
        
        if (u == end) {
            // Reconstruct path
            std::vector<int> path;
            int node = end;
            while (node != -1) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return {path, dist[end]};
        }
        
        sptSet.insert(u);
        
        if (adjacencyList.find(u) != adjacencyList.end()) {
            for (const auto& neighbor : adjacencyList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;
                
                if (sptSet.find(v) == sptSet.end() && dist[u] != INT_MAX && 
                    dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }
    
    return {{}, -1}; // No path found
}

std::vector<int> Graph::findMutualFriends(int userId1, int userId2) {
    std::set<int> friends1(getFriends(userId1).begin(), getFriends(userId1).end());
    std::vector<int> mutual;
    
    for (int friendId : getFriends(userId2)) {
        if (friends1.find(friendId) != friends1.end()) {
            mutual.push_back(friendId);
        }
    }
    
    return mutual;
}

std::vector<std::pair<int, int>> Graph::suggestFriends(int userId, int limit) {
    std::set<int> currentFriends(getFriends(userId).begin(), getFriends(userId).end());
    std::map<int, int> suggestionCount; // friendId -> mutual friends count
    
    // For each friend, check their friends
    for (int friendId : getFriends(userId)) {
        for (int potentialFriend : getFriends(friendId)) {
            if (potentialFriend != userId && 
                currentFriends.find(potentialFriend) == currentFriends.end()) {
                suggestionCount[potentialFriend]++;
            }
        }
    }
    
    // Convert to vector and sort by count
    std::vector<std::pair<int, int>> suggestions;
    for (const auto& pair : suggestionCount) {
        suggestions.push_back({pair.first, pair.second});
    }
    
    std::sort(suggestions.begin(), suggestions.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });
    
    if ((int)suggestions.size() > limit) {
        suggestions.resize(limit);
    }
    
    return suggestions;
}

int Graph::getDegreeCentrality(int userId) {
    if (adjacencyList.find(userId) != adjacencyList.end()) {
        return (int)adjacencyList[userId].size();
    }
    return 0;
}

std::vector<std::pair<int, int>> Graph::getMostConnectedUsers(int limit) {
    std::vector<std::pair<int, int>> usersWithDegree;
    
    for (const auto& pair : users) {
        int degree = getDegreeCentrality(pair.first);
        usersWithDegree.push_back({pair.first, degree});
    }
    
    std::sort(usersWithDegree.begin(), usersWithDegree.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second;
        });
    
    if ((int)usersWithDegree.size() > limit) {
        usersWithDegree.resize(limit);
    }
    
    return usersWithDegree;
}

Graph::Statistics Graph::getStatistics() {
    Statistics stats;
    stats.totalUsers = (int)users.size();
    
    int totalConnections = 0;
    for (const auto& pair : adjacencyList) {
        totalConnections += (int)pair.second.size();
    }
    stats.totalEdges = totalConnections / 2; // Undirected graph
    
    stats.averageConnections = stats.totalUsers > 0 ? 
        (double)totalConnections / stats.totalUsers : 0.0;
    
    stats.connectedComponents = (int)findConnectedComponents().size();
    
    return stats;
}

Graph::GraphData Graph::getGraphData() {
    GraphData data;
    
    for (const auto& pair : users) {
        data.nodes.push_back({pair.first, pair.second.getName()});
    }
    
    std::set<std::string> addedEdges;
    for (const auto& pair : adjacencyList) {
        int from = pair.first;
        for (const auto& neighbor : pair.second) {
            int to = neighbor.first;
            std::string edgeKey = from < to ? 
                std::to_string(from) + "-" + std::to_string(to) :
                std::to_string(to) + "-" + std::to_string(from);
            
            if (addedEdges.find(edgeKey) == addedEdges.end()) {
                data.edges.push_back({from, to});
                addedEdges.insert(edgeKey);
            }
        }
    }
    
    return data;
}

bool Graph::hasPath(int start, int end) {
    std::vector<int> path = BFSShortestPath(start, end);
    return !path.empty();
}
