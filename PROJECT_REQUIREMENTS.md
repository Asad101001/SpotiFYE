# Project Requirements & Architectural Overview

This document outlines the core requirements, technical systems, and design paradigms established during development. It focuses on the underlying logic, data structures, and UI systems rather than domain-specific implementation, making it highly reusable for future mass architectural shifts.

## 1. Architectural Philosophy
*   **Strictly Procedural (Non-OOP):** The codebase strictly avoids C++ Object-Oriented paradigms. It relies on structs, raw pointers, standard arrays, and procedural functions rather than classes or standard template libraries (like `std::vector`).
*   **Balanced Modularization:** The codebase is split into logical headers (main loop, UI definitions, rendering logic, and discrete files for each data structure) to avoid a monolithic structure without over-engineering the architecture.

## 2. Core Data Structures (DSA)
*   **Circular Doubly Linked List:** Implements robust sequential navigation (next/previous traversal) with O(1) tail insertion. Includes toggleable circular logic (wrap-around at the ends).
*   **Max-Heap (Priority Queue):** A frequency-based dynamic ranking system. It dynamically rebuilds and serves the top *N* items based on an incrementing integer counter without destroying the underlying data.
*   **Stack (LIFO):** Used to track user history or sequential states, allowing for chronological tracing of recent activity or state rollbacks.

## 3. UI / UX & Visual Aesthetics
*   **Theme - "AMOLED Liquid Glass":** Utilizes a pure pitch-black background (`#000000`) paired with highly vibrant, high-contrast accents (e.g., hot-pink `#DC1E64`). 
*   **Materials & Effects:** Features frosted/semi-transparent UI panels with hover states, giving a premium "liquid glass" feel. 
*   **Tactile Interactions:** Custom visual feedback systems, including expanding ripple animations triggered on mouse clicks and smooth lerping (linear interpolation) for dynamic visual elements.
*   **Typography:** Dual-weight, highly readable modern sans-serif typography (Poppins Bold & Regular). Established a strong visual hierarchy with large, punchy header text and subtle, smaller secondary details.
*   **Responsiveness:** Dynamic layout calculations performed per-frame. The UI seamlessly adapts to any monitor resolution and operates smoothly in a borderless full-screen environment.

## 4. Build System & Packaging
*   **Custom Compilation Script:** A streamlined `.bat` script utilizing `g++` (MinGW) to compile the project into a standalone Windows executable.
*   **Static Linking:** Critical runtime libraries (`libgcc`, `libstdc++`, etc.) are statically linked to ensure the resulting `.exe` runs out-of-the-box on any Windows machine without missing DLL errors.
*   **Embedded Binary Resources:** Application icons and metadata are compiled directly into the binary using a Windows Resource script (`.rc`) and `windres`.
*   **Resilient Pathing:** Intelligent directory resolution ensures the executable can always locate its external `assets/` folder, regardless of whether it is launched via terminal, double-click, or a desktop shortcut.

## 5. Repository & Version Control
*   **Clean Branching:** Standardized on `main` as the primary/default branch.
*   **Ignored Artifacts:** A strict `.gitignore` prevents compilation outputs (`.exe`, `.res`), OS files, and local scratchpads from polluting the repository.
