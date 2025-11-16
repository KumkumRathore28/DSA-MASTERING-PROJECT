# Social Network Graph System
## Full-Stack Project with C++ Backend - Complete Implementation

### Overview
A complete social network graph system built with:
- **C++ Backend**: RESTful API server with custom HTTP parsing
- **DSA Implementation**: Graphs, BFS, DFS, Dijkstra, Adjacency Lists, Priority Queue, Stack, Queue
- **Frontend**: Modern HTML/CSS/JavaScript with interactive graph visualization

### Features
1. **User Management**: Add, view users
2. **Friendship Management**: Connect/disconnect users
3. **Graph Algorithms**:
   - **BFS**: Find shortest path (degrees of separation)
   - **DFS**: Find connected components
   - **Dijkstra**: Shortest weighted path
4. **Visualization**: Interactive network graph
5. **Statistics**: Real-time network metrics

### DSA Concepts Implemented
- ✅ **Graph Data Structure** (Adjacency List)
- ✅ **Breadth-First Search (BFS)**
- ✅ **Depth-First Search (DFS)**
- ✅ **Dijkstra's Algorithm**
- ✅ **Priority Queue** (Min/Max Heap)
- ✅ **Stack** (Linked List implementation)
- ✅ **Queue** (Linked List implementation)
- ✅ **Map/Set** (for graph storage)

### Prerequisites
- C++17 compiler (g++, clang++, or MSVC)
- Make (optional, or use build.bat/CMake)
- Web browser (for frontend)

### Build Instructions

#### Windows:
build.bat#### Linux/Mac:
cd backend
make#### Manual Build:
cd backend
g++ -std=c++17 -pthread main.cpp Graph.cpp User.cpp HTTPParser.cpp JSONParser.cpp -o server -lws2_32
# Windows: add -lws2_32
# Linux/Mac: use -pthread instead### Running the Project

1. **Start the C++ Server**:
   - Windows: `run.bat` or `cd backend && server.exe`
   - Linux/Mac: `cd backend && ./server`

2. **Open Frontend**:
   - Open `frontend/index.html` in your browser
   - Or use a simple HTTP server:
     cd frontend
     python -m http.server 3000
     ### API Endpoints

- `GET /api/users` - Get all users
- `GET /api/users/:id` - Get specific user
- `POST /api/users` - Create new user
- `POST /api/friendships` - Add friendship
- `GET /api/path/:id1/:id2` - Find shortest path (BFS)
- `GET /api/statistics` - Get network statistics
- `GET /api/graph` - Get graph data for visualization
- `GET /api/components` - Get connected components (DFS)

### Default Ports
- Backend: `http://localhost:8080`
- Frontend: Open `frontend/index.html` directly

### Project Structure
