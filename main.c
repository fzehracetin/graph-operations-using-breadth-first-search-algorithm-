#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2415

typedef struct {
    int queue[MAX];
    int parent[MAX];
    int rear;
    int front;
}QUEUE;

void initiliazeQueue(QUEUE *q) {
    q->rear = 0;
    q->front = 0;
}

void wordsArrayMaker(FILE *f, char** words) { //dosyadaki kelimelerden kelime dizisi olusturur
    char read[7]; //dosyadan okunan kelime
    int i = 0;//dizinin indisi
    fseek(f, 0, SEEK_SET);
    while(!feof(f)) {
        fscanf(f, "%s", read);
        strcpy(words[i], read);
        i++;
    }
}

int diffFinder(char* word, FILE* f) { //verilen kelime dosyada var mı kontrol eder
    int diff = 1, i = 0, j = 0;// okunan kelime ve parametre olan kelime arasındaki fark : diff, i:indis, j:indis
    char read[5];//dosyadan okunan kelime
    fseek(f, 0, SEEK_SET);
    while (!feof(f) && (diff!= 0)) {
        fscanf(f, "%s", read);
        j = 0; diff = 0;
        while ( (j < 5) && (diff < 1)) { //kelime ile okunan kiyaslanir
            if ( word[j] != read[j])
                diff++;
            j++;
        }
        i++;
    }
    if (diff != 0) {
        printf("Aranan kelime matriste yok!\n");
        i = -1;
    }
    return i;
}

void matrixMaker(FILE* f1, FILE* f2, int **adjacency) { //adjacency matrisini olusturur
    char row[5], col[5];//okunan satir ve sutun
    int i = 0, j = 0, k = 0, diff = 0;//i, j, k: indis; diff: satir ve sutun arasindaki fark
    while( !feof(f1)) {
        fscanf(f1, "%s", row); // okunan kelime
        fseek(f2, 0, SEEK_SET);
        j = 0;
        while(!feof(f2)) {
            fscanf(f2, "%s", col);
            //****************************
            //fonksiyonunun yazilmamasi sebebi 2415*2415 kere function call yapmamak
            diff = 0; k = 0;
            while ( (k < 5) && (diff < 2)) { 
                if ( row[k] != col[k])
                    diff++;// satir ve sutun arasindaki fark             
                k++;
            }
            //****************************
            if (diff == 1) //matrise degerler atanir
                adjacency[i][j] = 1;
            else 
                adjacency[i][j] = 0;
            j++;
        }
        i++;
    }
}

void adjacencyFinder(char* word1, char* word2, FILE* f, int** adjacency) { //verilen iki kelime arasinda baglanti var mi kontrol eder
    int i = 0, j = 0;
    i = diffFinder(word1, f);// dosyada kelimeyi buldurur, indisini alir.
    j = diffFinder(word2, f);
    if ((i == -1) || (j == -1) ) //-1 ise kelime dosyada yok
        return;
    
    i--; j--;
    if (adjacency[i][j] == 1)
        printf("Baglanti var. \n");
    else 
        printf("Baglanti yok. \n");
}

int adjacencyEnqueue(int i, int wanted, QUEUE *q, int *visited, int **adjacency, char** words) { //kuyruktan dequeue edilen elemanin komsulari kuyruga enqueue edilir
    int j;//indis
    for (j = 0; j < MAX; j++) {
        if ((adjacency[i][j] == 1) && (visited[j] == 0)) {
            q->queue[q->rear++] = j;
            q->parent[j] = i;
            visited[j] = 1;
            if (j == wanted) {
                printf("Kuyruk : ");
                for (j = q->front; j < q->rear; j++) {
                    printf("%s ", words[q->queue[j]]);
                } printf("\n");
                return 1;
            }
        }
    }
    printf("Kuyruk : ");
    for (j = q->front; j < q->rear; j++) {
        printf("%s ", words[q->queue[j]]);
    } printf("\n");
    return 0;
}

void breadthFirstSearch(QUEUE *q, int wanted, int* visited, int **adjacency, char** words, int start) {
    int v, flag = 0, i, j, count = 0, yol[MAX];// v: dugum indisinin atanacagi degisken; flag: durum kontrolu; i, j: indis; yol: yolun tutulacagi dizi
    while ((q->front < q->rear) && (!flag)) {
        v = q->queue[q->front++]; //dequeue
        printf("dequeue : %s \n", words[v]);
        flag = adjacencyEnqueue(v, wanted, q, visited, adjacency, words);
    }
    if (!flag) 
        printf("Baglanti yok! \n");
    else {
        i = wanted; j = 0;
        while(i != start) {
            yol[j++] = i;
            i = q->parent[i];
        }
        yol[j] = start;
        printf("\n");
        while(j > 0) {
            printf("%s ->", words[yol[j--]]);
            count++;
        } printf("%s \n", words[wanted]);
        printf("%d harf degisimi ile kelime donusturuldu \n", count);
        printf("\n");
    }
}

void transformFinder(char* word1, char* word2, FILE* f, int ** adjacency, char** words) {
    int i, j, flag= 0, visited[MAX];//i, j: indis; flag: durum; visited: ziyaret edilenler dizisi
    QUEUE q;// kuyruk
    
    for (i = 0; i< MAX; i++) {
        visited[i] = 0; //initialization
    }
    initiliazeQueue(&q);
    
    i = diffFinder(word1, f);
    j = diffFinder(word2, f); // aranan
    
    if ((i != -1) && (j != -1) ) {
        i--; j--; //dizi indisleri 0'dan basladigi icin
        visited[i] = 1;
        flag = adjacencyEnqueue(i, j, &q, visited, adjacency, words); //word1'in komsulari kuyruga atilir
        if (flag) { //ilk adimda bulundu 
            printf("Kelimeler arasinda baglanti var. 1 harf degisimi ile donusum saglandi. \n");
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
    if (f1 == NULL) {
        printf("Dosya acilamadi \n");
    }
    f2 = fopen("D:\\kelime.txt", "r");
    if (f2 == NULL) {
        printf("Dosya acilamadi \n");
    } // iki defa acilmasi sebebi adjacency matrisi olusturma yontemi ile alakali
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
    printf("_________________GRAF ISLEMLERI_______________ \n");
    printf("1 - Kelimeler arasi baglanti buldurma \n");
    printf("2 - Kelimeler arasi donusum buldurma \n");
    printf("3 - Cikis \n");
    printf("Graf islemlerine hos geldiniz. Seciminizi yapiniz : ");
    scanf("%d", &secim);
    switch(secim) {
        case 1:
            printf("\n");
            printf("Kelimeler arasi baglanti bulma \n");
            printf("Baglantilari bulunacak kelimeleri giriniz. \n");
            printf("1. kelime : ");
            scanf("%s", word1);
            printf("2. kelime : ");
            scanf("%s", word2);
            adjacencyFinder(word1, word2, f1, adjacency);
            break;
        case 2:
            printf("\n");
            printf("Kelimeler arasi donusum bulma \n");
            printf("Donusum adimlari bulunacak kelimeleri giriniz. \n");
            printf("1. kelime : ");
            scanf("%s", word1);
            printf("2. kelime : ");
            scanf("%s", word2);
            transformFinder(word1, word2, f1, adjacency, words);
            break;
        case 3:
            break;
    }
   
    
	return 0;
}
