#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#define INF 99999

typedef struct
{
    int u, v, w;
} Edge;

typedef struct
{
    int V;
    int **adj;
    Edge *edges;
    int edgeCount;
    int edgeCapacity;
} Graph;

Graph* createGraph(int V, int maxEdges)
{
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->V = V;
    g->edgeCount = 0;
    g->edgeCapacity = maxEdges;

    g->adj = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++)
    {
        g->adj[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++)
            g->adj[i][j] = INF;
    }

    g->edges = (Edge*)malloc(maxEdges * sizeof(Edge));
    return g;
}

void addEdge(Graph *g, int u, int v, int w)
{
    if (u >= g->V || v >= g->V || g->edgeCount >= g->edgeCapacity)
    {
        if (g->edgeCount >= g->edgeCapacity)
        {
            printf("Warning: Edge capacity limit reached. Edge not added.\n");
        }
        else
        {
            printf("Error: Node index out of bounds.\n");
        }
        return;
    }

    g->adj[u][v] = w;
    g->edges[g->edgeCount++] = (Edge){u, v, w};
}

void freeGraph(Graph *g)
{
    if (!g) return;
    for (int i = 0; i < g->V; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g->edges);
    free(g);
}

Graph* createSampleGraph()
{
    printf("Creating Sample Graph with 10 Nodes (0-9)...\n");
    Graph *g = createGraph(10, 20); 
    
    addEdge(g, 0, 1, 4);
    addEdge(g, 0, 2, 2);
    addEdge(g, 1, 3, 5);
    addEdge(g, 1, 4, 3);
    addEdge(g, 2, 5, 8);
    addEdge(g, 3, 6, 10); 
    addEdge(g, 4, 7, 2); 
    addEdge(g, 5, 8, 6); 
    addEdge(g, 6, 9, 1); 
    addEdge(g, 7, 9, 12); 
    addEdge(g, 8, 9, 7); 
    addEdge(g, 2, 1, 1); 
    addEdge(g, 3, 5, 4); 
    addEdge(g, 5, 7, 5);
    addEdge(g, 8, 4, 2);
    
    return g;
}

void reconstructPath(int *pred, int start, int end)
{
    int path[100], count = 0;
    int curr = end;
    
    if (pred[end] == -1 && end != start)
    { 
        printf("N/A");
        return;
    }

    while (curr != -1 && curr != start)
    {
        path[count++] = curr;
        curr = pred[curr];
    }
    
    if (curr == start)
    {
        path[count++] = start;
        for (int i = count - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i != 0) printf(" -> ");
        }
    }
    else 
    {
        printf("N/A"); 
    }
}

void reconstructFWPath(int **pred, int start, int end)
{
    if (pred[start][end] == -1 && start != end)
    {
        printf("N/A");
        return;
    }
    
    if (pred[start][end] == start)
    {
        printf("%d -> %d", start, end);
    }
    else if (pred[start][end] == -1)
    {
        printf("%d", start);
    }
    else
    {
        reconstructFWPath(pred, start, pred[start][end]);
        printf(" -> %d", end);
    }
}

void dijkstra(Graph *g, int start, int *dist, int *pred, double *time_ms)
{
    
    int V = g->V;
    int *visited = (int*)calloc(V, sizeof(int));

    for (int i = 0; i < V; i++)
    {
        dist[i] = INF;
        pred[i] = -1;
    }

    dist[start] = 0;
    clock_t t0 = clock();

    
    for (int i = 0; i < V; i++)
    {
        int u = -1, min = INF;
        
        for (int j = 0; j < V; j++)
        {
            if (!visited[j] && dist[j] < min)
            {
                min = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = 1;

        
        for (int v = 0; v < V; v++)
        {
            if (g->adj[u][v] != INF && !visited[v] && dist[u] != INF && dist[u] + g->adj[u][v] < dist[v])
            {
                dist[v] = dist[u] + g->adj[u][v];
                pred[v] = u;
            }
        }
    }

    clock_t t1 = clock();
    *time_ms = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;
    free(visited);
}

int bellmanFord(Graph *g, int start, int *dist, int *pred, double *time_ms)
{
    int V = g->V;
    for (int i = 0; i < V; i++)
    {
        dist[i] = INF;
        pred[i] = -1;
    }

    dist[start] = 0;
    clock_t t0 = clock();

    for (int i = 0; i < V - 1; i++)
    {
        int changed = 0;
        for (int j = 0; j < g->edgeCount; j++)
        {
            int u = g->edges[j].u;
            int v = g->edges[j].v;
            int w = g->edges[j].w;
            
            if (dist[u] != INF && dist[u] + w < dist[v]) 
            {
                
                dist[v] = dist[u] + w;
                pred[v] = u;
                changed = 1;
            }
        }
        if (!changed) break;
    }

    int negCycle = 0;
    for (int j = 0; j < g->edgeCount; j++)
    {
        int u = g->edges[j].u;
        int v = g->edges[j].v;
        int w = g->edges[j].w;
        if (dist[u] != INF && dist[u] + w < dist[v])
        {
            negCycle = 1;
            break;
        }
    }

    clock_t t1 = clock();
    *time_ms = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;
    
    
    if (negCycle)
    {
        for (int i = 0; i < V; i++) dist[i] = INF;
    }

    return negCycle;
}

int** floydWarshall(Graph *g, double *time_ms, int *hasNegCycle, int ***pred_out)
{
    int V = g->V;
    int **dist = (int**)malloc(V * sizeof(int*));
    int **pred = (int**)malloc(V * sizeof(int*)); 

    
    for (int i = 0; i < V; i++)
    {
        dist[i] = (int*)malloc(V * sizeof(int));
        pred[i] = (int*)malloc(V * sizeof(int));

        for (int j = 0; j < V; j++)
        {
            dist[i][j] = g->adj[i][j];

            if (g->adj[i][j] != INF)
                pred[i][j] = i;
            else
                pred[i][j] = -1;
        }

        dist[i][i] = 0;
        pred[i][i] = -1;  
    }

    clock_t t0 = clock();

    
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }

    int negCycle = 0;
    for (int i = 0; i < V; i++)
    {
        if (dist[i][i] < 0)
            negCycle = 1;
    }

    clock_t t1 = clock();
    *time_ms = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;
    *hasNegCycle = negCycle;
    
    *pred_out = pred; 

    printf("\nAll-Pairs Distance Matrix:\n");
    
    printf("Source/Dest\t");
    for (int j = 0; j < V; j++)
        printf("Node %d\t", j);
    printf("\n------------------------------------------------------------\n");

    for (int i = 0; i < V; i++)
    {
        
        printf("Node %d\t\t", i);
        for (int j = 0; j < V; j++)
        {
            if (dist[i][j] >= INF) 
                
                printf("INF\t");
            else
                
                printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------\n");

    return dist;
}


int main()
{    
    Graph *g = createSampleGraph(); 
    int s = 0, t = 9; 
    int V = g->V;

    int *dist = (int*)malloc(V * sizeof(int));
    int *pred = (int*)malloc(V * sizeof(int));
    
    double d_time, bf_time, fw_time;
    int negCycle;

    printf("\n======================================================================\n");
    printf("A. DIJKSTRA'S ALGORITHM\n");
    printf("======================================================================\n");
    dijkstra(g, s, dist, pred, &d_time);
    printf("Time Taken: %.4f ms\n", d_time);
    
    
    printf("Distance %d->%d: %d\n", s, t, dist[t]);
    printf("Path: ");
    if (dist[t] != INF)
        reconstructPath(pred, s, t);
    else
        printf("N/A");
    printf("\n");

    printf("\n======================================================================\n");
    printf("B. BELLMAN-FORD ALGORITHM\n");
    printf("======================================================================\n");
    negCycle = bellmanFord(g, s, dist, pred, &bf_time);
    if (negCycle)
    {
        
        printf("Negative cycle detected — paths undefined.\n"); 
        printf("Distance %d->%d: UNDEFINED\n", s, t);
        printf("Path: N/A\n");
    }
    else
    {
        printf("Distance %d->%d: %d\n", s, t, dist[t]);
        printf("Path: ");
        reconstructPath(pred, s, t);
        printf("\n");
    }
    printf("Time Taken: %.4f ms\n", bf_time);
 
    printf("\n======================================================================\n");
    printf("C. FLOYD-WARSHALL ALGORITHM\n");
    printf("======================================================================\n");
    int hasNegCycle = 0;
    int **fw_dist = NULL;
    int **fw_pred = NULL; 
    
    fw_dist = floydWarshall(g, &fw_time, &hasNegCycle, &fw_pred);
    
    if (hasNegCycle)
    {
        printf("Negative cycle detected — results invalid.\n");
        printf("Distance %d->%d: UNDEFINED\n", s, t);
        printf("Path: N/A\n");
    }
    else
    {
        printf("Time Taken: %.4f ms\n", fw_time);
        printf("Distance %d->%d: %d\n", s, t, fw_dist[s][t]);
        printf("Path: ");
        
        reconstructFWPath(fw_pred, s, t);
        printf("\n");
    }
  
    printf("\n======================================================================\n");
    printf("D. FINAL TIME COMPLEXITY COMPARISON\n");
    printf("======================================================================\n");
    printf("%-15s%-20s%-15s%-30s\n", "Algorithm", "Complexity", "Time (ms)", "When to Use");
    printf("---------------------------------------------------------------------\n");
    
    printf("%-15s%-20s%-15.4f%-30s\n", "Dijkstra", "O(V*V)", d_time, "Fastest for graphs with no negative edges");
    printf("%-15s%-20s%-15.4f%-30s\n", "Bellman-Ford", "O(V * E)", bf_time, "Needed if the graph has negative edges"); 
    printf("%-15s%-20s%-15.4f%-30s\n", "Floyd-Warshall", "O(V*V*V)", fw_time, "To find all shortest paths at once"); 
    printf("======================================================================\n");

    
    free(dist);
    free(pred);
      
    if (fw_dist)
    {
        for (int i = 0; i < V; i++) free(fw_dist[i]);
        free(fw_dist);
    }
     
    if (fw_pred)
    {
        for (int i = 0; i < V; i++) free(fw_pred[i]);
        free(fw_pred);
    }

    freeGraph(g);
    return 0;
}
