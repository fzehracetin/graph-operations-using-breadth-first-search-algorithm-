#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2415

typedef struct 
{
    int queue[MAX];
    int parent[MAX];
    int rear;
    int front;
}QUEUE;

void initiliazeQueue(QUEUE *q) 
{
    q->rear = 0;
    q->front = 0;
}

//it makes a char matrix from words in the file
void wordsArrayMaker(FILE *f, char** words) 
{ 
    char read[7]; //dosyadan okunan kelime
    int i = 0;//dizinin indisi
    fseek(f, 0, SEEK_SET);
    while(!feof(f)) 
    {
        fscanf(f, "%s", read);
        strcpy(words[i], read);
        i++;
    }
}

//it searches the given word in the file
int diffFinder(char* word, FILE* f) 
{
    int diff = 1, i = 0, j = 0;
    char read[5];
    fseek(f, 0, SEEK_SET);
    while (!feof(f) && (diff!= 0)) 
    {
        fscanf(f, "%s", read);
        j = 0; diff = 0;
        while ( (j < 5) && (diff < 1)) 
	{
            if ( word[j] != read[j])
                diff++;
            j++;
        }
        i++;
    }
    if (diff != 0) 
    {
        printf("This word does not exist in file!\n");
        i = -1;
    }
    return i; // returns to index
}

//it makes adjacency matrix for words in file
void matrixMaker(FILE* f1, FILE* f2, int **adjacency) 
{ 
    char row[5], col[5];
    int i = 0, j = 0, k = 0, diff = 0;
    while( !feof(f1)) 
    {
        fscanf(f1, "%s", row);
        fseek(f2, 0, SEEK_SET);
        j = 0;
        while(!feof(f2)) 
	{
            fscanf(f2, "%s", col);
            diff = 0; k = 0;
            while ( (k < 5) && (diff < 2)) 
	    { 
                if ( row[k] != col[k])
                    diff++;// diffrence between row word and column word             
                k++;
            }
            if (diff == 1) 
                adjacency[i][j] = 1;
            else 
                adjacency[i][j] = 0;
            j++;
        }
        i++;
    }
}

//it controls for the adjacency between two given words
void adjacencyFinder(char* word1, char* word2, FILE* f, int** adjacency) 
{ 
    int i = 0, j = 0;
    i = diffFinder(word1, f); // finds the word in file, returns to index
    j = diffFinder(word2, f);
    if ((i == -1) || (j == -1) ) //if it equals to -1, the word is not exist in file
        return;
    
    i--; j--;
    if (adjacency[i][j] == 1)
        printf("There is adjacency between this two words. \n");
    else 
        printf("There is no adjacency. \n");
}

// enqueued for the word that dequeued to queue
int adjacencyEnqueue(int i, int wanted, QUEUE *q, int *visited, int **adjacency, char** words) 
{ 
    int j;
    for (j = 0; j < MAX; j++) 
    {
        if ((adjacency[i][j] == 1) && (visited[j] == 0)) 
	{
            q->queue[q->rear++] = j; //enqueue 
            q->parent[j] = i;
            visited[j] = 1;
            if (j == wanted) 
                return 1;       
        }
    }
    return 0;
}

//breadth first search
void breadthFirstSearch(QUEUE *q, int wanted, int* visited, int **adjacency, char** words, int start) 
{
    int v, flag = 0, i, j, count = 0, yol[MAX];
    while ((q->front < q->rear) && (!flag)) 
    {
        v = q->queue[q->front++]; //dequeue
        printf("dequeue : %s \n", words[v]);
        flag = adjacencyEnqueue(v, wanted, q, visited, adjacency, words);
    }
    if (!flag) 
        printf("There is no transformation between these two words! \n");
    else 
    {
        i = wanted; 
	j = 0;
        while(i != start) 
	{
            yol[j++] = i;
            i = q->parent[i];
        }
        yol[j] = start;
        printf("\n");
        while(j > 0) 
	{
            printf("%s ->", words[yol[j--]]);
            count++;
        } 
	printf("%s \n", words[wanted]);
        printf("Word transformed with %d character change\n", count);
        printf("\n");
    }
}

//this function searches for transformation in two words in a file
void transformFinder(char* word1, char* word2, FILE* f, int ** adjacency, char** words) 
{
    int i, j, flag= 0, visited[MAX];
    QUEUE q;// kuyruk
    
    for (i = 0; i< MAX; i++) 
    {
        visited[i] = 0; //initialization
    }
    initiliazeQueue(&q);
    
    i = diffFinder(word1, f);
    j = diffFinder(word2, f); 
    
    if ((i != -1) && (j != -1) ) 
    {
        i--; j--; 
        visited[i] = 1;
        flag = adjacencyEnqueue(i, j, &q, visited, adjacency, words); //word1's neigbours enqueued to queue
        if (flag) //finds in the first step
	{ 
            printf("There is transformation between these to words.\n");
	    printf("Word transformation with 1 character change. \n");
            printf("%s -> %s \n", words[i], words[j]);
        }
        else 
            breadthFirstSearch(&q, j, visited, adjacency, words, i);
    }
}

int main(int argc, char **argv)
{
    FILE *f1, *f2;// kelime dosyasi
    int i, secim;
    char word1[5], word2[5];
    
    f1 = fopen("D:\\kelime.txt", "r");
    if (f1 == NULL) 
    {
        printf("File could not opened \n");
    }
    
    f2 = fopen("D:\\kelime.txt", "r");
    if (f2 == NULL) 
    {
        printf("File could not opened \n");
    } 
    // opened twice beacuse of the matrixMaker function.
    
    char **words = (char**) malloc(MAX *sizeof(char*));
    for (i = 0; i < MAX; i++) {
        words[i] = (char*) malloc(6 *sizeof(char));
    }
    int **adjacency = (int**) malloc(MAX *sizeof(int*));
    for (i = 0; i < MAX; i++) {
        adjacency[i] = (int*) malloc(MAX* sizeof(int));
    }
    
    matrixMaker(f1, f2, adjacency);
    wordsArrayMaker(f1, words);
    system("cls");
    printf("_________________GRAPH PROCESSING_______________ \n");
    printf("1 - Finding adjacency between two words \n");
    printf("2 - Finding transformation between two words \n");
    printf("3 - Exit \n");
    printf("Welcome to graph processing. Make your selection : ");
    scanf("%d", &secim);
    switch(secim) {
        case 1:
            printf("\n");
            printf("Finding adjacency between two words \n");
            printf("Write the two words. \n");
            printf("1st word : ");
            scanf("%s", word1);
            printf("2nd word : ");
            scanf("%s", word2);
            adjacencyFinder(word1, word2, f1, adjacency);
            break;
        case 2:
            printf("\n");
            printf("Finding transformation between two words \n");
            printf("Write the two words. \n");
            printf("1st kelime : ");
            scanf("%s", word1);
            printf("2nd kelime : ");
            scanf("%s", word2);
            transformFinder(word1, word2, f1, adjacency, words);
            break;
        case 3:
            break;
    }
   
    
	return 0;
}
