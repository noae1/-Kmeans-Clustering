#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define EPSILON 0.001
#define assertAndReturn(cond)                   \
        if (!(cond)) {                          \
            printf("An Error Has Occurred");    \
            return 1;                           \
        }                                       \


#define assertErrorOccurred()                   \
        if (errorOccurred) {                    \
            printf("An Error Has Occurred");    \
            return 1;                           \
        }                                       \

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};
int errorOccurred = 0;
void prepData();
int isNumber(char number[]);
void setZerosToNewCentroids();

void freeLinkedList(struct vector *vec);
void freeCordsList(struct cord *cord);

double calcMaxCentroidsDelta();

double d(struct cord v1,struct cord v2);

struct vector *data =NULL;
struct vector *newCentroids;
struct vector *prevCentroids;


int iteration_number = 1;
int k;
int iter = 200;
int rowsCount=0;
int columnCount=0;
double maxCentroidDelta = EPSILON+1;


void copyFirstKToNew();

double calcMaxCentroidsDelta();
int isDigit(char a);

void printCentroids(struct vector *pVector);

int main(int argc, char *argv[] )
{


    int i =0,j=0;
    double minDist;
    struct vector *currPrevCentroid;
    if(argc == 3){
        if(!isNumber(argv[2])){
            printf("Invalid maximum iteration!");
            return 1;
        }
        iter = atoi(argv[2]);
    }
    else{
        if(argc != 2){
            printf("An Error Has Occurred");
            return 1;
        }
    }
    if(!isNumber(argv[1])){
        printf("Invalid number of clusters!");
        return 1;
    }
    k = atoi(argv[1]);

    if(iter<=1 || iter >= 1000 || iter % 1 != 0){
        printf("Invalid maximum iteration!");
        return 1;
    }
    prepData();
    assertErrorOccurred();


    if(k<=1 || k >= rowsCount || k % 1 != 0){
        freeLinkedList(data);
        printf("Invalid number of clusters!");
        return 1;

    }


    copyFirstKToNew(data);
    assertErrorOccurred();

    while(iteration_number < iter && maxCentroidDelta>=EPSILON){
        struct vector *currNewCentroid;

        struct vector *currVector = data;
        int *assignmentsCounter = calloc(k, sizeof (int));
        assertAndReturn(assignmentsCounter!=NULL);
        freeLinkedList(prevCentroids);
        assertErrorOccurred();
        prevCentroids = newCentroids;
        setZerosToNewCentroids();
        assertErrorOccurred();
        i = 0;
        for(; i< rowsCount; i++){
            int minIndex= 0;
            struct vector *minPointer;


            struct cord *minDistCentCord;
            struct cord *cordsToAdd;


            currNewCentroid = newCentroids;
            currPrevCentroid = prevCentroids;
            minDist = d(*currPrevCentroid->cords,*currVector->cords);
            minPointer = currNewCentroid;

            j =1;
            for (; j< k; j++){
                double dis;
                currPrevCentroid = currPrevCentroid->next;
                currNewCentroid = currNewCentroid->next;  
                dis = d(*currPrevCentroid->cords,*currVector->cords);
                if(dis<minDist) {
                    minDist=dis;
                    minPointer = currNewCentroid;
                    minIndex=j;
                }
            }   
            assignmentsCounter[minIndex]++;


            minDistCentCord = minPointer->cords;
            cordsToAdd = currVector->cords;
            j = 0;
            for(; j<columnCount; j++){
                minDistCentCord->value += cordsToAdd->value;
                minDistCentCord = minDistCentCord->next;
                cordsToAdd = cordsToAdd->next;
            }

            currVector= currVector->next;
        }


        currNewCentroid= newCentroids;
        i = 0;
        for (;i<k;i++){
            if(assignmentsCounter[i]>0) {
                struct cord *cordsToChange = currNewCentroid->cords;
                int j=0;
                for(;j<columnCount;j++){
                    cordsToChange->value = cordsToChange->value / assignmentsCounter[i];
                    if (j != columnCount) {
                        cordsToChange = cordsToChange->next;
                    }
                }
            }
            currNewCentroid=currNewCentroid->next;
        }


        free(assignmentsCounter);
        iteration_number++;
        maxCentroidDelta = calcMaxCentroidsDelta();
        assertErrorOccurred();
    }
    printCentroids(newCentroids);
    assertErrorOccurred();
    freeLinkedList(newCentroids);
    assertErrorOccurred();
    freeLinkedList(prevCentroids);
    assertErrorOccurred();
    freeLinkedList(data);
    assertErrorOccurred();
    return 0; 
}

void printCentroids(struct vector *pVector) {
    struct vector *headVector = pVector;

    int i =1;
    
    if(pVector==NULL){
        errorOccurred = 1;
        return;
    }
    for(; i<=k; i++){
        struct cord *cords = headVector->cords;
        int j=1;
        for(;j<=columnCount; j++){
            printf("%0.4f",cords->value);
            if(j!=columnCount){
                printf(",");
                cords=cords->next;
            }
        }
        printf("\n");
        headVector=headVector->next;
    }
}

double calcMaxCentroidsDelta() {

    struct vector *currNewCentroid = newCentroids;
    struct vector *currPrevCentroid = prevCentroids;
    int i =1;

    maxCentroidDelta = d(*currNewCentroid->cords,*currPrevCentroid->cords);
    if(!(maxCentroidDelta>=0)){
        errorOccurred = 1;
        return -1;
    }

    for(;i<k;i++){
        double dis;
        currNewCentroid = currNewCentroid->next;
        currPrevCentroid = currPrevCentroid->next;
        dis =  d(*currNewCentroid->cords,*currPrevCentroid->cords);
        if(!(dis>=0)){
            errorOccurred = 1;
            return -1;
        }

        if(dis > maxCentroidDelta)
            maxCentroidDelta = dis;
    }
    return maxCentroidDelta;

}

int isNumber(char number[])
{
    int i = 0;
    for (; number[i] != 0; i++)
    {
        if (!isDigit(number[i]))
            return 0;
    }
    return 1;
}
int isDigit(char a){
    if (a-'0'<= 9 &&a-'0'>=0){
        return 1;
    }
    return 0;
}


void prepData(){
    double n;
    char c;
    int columnsCountInitialized=0;

    struct vector *head_vec, *curr_vec,*prev_vec;
    struct cord *head_cord, *curr_cord;


    head_cord = malloc(sizeof(struct cord));
    if(!(head_cord!=NULL)){
        errorOccurred = 1;
        return;
    }
    

    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    if(!(head_vec!=NULL)){
        errorOccurred = 1;
        return;
    }

    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (scanf("%lf%c", &n, &c) == 2)
    {
        if(!columnsCountInitialized){
            columnCount++;
        }

        if (c == '\n')
        {
            columnsCountInitialized=1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if(!(curr_vec->next!=NULL)){
                errorOccurred = 1;
                return;
            }

            prev_vec=curr_vec;
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if(!(head_cord!=NULL)){
                errorOccurred = 1;
                return;
            }

            curr_cord = head_cord;
            curr_cord->next = NULL;
            rowsCount++;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        if(!(curr_cord->next!=NULL)){
            errorOccurred = 1;
            return;
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;

    }
    prev_vec->next = NULL;
    free(head_cord); 
    free(curr_vec); 

    data = head_vec;
}

void copyFirstKToNew() {
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i=1;

    struct vector *curr_data_vec = data;
    struct cord *curr_data_cord;
    head_vec = malloc(sizeof(struct vector));
    if(!(head_vec!=NULL)){
        errorOccurred = 1;
        return;
    }

    curr_vec = head_vec;
    curr_vec->next = NULL;
    for(;i<=k;i++){
        int j=1;

        curr_data_cord = curr_data_vec->cords;
        head_cord = malloc(sizeof(struct cord));
        if(!(head_cord!=NULL)){
            errorOccurred = 1;
            return;
        }
        curr_cord = head_cord;
        for (;j<=columnCount; j++){
            curr_cord->value = curr_data_cord->value;

            if(j != columnCount) {
                curr_cord->next = malloc(sizeof(struct cord));
                if(!(curr_cord->next!=NULL)){
                    errorOccurred = 1;
                    return;
                }
                curr_cord = curr_cord->next;
            }
            curr_cord->next = NULL;
            curr_data_cord=curr_data_cord->next; 
        }

        curr_data_vec = curr_data_vec->next;
        curr_vec->cords=head_cord;
        if(i!=k){
            curr_vec->next = malloc(sizeof(struct vector));
            if(!(curr_vec->next!=NULL)){
                errorOccurred = 1;
                return;
            }
            curr_vec = curr_vec->next;
        }
        curr_vec->next=NULL;

    }
    newCentroids = head_vec;

}
void setZerosToNewCentroids(){
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i=1;

    head_vec= malloc(sizeof (struct vector));
    if(!(head_vec!=NULL)){
        errorOccurred = 1;
        return;
    }
    curr_vec = head_vec;
    for(;i<=k;i++) {
        int j=1;

        head_cord = malloc(sizeof (struct cord));
        if(!(head_cord!=NULL)){
            errorOccurred = 1;
            return;
        }
        curr_cord = head_cord;
        curr_vec->cords=head_cord;
        for (;j<=columnCount; j++) {
            curr_cord->value=0;
            if(j != columnCount){
                curr_cord->next = malloc(sizeof(struct cord));
                if(!(curr_cord!=NULL)){
                    errorOccurred = 1;
                    return;
                }
                curr_cord = curr_cord->next;
            }
            curr_cord->next=NULL;
        }
        if(i!= k){
            curr_vec->next= malloc(sizeof (struct vector));
             if(!(curr_vec!=NULL)){
                errorOccurred = 1;
                return;
            }
            curr_vec=curr_vec->next;
        }
        curr_vec->next = NULL;
    }
    newCentroids = head_vec;
}
double d(struct cord v1,struct cord v2){
    double sum = 0;
    int i =1;
    for(;i<=columnCount; i++){
        sum += pow(v1.value-v2.value,2);
        if(i!=columnCount) {
            v1 = *v1.next;
            v2 = *v2.next;
        }
    }
    return sqrt(sum);
}
void freeLinkedList(struct vector *vec){
    if(vec!= NULL){

        freeLinkedList(vec->next);

        freeCordsList (vec->cords); 

        free(vec);
    }
}
void freeCordsList(struct cord *cord){
    if(cord != NULL){
        freeCordsList(cord->next);
        free(cord);
    }
}