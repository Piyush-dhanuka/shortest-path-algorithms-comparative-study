# shortest-path-algorithms-comparative-study

## Overview  
This project presents a **comparative study of three shortest path algorithms** — **Dijkstra’s**, **Bellman-Ford**, and **Floyd-Warshall** — implemented in C.  
The study focuses on their **design, performance analysis, and computational complexity**. 

Developed as part of the **Design and Analysis of Algorithms (DAA) Laboratory Project**, the implementation provides a hands-on understanding of how different algorithms perform on the same graph under identical conditions.

---

## Objectives  
- Implement and analyze multiple shortest path algorithms.  
- Measure and compare their execution time.  
- Evaluate the effect of edge weights (positive and negative) on algorithm performance.  
- Summarize time complexity and best-use scenarios.  

---

## Features  
- **Graph Representation:** Directed weighted graph using an adjacency matrix and edge list.  
- **Implemented Algorithms:**  
  1. **Dijkstra’s Algorithm** – Optimized for graphs with non-negative edge weights.  
  2. **Bellman-Ford Algorithm** – Detects negative weight cycles.  
  3. **Floyd-Warshall Algorithm** – Computes shortest paths between all node pairs.  
- **Path Reconstruction:** Displays full shortest path from source to destination.  
- **Performance Measurement:** Execution time recorded in milliseconds for comparison.  
- **Final Summary Table:** Displays complexity, timing, and best-case scenarios.  

---

## Algorithms Summary

| Algorithm      | Time Complexity | Handles Negative Weights  | Finds All-Pairs Paths |Best Use Case                                  |
|----------------|-----------------|---------------------------|-----------------------|-----------------------------------------------|
| Dijkstra       | O(V²)           | No                        |No                     | Fastest for graphs with only positive weights |
| Bellman-Ford   | O(V × E)        | Yes                       |No                     | Graphs containing negative edge weights       |
| Floyd-Warshall | O(V³)           | Yes                       |Yes                    | When all-pairs shortest paths are required    |

---

## How to Run

### 1. Clone the Repository
git clone https://github.com/Piyush-dhanuka/shortest-path-algorithms-comparative-study.git
cd shortest-path-algorithms-comparative-study

### 2. Compile the Code
gcc shortest_path.c -o shortest_path

### 3. Run the Program
./shortest_path
[sample-ouput.txt](https://github.com/user-attachments/files/23216217/sample-ouput.txt)
