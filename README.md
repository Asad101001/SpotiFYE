<div align="center">
  <img src="assets/icon.ico" alt="SpotiFYE Logo" width="120" height="120"/>
  <h1>SpotiFYE Music Player</h1>
  <p><strong>A high-fidelity, liquid-glass AMOLED music player powered by C++, Raylib, and custom Data Structures.</strong></p>

  [![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](#)
  [![Raylib](https://img.shields.io/badge/Raylib-000000?style=for-the-badge&logo=raylib&logoColor=white)](#)
  [![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)](#)
</div>

<br/>

## 🎵 Overview

**SpotiFYE** is a modern, standalone audio player engineered without Object-Oriented Programming (OOP) paradigms. It leverages pure procedural C++ alongside custom Data Structures and Algorithms (DSA) to deliver a blazing-fast, responsive, and visually stunning music listening experience.

Wrapped in a **Cyber-Brutalist, Liquid Glass AMOLED** aesthetic, SpotiFYE pairs hot-pink neon accents with deep black surfaces to create a premium interface. Features like dynamic equalizer animations, smooth click ripples, and pulsating active borders are natively rendered at 60 FPS using Raylib.

---

## 🧠 Core Architecture & DSA

Unlike typical wrapper applications, SpotiFYE relies on custom-built data structures to handle audio logic efficiently:

- **Doubly Circular Linked List (`Playlist.h`)**
  - Enables seamless scrolling, continuous looping (Repeat All), and O(1) tail insertions for loading large libraries.
- **Stack (`History.h`)**
  - A Last-In-First-Out (LIFO) model dynamically tracking your playback history to accurately recount recent songs.
- **Max-Heap (`MostPlayed.h`)**
  - An underlying priority queue ranking songs by their `playCount`. The heap dynamically rebuilds on play and uses a non-destructive $O(K \log N)$ extraction to generate the "Most Played" leaderboards in real time.

---

## ✨ Features

- **AMOLED Glass UI:** Deep black `#000000` base with `#DC1E64` hot-pink accents and frosted glass overlays.
- **Visual Feedback:** Responsive UI with expanding ripple click effects, continuous sine-wave glowing borders, and real-time equalizer bars.
- **Borderless Fullscreen:** Scales dynamically to your monitor's native resolution.
- **Audio Processing:** Hardware-accelerated audio streaming via Raylib (`.mp3` decoding, multi-channel support).
- **Procedural Engine:** Monolithic, state-driven procedural backend decoupled into modular headers.

---

## 🚀 Getting Started

### Prerequisites
- **Windows OS** (Targets Windows x64)
- **MinGW-w64** (Make sure `g++` is in your system PATH)

### Installation & Build

1. Clone the repository:
   ```cmd
   git clone https://github.com/Asad101001/SpotiFYE.git
   cd SpotiFYE
   ```

2. Compile the project:
   Simply run the automated build script. It handles resource compilation (for the `.exe` icon) and static linking of Raylib.
   ```cmd
   build.bat
   ```

3. Enjoy your music! SpotiFYE will launch automatically on a successful build.

---

## ⌨️ Controls

| Key | Action |
| --- | --- |
| `Space` | Play / Pause |
| `Right Arrow` | Next Song |
| `Left Arrow` | Previous Song |
| `R` | Toggle Playlist Repeat |
| `F11` | Toggle Fullscreen |

<br/>

<div align="center">
  <sub>Built with ❤️ by Asad</sub>
</div>
