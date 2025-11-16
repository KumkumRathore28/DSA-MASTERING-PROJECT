const API_BASE = 'http://localhost:8080';

let network = null;
let nodes = null;
let edges = null;

// Load statistics
async function loadStatistics() {
    try {
        const response = await fetch(`${API_BASE}/api/statistics`);
        const stats = await response.json();
        
        document.getElementById('totalUsers').textContent = stats.totalUsers || 0;
        document.getElementById('totalEdges').textContent = stats.totalEdges || 0;
        document.getElementById('avgConnections').textContent = 
            parseFloat(stats.averageConnections || 0).toFixed(2);
        document.getElementById('components').textContent = stats.connectedComponents || 0;
    } catch (error) {
        console.error('Error loading statistics:', error);
        document.getElementById('totalUsers').textContent = 'Error';
        document.getElementById('totalEdges').textContent = 'Error';
        document.getElementById('avgConnections').textContent = 'Error';
        document.getElementById('components').textContent = 'Error';
    }
}

// Load all users
async function loadUsers() {
    try {
        const response = await fetch(`${API_BASE}/api/users`);
        const users = await response.json();
        
        const usersList = document.getElementById('usersList');
        usersList.innerHTML = '';
        
        if (users.length === 0) {
            usersList.innerHTML = '<p>No users found. Add a user to get started!</p>';
            return;
        }
        
        users.forEach(user => {
            const userCard = document.createElement('div');
            userCard.className = 'user-card';
            userCard.innerHTML = `
                <h3>${user.name || 'User'}</h3>
                <p>ID: ${user.id} | Email: ${user.email || 'N/A'}</p>
                <p>Friends: ${user.friendsCount || 0} | Profile: ${user.profile || 'N/A'}</p>
            `;
            usersList.appendChild(userCard);
        });
    } catch (error) {
        console.error('Error loading users:', error);
        document.getElementById('usersList').innerHTML = 
            '<p style="color: red;">Error loading users. Make sure the server is running!</p>';
    }
}

// Add friendship
async function addFriendship() {
    const userId1 = parseInt(document.getElementById('friendUserId1').value);
    const userId2 = parseInt(document.getElementById('friendUserId2').value);
    
    if (!userId1 || !userId2) {
        alert('Please enter both user IDs');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/api/friendships`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                userId1: userId1,
                userId2: userId2
            })
        });
        
        const result = await response.json();
        
        if (response.ok) {
            alert('Friendship added successfully!');
            document.getElementById('friendUserId1').value = '';
            document.getElementById('friendUserId2').value = '';
            loadUsers();
            loadStatistics();
            visualizeGraph();
        } else {
            alert('Error: ' + (result.error || 'Failed to add friendship'));
        }
    } catch (error) {
        console.error('Error adding friendship:', error);
        alert('Error connecting to server. Make sure it is running!');
    }
}

// Find path using BFS
async function findPath() {
    const userId1 = parseInt(document.getElementById('pathUserId1').value);
    const userId2 = parseInt(document.getElementById('pathUserId2').value);
    
    if (!userId1 || !userId2) {
        alert('Please enter both user IDs');
        return;
    }
    
    try {
        const response = await fetch(`${API_BASE}/api/path/${userId1}/${userId2}`);
        const result = await response.json();
        
        const pathResult = document.getElementById('pathResult');
        
        if (result.distance === -1 || !result.path || result.path.length === 0) {
            pathResult.innerHTML = '<p style="color: red;">No path found between these users.</p>';
        } else {
            pathResult.innerHTML = `
                <p><strong>Path found using BFS!</strong></p>
                <p>Distance: ${result.distance} connections</p>
                <p>Path: ${result.path.join(' → ')}</p>
            `;
            
            // Highlight path in graph
            highlightPath(result.path);
        }
    } catch (error) {
        console.error('Error finding path:', error);
        document.getElementById('pathResult').innerHTML = 
            '<p style="color: red;">Error finding path. Make sure the server is running!</p>';
    }
}

// Visualize graph
async function visualizeGraph() {
    try {
        const response = await fetch(`${API_BASE}/api/graph`);
        const data = await response.json();
        
        const container = document.getElementById('networkGraph');
        
        nodes = new vis.DataSet(data.nodes.map(node => ({
            id: node.id,
            label: node.label || `User ${node.id}`,
            color: {
                background: '#667eea',
                border: '#4c63d2',
                highlight: {
                    background: '#764ba2',
                    border: '#5a3d7a'
                }
            },
            font: {
                size: 16,
                color: '#333'
            },
            shape: 'circle'
        })));
        
        edges = new vis.DataSet(data.edges.map(edge => ({
            from: edge.from,
            to: edge.to,
            color: {
                color: '#b4b4b4',
                highlight: '#667eea'
            },
            width: 2
        })));
        
        const graphData = { nodes, edges };
        const options = {
            nodes: {
                size: 30,
                font: {
                    size: 14
                }
            },
            edges: {
                smooth: {
                    type: 'continuous'
                }
            },
            physics: {
                enabled: true,
                stabilization: true
            },
            interaction: {
                hover: true,
                tooltipDelay: 200
            }
        };
        
        if (network) {
            network.setData(graphData);
        } else {
            network = new vis.Network(container, graphData, options);
        }
    } catch (error) {
        console.error('Error loading graph:', error);
        document.getElementById('networkGraph').innerHTML = 
            '<p style="color: red; padding: 20px;">Error loading graph. Make sure the server is running!</p>';
    }
}

// Highlight path in graph
function highlightPath(path) {
    if (!network || !edges) return;
    
    // Reset all edges
    edges.forEach(edge => {
        edges.update({
            id: edge.id,
            color: { color: '#b4b4b4' },
            width: 2
        });
    });
    
    // Highlight path edges
    for (let i = 0; i < path.length - 1; i++) {
        edges.forEach(edge => {
            if ((edge.from === path[i] && edge.to === path[i + 1]) ||
                (edge.from === path[i + 1] && edge.to === path[i])) {
                edges.update({
                    id: edge.id,
                    color: { color: '#10b981' },
                    width: 4
                });
            }
        });
    }
    
    // Highlight nodes
    nodes.forEach(node => {
        const isInPath = path.includes(node.id);
        nodes.update({
            id: node.id,
            color: {
                background: isInPath ? '#10b981' : '#667eea',
                border: isInPath ? '#059669' : '#4c63d2'
            }
        });
    });
}

// Show add user form
function showAddUserForm() {
    document.getElementById('addUserModal').classList.add('show');
}

// Close modal
function closeModal() {
    document.getElementById('addUserModal').classList.remove('show');
}

// Create user
async function createUser(event) {
    event.preventDefault();
    
    const name = document.getElementById('userName').value;
    const email = document.getElementById('userEmail').value;
    const profile = document.getElementById('userProfile').value;
    
    try {
        const response = await fetch(`${API_BASE}/api/users`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                name: name,
                email: email,
                profile: profile
            })
        });
        
        if (response.ok) {
            alert('User created successfully!');
            closeModal();
            document.getElementById('userName').value = '';
            document.getElementById('userEmail').value = '';
            document.getElementById('userProfile').value = '';
            loadUsers();
            loadStatistics();
            visualizeGraph();
        } else {
            const result = await response.json();
            alert('Error: ' + (result.error || 'Failed to create user'));
        }
    } catch (error) {
        console.error('Error creating user:', error);
        alert('Error connecting to server. Make sure it is running!');
    }
}

// Initialize on load
window.onload = function() {
    loadStatistics();
    loadUsers();
    visualizeGraph();
    
    // Refresh statistics every 5 seconds
    setInterval(loadStatistics, 5000);
};

// Close modal when clicking outside
window.onclick = function(event) {
    const modal = document.getElementById('addUserModal');
    if (event.target === modal) {
        closeModal();
    }
};
