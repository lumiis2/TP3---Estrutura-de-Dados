# Logistical Problem Solving for BiUaiDi

This project aims to develop a solution for a logistical problem faced by the fictional company **BiUaiDi**, which is establishing itself in Belo Horizonte. The company requires an efficient system to manage its charging stations throughout the city. The objective is to identify and return the `k` nearest active charging points to a given coordinate (x, y), facilitating navigation to these stations.

Currently, the system employs a simple and inefficient approach that calculates the distance from (x, y) to all charging points and then sorts these points to find the nearest ones. While functional, this method is not scalable and becomes ineffective as the number of stations increases.

The goals of this project include:

- **Application Flexibility**: Adapting the system to allow for the dynamic activation and deactivation of charging points, providing more flexible management.
- **Efficient Data Structure**: Implementing a **QuadTree**, a data structure better suited for geographic data, to optimize the search for the nearest charging points.
- **Comparison of Approaches**: Evaluating and comparing the current method with the new solution in terms of performance, workload, and efficiency.

These improvements aim to make the system more efficient and suitable for the real management needs of charging stations.
