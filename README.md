# Xonix Game
Xonix is a 2D arcade-style game developed using SFML in C++, featuring both single-player and multiplayer modes, immersive themes, and a social gaming experience.
## Features
### User Authentication: 
- Secure login/signup system using .txt files.
### Game Modes:
- Single Player with 3 progressive levels (increasing enemies & speed).
- Multiplayer with matchmaking via a priority queue.
### Social System:
- Send/accept/delete friend requests.
- View mutual friends and connect with them.
### Themes:
- Nature-inspired dynamic themes via AVL tree for a personalized feel.
### Leaderboard:
- Top 10 players ranked using a min-heap.
### Game Saving:
- Players can save and resume their single-player progress.
### Matchmaking:
- Matched players battle, with losing scores updated and winners only if they beat their previous high score.
# Data Structures Used:
|Data Structures| Usage |
|---| --- |
|File I/O| User authentication, scores, saved games, and friend request records.|
|Hash Table & Graphs| Friend system with mutual connections.|
|Stack| Managing power-ups.|
|Min-Heap| Leaderboard ranking.|
|Priority Queue| Multiplayer matchmaking.|
|AVL Tree| Efficient theme management.|
|Linked List| Saving and loading game states.|
# Getting Started
Since SFML was used for graphics and audio, the SFML library is required to run the game. [installing SFML for Visual Studio](https://www.youtube.com/watch?v=lFzpkvrscs4)
