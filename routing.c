#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

#define N 6
#define INF 666

/*STRUCTS*/
struct Edge{
    char src;
    char dst;
    int cost;
};

struct CostPair{
    char parent;
    int cost;
};

/*PROTOTYPES*/
void dijkstra (int graph[N][N], int src);
int minDistance (int dist[], bool included[]);
void computeForwarding (int distances[], int pathrecord[], int src);
void computeDijkstra(int adjacencymatrix[][N], int MST[][N]);
void buildAdjacencyMatrix(struct Edge links[], int adjacecymatrix[][N]);
void readfile(struct Edge links[]);
void printMatrix(int adjacencymatrix[][N]);
void writefile(char buffer[]);

void buildMST(struct CostPair table[N-1][N], int MST[][N], int adjacecymatrix[][N]);
void LCE(int MST[][N], char src, char edge, char dst, int n);
int getMin(int a, int b);
void printDijkstras(struct CostPair table[N-1][N], char *included);
int isIncluded(char *included, char new);
void dijkstras(int adjacencymatrix[][N], int MST[][N], int src);
void forwarding (int MST[][N]);

int main(){
    
    struct Edge links[10];
    int adjacencymatrix[N][N] = {0};
    int MST[N][N] = {0};
    
    /*Remove the output file before computing new routing file*/
    remove("LS.txt");
    
    /*READ FILE*/
    readfile(links);
    
    /*build weighted graph representation using adjecency matrix*/
    buildAdjacencyMatrix(links, adjacencymatrix);
    printf("Graph:\n"), printMatrix(adjacencymatrix);
    
    /*Perform Dijkstra's find the MST and compute forwarding tables*/
    computeDijkstra(adjacencymatrix, MST);
    
    return 0;
}

/*Implementation of the dijkstra's algorithm*/
void dijkstras(int adjacencymatrix[][N], int MST[][N], int src){
    struct CostPair table[N][N] = {0};
    /*nodes included in computing dijkstras*/
    char included[N+1] = {'\0'};

    int min = INF;
    int nextnode = 0;

    included[0] = src + 'u';

    /*Initialize the first row of dijkstra's table*/
    for(int i = 0; i < N; i++) {
        /*populate the first row with connected edges in adjacency matrix*/
        if(adjacencymatrix[src][i] != 0 || i == src) {
            table[0][i].cost = adjacencymatrix[src][i];
        } else {
            table[0][i].cost = INF;
        }
        /*write the parent*/
        table[0][i].parent = src + 'u';

        /*find the minimum cost link to create the next row of dijkstra*/
        if (table[0][i].cost < min && i!=0){
            min = table[0][i].cost;
            nextnode = i;
        }
    }

    /*include the minimum*/
    included[1] = (nextnode + 'u');


    /*Compute the table for dijkstra*/
    for(int i = 1; i < N; i++) {

        for(int j = 0; j < N; j++) {
            /*populate the table row with connected edges*/
            if(adjacencymatrix[nextnode][j] != 0 || table[i-1][j].cost != INF) {
                /*populate the table with the minimum edge*/
                //the table element is either taken from previous row or the adjacency matrix
                table[i][j].cost = getMin(min + adjacencymatrix[nextnode][j], table[i-1][j].cost);

                /*add the parent for the table element*/
                if(table[i][j].cost == table[i-1][j].cost){
                    table[i][j].parent = table[i-1][j].parent;
                } else {
                    table[i][j].parent = nextnode + 'u';
                }

            /*If there is no link yet found write infinite*/
            } else if (nextnode != j){
                table[i][j].cost = INF;
                table[i][j].parent = i + 'u';
            /*otherwise get the value from previous row (Redundant)*/
            } else {
                table[i][j].cost = table[i-1][j].cost;
                table[i][j].parent = table[i-1][j].parent;
            }
        }

        /*After populating the row get the nextnode to compute the next row*/
        min = INF;

        for(int j = 0; j < N; j++) {

            if (isIncluded(included, j + 'u')) { continue; }

            if (table[i][j].cost < min){
                min = table[i][j].cost;
                nextnode = j;
            }
        }

        included[i+1] = (nextnode + 'u');
    }

    printf("\n");
    printDijkstras(table, included);
    printf("\n");

    /*create the minimum spanning tree using dijkstras*/
    buildMST(table, MST, adjacencymatrix);

}

/*compute the forwarding table for each src*/
void forwarding(int MST[][N]){
    /*String for writing to output file*/
    char link[10] = {'\0'};

    /*compute LCE to each destination form each source*/
    for (int src = 0; src < N; src++) {
        for (int dst = 0; dst < N; dst++) {

            if(src != dst) {
                if (MST[src - 'u'][dst - 'u'] != 0) {
                    sprintf(link, "%c (%c, %c)\n", dst + 'u', src + 'u', dst + 'u');
                } else {

                    int edge = dst;
                    /*Check candidate edge for each node*/
                    for (int e = 0; e < N; e++) {
                        if(MST[src][e] != 0){
                            LCE(MST, src + 'u', e + 'u', dst + 'u', N);
                                edge = e;
                                break;
                        }
                    }

                    link[0] = dst + 'u', link[1] = ' ', link[2] = '(', link[3] = src + 'u', link[4] = ',', link[5] = ' ', link[6] = edge + 'u', link[7] = ')', link[8] = '\n', link[9] = '\0';

                }
            }
            
        }
    }
    
}


/*recursive function for finding the least cost edge*/
void LCE(int MST[][N], char src, char edge, char dst, int n){
    //printf("(%c %c) -> %d\n", src, dst, MST[src - 'u'][dst - 'u']);
    int links[N][N];
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            links[i][j] = MST[i][j];
        }
    }
    links[src - 'u'][edge - 'u'] = 0;

    if(src == edge) return;
    if (MST[src - 'u'][dst - 'u'] != 0) return;
    if (n == 0) return;

    /*check the edges of the visited node*/
    for(int e = 0; e < N; e++){
        if (links[e][dst - 'u'] != 0)
            return;
        else if (MST[src - 'u'][e] != 0)
            LCE(links, src, e + 'u', dst, n-1);
        }
}

/*Function computes dijkstra for each src*/
void computeDijkstra(int adjacencymatrix[][N], int MST[][N]){
    /*compute dijkstras form each source*/
    for (int src = 0; src < N; src++) {
        memset(MST, 0, sizeof(MST[0][0]) * N * N);
        dijkstras(adjacencymatrix, MST, src);
        printf("MST:\n"), printMatrix(MST), printf("\n");
    }
    
    printf("Link State Forwarding Table:\n\n");
    for (int src = 0; src < N; src++)
        dijkstra(adjacencymatrix, src);
}

/*Function for building the MST*/
void buildMST(struct CostPair table[N-1][N], int MST[][N], int adjacecymatrix[][N]){
    /*loop the through the nodes*/
    for(int x = 0; x < N; x++) {
        /*get the connected node for MST from dijkstra table*/
        int y = table[N-2][x].parent - 'u';
        
        /*populate the MST adjacency matrix with computed links*/
        MST[x][y] = adjacecymatrix[x][y];
        MST[y][x] = adjacecymatrix[x][y];
    }
}

/*Function builds adjacency ,atrix from edjes*/
void buildAdjacencyMatrix(struct Edge links[], int adjacencymatrix[][N]){
    /*iterate through the array of links*/
    for (int i = 0; i < 10; i++){
        /*get coordinates from links*/
        int x = links[i].src - 'u';
        int y = links[i].dst - 'u';
        
        /*populate the adjacency ,atrix with cost*/
        adjacencymatrix[x][y] = links[i].cost;
        adjacencymatrix[y][x] = links[i].cost;
    }
    
}

/*Function gets minimum of two integer vlaues*/
int getMin(int a, int b){
    return a <= b ? a : b;
}

/*Subroutine for printing the adjacency matrix*/
void printMatrix(int adjacencymatrix[][N]){
    char arr[] = "uvwxyz";
    printf("  u v w x y z\n");
    
    for(int i = 0; i < N; i++) {
        printf("%c ", arr[i]);
        for(int j = 0; j < N; j++) {
            printf("%d ",  adjacencymatrix[i][j]);
        }
        printf("\n");
    }
}

/*Subroutine for printing Dijkstra's table*/
void printDijkstras(struct CostPair table[N-1][N], char *included){
    printf("Dijkstra's Algorithm Table: \n");
    printf("\tu\tv\tw\tx\ty\tz\n");
    for(int i = 0; i < N-1; i++) {
        for(int s = 0; s <= i; s++)
            printf("%c", included[s]);
        
        for(int j = 0; j < N; j++) {
            if(table[i][j].cost == INF)
                printf("\t%s,%c", "∞", table[i][j].parent);
            else
                printf("\t%d,%c", table[i][j].cost, table[i][j].parent);
        }
        printf("\n");
    }
}

/*Checks if the node is included in computing dijkstra*/
int isIncluded(char *included, char new){
    int found = 0;
    /*iterate through string to see if the new node is found*/
    for(int i = 0; i < strlen(included); i++){
        if (new == included[i])
            found = 1;
    }
    
    return found;
}

/*Function for reading file: router.txt*/
void readfile(struct Edge links[]){
    FILE *fp;
    fp = fopen("router.txt", "r");
    if(fp) {
        int c, i = 0;
        while(c != EOF){
            fscanf(fp, "%c %c %d", &links[i].src, &links[i].dst, &links[i].cost);
            c = fgetc(fp), i++;
        }
    } else {
        printf("Error: file <router.txt> not found\n");
    }
    
    fclose(fp);
    
    
}

/*Function for writing file: LS.txt*/
void writefile(char buffer[]){
    FILE *fp;
    fp = fopen("LS.txt", "a+");
    
    fprintf(fp, "%s", buffer);
    
    fclose(fp);
}

int PreNode = 0;  // Previous node in the path
int R = 0;       // PreNode counter for tracing path record

/*compute the Least Cost Forwarding Edge*/
void LCFE(int pathrecord[], int dst) {
    /*If the dest is source itself*/
    if (pathrecord[dst] == -1) return;
    /*recursively traverse the path*/
    LCFE(pathrecord, pathrecord[dst]);
    /*Record the previous node*/
    R++; if(R == 1) PreNode = dst;
}

/*compute forwarding for the source*/
void computeForwarding (int distances[], int pathrecord[], int src) {
    /*String for writing to output file*/
    char link[10] = {'\0'};
    /*compute the forwarding edge for each destination*/
    for (int dst = 0; dst < N; dst++) {
        /*find the least cost forwarding edge*/
        LCFE(pathrecord, dst);
        /*generate the link for the table and write*/
        if (distances[dst] != 0) {
            sprintf(link, "%c (%c, %c)\n", dst + 'u', src + 'u', PreNode + 'u');
            printf("%s",link);
            writefile(link);
        }
        R = 0;
    }
    printf("--------\n");
    strcpy(link, "--------\n");
    writefile(link);
}

/*Citation: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/ */
//Eddited for use in the assignment, This is being used to compute forwarding

/* Auxillary function to find the vertex with minimum distance vertex */
int minDistance (int dist[], bool included[]) {
    int min = INF, min_index;
    /*iterate through the nodes and find min distance*/
    for (int n = 0; n < N; n++)
        if (included[n] == false && dist[n] <= min)
            min = dist[n], min_index = n;
  
    return min_index;
}
  
/* Function that implements Dijkstra's single source shortest path algorithm */
void dijkstra(int graph[N][N], int src) {
    
    int dist[N] = {INF};             // Array holds the minimum distance from source to node
    bool included[N] = {false};     // Array records the incuded node as boolean 1-0 at node index
    int pathrecord[N];             // Array records nodes included to form path
  
    /*Initialize all distances as INFINITE and included as false*/
    for (int i = 0; i < N; i++) dist[i] = INF, included[i] = 0;
  
    /*Distance from source vertex to itself is always 0*/
    dist[src] = 0;
    /*source has no predecessor node in path*/
    pathrecord[src] = -1;
  
    /*Find shortest path for all vertices*/
    for (int i = 0; i < N - 1; i++) {
        /*Include the minimum distance vertex from the set of nodes not included*/
        int n = minDistance(dist, included);
        included[n] = true;
  
        /*Update distance values of the adjacent vertices of the new node n*/
        for (int j = 0; j < N; j++)
            if (!included[j] && graph[n][j] && dist[n] != INF && dist[n] + graph[n][j] < dist[j])
                dist[j] = dist[n] + graph[n][j], pathrecord[j] = n;
    }
  
    /*Compute the Forwarding table for the source*/
    computeForwarding(dist, pathrecord, src);
}
