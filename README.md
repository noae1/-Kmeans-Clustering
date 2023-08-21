# Kmeans-Clustering Project using C and Python:

This repository contains the implementation of the K-means clustering Algorithm in Both C and Python. 

## About
The K-means algorithm is a popular clustering method for finding a partition of N unlabeled
observations x1, x2, . . . , xN ∈ Rd into K distinct clusters, where K is a parameter of the method.

Kmeans algorithm is an iterative algorithm that tries to partition the dataset 
into K distinct non-overlapping subgroups (clusters), where each data point belongs to exactly one cluster.
Each cluster k is represented by it’s centroid which is the mean µk ∈ Rd of the cluster’s members.

## Algorithm
1. Initialize centroids as first k datapoints: µk = xk, ∀k ∈ K
2. repeat:
3.    Assign every xi to the closest cluster k: argmin_k d(xi, µk), ∀k 1 ≤ k ≤ K
4.    Update the centroids: µk = 1/|k|* sumOf(xi : xi∈k )
5. until convergence: ∆µk < epsilon = 0.001 OR iteration_number = specified MAX_iteration, 
when ∆µk calculated by Eclidean Distance, between the updated centroid to the previous one, for all centroids.

## Requirements
1. K - number of clusters, 1 < K < N.
2. MAX_iteration number - 1 < iter < 1000, if not provided, default value is 200.
3. The datapoints should be provided in a .txt file.

## Compile and Running
# C
 Compile the code using: ```gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans```.
 For example, run: ```./kmeans 3 100 <input_data.txt``` (K = 3, iter = 100).

## Python
 For example, run: ```python3 kmeans.py 3 100 input_data.txt``` (K = 3, iter = 100).
