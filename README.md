<div align="center">
  <img src="assets/icon.ico" alt="SpotiFYE Logo" width="128" height="128"/>
  <h1>SpotiFYE Music Player</h1>
  <p><strong>A high-fidelity, liquid-glass AMOLED music player built with pure procedural C++, Raylib, and custom Data Structures & Algorithms.</strong></p>

  <p>
    <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++"/>
    <img src="https://img.shields.io/badge/Raylib-000000?style=for-the-badge&logo=raylib&logoColor=white" alt="Raylib"/>
    <img src="https://img.shields.io/badge/Architecture-Procedural_DSA-ff69b4?style=for-the-badge" alt="Procedural DSA"/>
    <img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows"/>
  </p>
</div>

<br/>

---

## 🎬 Video Showcase & Demo

Experience **SpotiFYE** in action — featuring 60 FPS liquid glass animations, real-time equalizer visualizations, interactive track seeking, and custom DSA-powered navigation:

<div align="center">
  <video src="https://github.com/user-attachments/assets/93e9097a-57db-4f42-92da-1158e24487d0" controls="controls" style="max-width: 850px; width: 100%; border-radius: 12px; box-shadow: 0px 8px 30px rgba(220, 30, 95, 0.4);"></video>
</div>

---

## 🎵 Overview

**SpotiFYE** is a modern, standalone audio application engineered without Object-Oriented Programming (OOP) paradigms. It leverages pure procedural C++ alongside custom Data Structures and Algorithms (DSA) to deliver an ultra-fast, lightweight, and visually stunning music listening experience.

Wrapped in a **Cyber-Brutalist, Liquid Glass AMOLED** aesthetic, SpotiFYE pairs hot-pink neon accents (`#DC1E64`) with pitch-black surfaces (`#000000`) to create a premium interface. Dynamic equalizer animations, interactive click ripples, smooth track seeking, and pulsating glowing borders are natively rendered at 60 FPS using Raylib.

---

## 🧠 Core Architecture & Data Structures (DSA)

Unlike conventional media applications that rely on high-level wrappers or standard containers, SpotiFYE implements low-level custom data structures engineered specifically for audio playback management:

| Data Structure | Implementation File | Purpose & Algorithm | Time Complexity |
| :--- | :--- | :--- | :--- |
| **Doubly Circular Linked List** | [`src/Playlist.h`](src/Playlist.h) | Sequential bidirectional song traversal with head/tail pointers. Supports $O(1)$ tail insertions for loading large libraries and toggleable circular wrap-around looping. | Insertion: $\mathcal{O}(1)$<br/>Traversal: $\mathcal{O}(1)$ |
| **Max-Heap (Priority Queue)** | [`src/MostPlayed.h`](src/MostPlayed.h) | Frequency-based dynamic ranking system ordering songs by session play count. Uses `upHeapify` and `downHeapify` algorithms with a non-destructive top-$N$ extraction engine. | Rebuild: $\mathcal{O}(N)$<br/>Top-$K$ Extract: $\mathcal{O}(K \log N)$ |
| **Stack (LIFO)** | [`src/History.h`](src/History.h) | Chronological playback history tracker recording recently played tracks. Dynamic pointer allocation capped at a strict size limit to prevent unbounded memory growth. | Push: $\mathcal{O}(1)$<br/>Peek: $\mathcal{O}(1)$ |
| **Procedural Array & Map Cache** | [`src/Library.h`](src/Library.h) | Contiguous array storage for master library tracks combined with GPU texture caching for album cover art to prevent redundant disk reads. | Indexed Access: $\mathcal{O}(1)$ |

---

## ✨ Features & Design Highlights

- 🖤 **AMOLED Liquid Glass Aesthetics:** Deep pitch-black base (`#000000`) with semi-transparent frosted glass cards, dynamic linear interpolation (lerp) glows, and hot-pink highlights.
- ⚡ **Pure Procedural Architecture:** 0% OOP classes — built strictly using C structs, procedural function pipelines, raw memory operations, and modular header separation.
- 📊 **Real-Time Visualizer:** Multi-bar dynamic audio visualizer rendered live via sine-wave harmonic synthesis.
- 🖱️ **Tactile Interactive Ripple Engine:** Dynamic radial ripple expansion on button clicks and playlist track selections.
- 🔍 **Integrated Cover Art Engine:** Automatic image scaling and trilinear texture filtering for album artwork using Poppins typography.
- 🔊 **Hardware Accelerated Audio:** Raylib-backed audio streaming supporting volume control, instant mute toggling, and interactive progress bar seeking.
- 🖥️ **Borderless Fullscreen:** Scales dynamically to any display resolution while handling keyboard shortcuts seamlessly.

---

## ⌨️ Controls & Keyboard Shortcuts

| Shortcut | Action |
| :--- | :--- |
| <kbd>Space</kbd> | Play / Pause Audio |
| <kbd>→</kbd> Right Arrow | Next Track |
| <kbd>←</kbd> Left Arrow | Previous Track |
| <kbd>R</kbd> | Toggle Playlist Circular Repeat Mode |
| <kbd>F11</kbd> | Toggle Borderless Fullscreen |
| <kbd>Left Mouse Click</kbd> | Select Track / Seek Timeline / Adjust Volume / Click Buttons |

---

## 🚀 Getting Started & Build Instructions

### Prerequisites
- **Operating System:** Windows 10 / 11 (64-bit)
- **Compiler:** MinGW-w64 (`g++` with C++17/20 support added to system PATH)

### Quick Start (One-Click Build)

1. **Clone the repository:**
   ```cmd
   git clone https://github.com/Asad101001/SpotiFYE.git
   cd SpotiFYE
   ```

2. **Run the automated build script:**
   ```cmd
   build.bat
   ```
   *The script automatically compiles embedded Windows icons via `windres`, statically links Raylib and MinGW runtime libraries, and launches `build/SpotiFYE.exe`.*

### Manual Compilation

If you prefer building directly from the command line:

```cmd
if not exist build mkdir build
windres assets\resource.rc -O coff -o assets\resource.res
g++ src\main.cpp assets\resource.res -o build\SpotiFYE.exe -I lib\include -L lib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -static-libgcc -static-libstdc++
```

---

## 📁 Project Structure

```text
SpotiFYE/
├── assets/
│   ├── demo.mp4               # Demo preview video
│   ├── preview.png            # High-resolution video preview poster
│   ├── icon.ico               # Windows application icon
│   ├── library.txt            # Track metadata store
│   ├── Poppins-Bold.ttf       # UI Header typography
│   ├── Poppins-Regular.ttf    # UI Body typography
│   ├── resource.rc            # Windows resource script
│   ├── covers/                # Album cover image assets
│   └── songs/                 # MP3 audio files
├── lib/
│   ├── include/               # Raylib header files
│   └── lib/                   # Raylib static library binaries
├── src/
│   ├── Song.h                 # Song data record definition
│   ├── Library.h              # Master song array & texture cache
│   ├── Playlist.h             # Circular Doubly Linked List implementation
│   ├── History.h              # LIFO Stack history tracking
│   ├── MostPlayed.h           # Max-Heap priority queue implementation
│   ├── ui.h                   # Color palette, font macros & ripple engine
│   ├── ui_draw.h              # UI layout, panel rendering & audio pipeline
│   └── main.cpp               # Application entry point & game loop
├── build.bat                  # Automated build & launch script
├── .gitignore                 # Git ignore rules
└── README.md                  # Main project documentation
```

---

<div align="center">
  <sub>Engineered with ❤️ by <strong>Asad</strong></sub>
</div>
