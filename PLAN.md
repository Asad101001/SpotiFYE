# Smart Music Manager & Recommendation Engine

**Project Overview:** A Data Structures and Algorithms (DSA) focused music manager that prioritizes discovering songs, organizing playlists, generating recommendations, and visualizing music relationships, moving beyond a simple playback tool.

## 1. Data Generation & Metadata

To build a meaningful recommendation graph without a massive dataset or neural networks, we will use a synthetic metadata approach.

*   **Target Library Size:** 50 - 100 songs (Requires ~250MB - 500MB storage).
*   **Metadata Structure (`Song` object):**
    *   `title` (string)
    *   `artist` (string)
    *   `genre` (string)
    *   `duration` (int)
    *   `playCount` (int)
    *   `rating` (int)
    *   `filePath` (string) - *Path to MP3*
    *   `coverArtPath` (string) - *Path to local image (e.g., `covers/song1.jpg`)*
*   **Recommendation Algorithm (Synthetic Similarity):**
    *   Same genre: +5 similarity
    *   Same artist: +10 similarity
    *   Duration difference < 30 sec: +2 similarity
    *   User rating close: +3 similarity

## 2. Architecture & File Structure

Instead of building data structures from scratch, we will **reuse logic from your `DSA-3rdSemester-Labs` repository**. We will directly import or minimally adapt your existing implementations (e.g., `Stack` from Week 4, `LinkedList` from Week 3, `Heap` from Week 12, etc.). This ensures the codebase is authentic to your coursework and keeps the implementation straightforward.

```text
dsaProject/
├── covers/              # Album art images
├── songs/               # MP3 files
├── src/
│   ├── Song.h / .cpp
│   ├── Playlist.h / .cpp       (Reusing Week 3 Linked Lists)
│   ├── QueueManager.h / .cpp   (Reusing Week 4 Queue)
│   ├── HistoryStack.h / .cpp   (Reusing Week 4 Stack)
│   ├── HeapManager.h / .cpp    (Reusing Week 12 Heaps)
│   ├── AVLTree.h / .cpp        (Adapted from Week 7/8 BST)
│   ├── Graph.h / .cpp          (Reusing Week 13 Graphs)
│   ├── Player.h / .cpp         (Playback logic)
│   ├── Renderer.h / .cpp       (Raylib UI logic)
│   └── main.cpp
└── PLAN.md
```

## 3. Development Phases & Milestones

### Phase 1: Core Music System (The Foundation)
*Focus: Basic playback and linear data structures.*
*   **Playback Controls:** Play, Pause, Stop, Next, Previous.
*   **Playlist Navigation:** `Doubly Linked List` (Previous <-> Current <-> Next).
*   **Repeat Mode:** `Circular Linked List` (A -> B -> C -> A).
*   **Up Next:** `Queue` (FIFO for queued songs).
*   **Playback History:** `Stack` (LIFO for recently played, allowing a "Back" button to `Pop()`).
*   **Most Played Songs:** `Heap` (Max-Heap to maintain top 10 songs).

### Phase 2: Library Management (Searching & Sorting)
*Focus: Efficient retrieval and ordering.*
*   **Searching:** `AVL Tree` (O(log n) lookups for searching by Song Title, Artist, or Album).
*   **Sorting:** Implement and visually demonstrate sorting algorithms operating on the library (Sort by Title, Duration, Rating, Play Count).
    *   `Quick Sort`
    *   `Merge Sort`
    *   `Heap Sort`

### Phase 3: Recommendation Graph (Discovery)
*Focus: Graph algorithms for song relationships.*
*   **Graph Construction:** Songs are nodes. Edges are weighted by the "Synthetic Similarity" score.
*   **Nearby Recommendations:** `BFS` (Find songs 1 or 2 degrees of separation away).
*   **Deep Music Discovery:** `DFS` (Follow a long chain of similar music).
*   **Most Similar Path:** `Dijkstra` (Find the optimal transition path between two very different songs).

### Phase 4: Minimum Recommendation Network (Advanced Graphs)
*Focus: Academic demonstration of spanning trees.*
*   **Prim & Kruskal:** Generate a "Minimum Recommendation Network" connecting all songs in the library with the minimum similarity cost. (Primarily for satisfying academic requirements for spanning trees).

## 4. UI / UX Vision ("Liquid Audio" / "Aura Music")

**Graphics Library Recommendation:** **Raylib**
Raylib is the perfect choice here. Since you already know how to download and include its header files, it requires zero complex setup. It is completely reliable, lightweight, and not buggy. 

**Achieving the "iOS 26 Liquid Glass" Premium Look:**
To achieve the frosted, glossy, opaque glass aesthetic, we won't rely on simple solid colors. Raylib natively supports **Custom GLSL Shaders**. We will write a simple "Fragment Shader" that takes the background behind the UI panel and applies a Fast Gaussian Blur and a slight white/black tint. 
This allows you to have a vibrant album cover in the background, and your UI elements (Playlist, Controls) will float on top as beautiful, translucent frosted glass cards with rounded corners.

```text
------------------------------------------------
| Library | Playlist | Algorithms | Statistics |
------------------------------------------------
[ Album Art ]

Now Playing: Blinding Lights
Play Count: 45
Genre: Pop
Rating: 4.7
------------------------------------------------
Up Next (Queue):
1. Song A
2. Song B
------------------------------------------------
History (Stack):
1. Song X
2. Song Y
------------------------------------------------
```
