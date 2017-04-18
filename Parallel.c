#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>     /* For MPI functions, etc */ 
#include "MPI_Project.h"

/**********function*****************/
void readDataFromFile(int *arr, char *fileName)
{
    char buffer[6000];
    char *record;
    char *line;

    //open read in stream
    char *path = "./data/";
    char str[20];
    strcpy(str, path);
    strcat(str, fileName);
    FILE *fstream = fopen(str, "r");

    if(fstream == NULL)
    {
        printf("File opening failed!\n");
        exit(0);
    }

    char *ptr = "C:";
    int index = 0;
    while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
    {
        record = strtok(line," ");
        if (!strcmp(ptr, record))
        {
            while(record != NULL)
            {
                record = strtok(NULL, " ");
                if (record != NULL && *record != '\n')
                {
                    *(arr+index) = atoi(record);
                    index++;
                }
            }
        }
    }
    fclose(fstream);
}

int dcjDistance(node **doublyLinkedListA, node **doublyLinkedListB, int *listNumberA, int *listNumberB, int n){

    /*calculate the dcj distance between two genomes*/
    
    int startValue = 0;
    int aaComponent = 0;
    int abComponent = 0;
    int circle = 0;
    int flag = -1;
    int listbReturnValue = -1;
    int listaReturnValue = -1;
    while (startValue != -1)
    {
        flag = -1;
        startValue = lookupStartValue(doublyLinkedListA, listNumberA);
        if (startValue != -1)
        {
            listaReturnValue = startValue;

            while (flag == -1)
            {   
                listbReturnValue = lookupNode(doublyLinkedListB, listNumberB, listaReturnValue);
                flag = checkIfStop(doublyLinkedListB, listNumberB, listbReturnValue, startValue);
                if (flag == 2){
                    abComponent++;
                }else{
                    listaReturnValue = lookupNode(doublyLinkedListA, listNumberA, listbReturnValue);
                    flag = checkIfStop(doublyLinkedListA, listNumberA, listaReturnValue, startValue);
                    if (flag == 1){
                        aaComponent++;
                    }else if(flag == 3){
                        circle++;
                    }
                }
            }
        }
    }

    int distance = 0;
    distance = n - circle - abComponent/2;
    return distance;
}

int lookupStartValue(node **doublyLinkedList, int *listNumber){
    /*calculate the dcj distance between two genomes*/
    //get the start point
    node *startPtr = (node *) malloc(sizeof(node));
    int startValue = -1;
    //int startGenomeTag = 0;
    int j = 0;
    while (j < (*listNumber)){
        node *tempPtr = (node *) malloc(sizeof(node));
        tempPtr = doublyLinkedList[j];
        if (tempPtr != NULL)
        {
            if (checkLinkedListType(tempPtr) == 1)
            {
                while (tempPtr->status == both && tempPtr->next != NULL){
                //printf("*************************************\n");
                tempPtr = tempPtr->next;
                }
            }
            else
            {
                node *head = tempPtr;
                do
                {
                    tempPtr = tempPtr->next;
                }while (tempPtr->status == both && tempPtr != head);
            }
            //printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            if (tempPtr ->status != both){
                startPtr = tempPtr;
                //startGenomeTag = startPtr->gTag;
                break;
            }
        }
        j++;
    }

    if ( startPtr->nType == thNode){
        if (startPtr->fvalue == HEAD || startPtr->fvalue == TAIL){
            startValue = startPtr->svalue;
        }else{
            startValue = startPtr->fvalue;
        }
        startPtr->status = both;
    }else {
        if (startPtr->status == none){
            startValue = startPtr->fvalue;
            startPtr->status = left;
        }else if (startPtr->status == left){
            startValue = startPtr->svalue;
            startPtr->status = both;
        }else if (startPtr->status == right){
            startValue = startPtr->fvalue;
            startPtr->status = both;
        }
    }
    return startValue;
}

int checkIfStop(node **doublyLinkedList, int *listNumber, int getValue, int startValue){
    int j = 0;
    int flag = -1;
    while (j < (*listNumber))
    {
        node *tempPtr = (node *) malloc(sizeof(node));
        tempPtr = doublyLinkedList[j];
        if (tempPtr != NULL)
        {
            if (tempPtr->gTag == firstGenome)
            {
                if (checkLinkedListType(tempPtr) == 1)
                {
                    while (!((tempPtr->fvalue == getValue)||(tempPtr->svalue == getValue)) && tempPtr->next != NULL)
                    {
                        tempPtr = tempPtr->next;
                    }
                }
                else
                {
                    node *t_node = tempPtr;
                    do
                    {
                        tempPtr = tempPtr->next;
                    }while(!((tempPtr->fvalue == getValue)||(tempPtr->svalue == getValue)) && tempPtr != t_node);
                }

                if ((tempPtr->fvalue == getValue)||(tempPtr->svalue == getValue))
                {
                    
                    if (tempPtr->nType == thNode)
                    {
                        tempPtr->status = both;
                        //aa
                        flag = 1;
                    }
                    // else if (startValue == getValue)
                    // {
                    //  flag = 3;
                    // }
                    else if (tempPtr->fvalue == getValue)
                    {
                        if (tempPtr->fvalue == startValue)
                        {
                            //circle
                            flag = 3;
                        }
                    }
                    else if (tempPtr->svalue == getValue)
                    {
                        if (tempPtr->svalue == startValue)
                        {
                            //tempPtr->status = both;
                            //circle
                            flag = 3;
                        }
                    }
                    break;
                }
            }
            else
            {
                while (!((tempPtr->fvalue == getValue)||(tempPtr->svalue == getValue)) && tempPtr->next != NULL)
                {
                    tempPtr = tempPtr->next;
                }

                if ((tempPtr->fvalue == getValue)||(tempPtr->svalue == getValue))
                {
                    if (tempPtr->nType == thNode){
                        tempPtr->status = both;
                        //ab
                        flag = 2;
                    }
                }
            }
        }
        j++;
    }
    return flag;
}

int lookupNode(node **doublyLinkedList, int *listNumber, int startValue){
    int resultValue = -1;
    int j = 0;
    while (j<(*listNumber))
    {
        node *tempNode = (node *) malloc(sizeof(node));
        tempNode = doublyLinkedList[j];

        if (tempNode != NULL)
        {
            if (checkLinkedListType(tempNode) == 1)
            {
                while (!((tempNode->fvalue == startValue)||(tempNode->svalue == startValue)) && tempNode->next != NULL)
                {
                    tempNode = tempNode->next;
                }
            }
            else
            {
                node *temp_head = tempNode;
                do
                {
                    tempNode = tempNode->next;
                }while (!((tempNode->fvalue == startValue)||(tempNode->svalue == startValue)) && tempNode != temp_head);
            }

            if ((tempNode->fvalue == startValue)||(tempNode->svalue == startValue))
            {
                if (tempNode->fvalue == startValue)
                {
                    resultValue = tempNode->svalue;
                }
                else
                {
                    resultValue = tempNode->fvalue;
                }
                tempNode->status = both;
                break;
            }
        }   
        j++;
    }
    //printf("resultValue: %d\n", resultValue);
    //printf("------------------------------------------\n");
    return resultValue;
}

int checkLinkedListType(node *doublyLinkedList){
    node *temp = (node *) malloc(sizeof(node));
    temp = doublyLinkedList;
    int llType = 0;
    // if (temp != NULL)
    // {
    //  printf("NULL\n");
    //  printf("ntype: %d\n", temp->fvalue);
    // }
    if (temp->nType == thNode)
    {
        llType = 1;
    }
    return llType;
}

void initialGenome(int *arrayList, node **head){
    for (int i=0; i<=OrignalLength; i++){
        //printf("----%d-----%d\n", arrayList[i*2], arrayList[i*2+1]);
        appendDLL(head, arrayList[i*2], arrayList[i*2+1]);
    }
}

void appendDLL(node **head_ref, int fvalue, int svalue){
    /* 1. allocate node */
    node* new_node = (node*) malloc(sizeof(node));
 
    node *last = *head_ref;  /* used in step 5*/
 
    /* 2. put in the data  */
    new_node->fvalue = fvalue;
    new_node->svalue = svalue;
    new_node->status = none;
 
    /* 3. This new node is going to be the last node, so 
          make next of it as NULL*/
    new_node->next = NULL;
 
    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL)
    {
        new_node->prev = NULL; 
        *head_ref = new_node;
        return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
 
    /* 6. Change the next of last node */
    last->next = new_node;
 
    /* 7. Make last node as previous of new node */
    new_node->prev = last;
 
    return;
}

void initialData(int *genomeList, int *listToGenome){
    node *head = (node*) malloc(sizeof(node));
    if (genomeList == NULL){
        printf("input data is null\n");
        return;
    }

    int genomeLength = OrignalLength*2 + 2;

    int doubleAry[OrignalLength*2] = {0};

    for (int i=0; i<OrignalLength; i++){
        if (genomeList[i] > 0){
            doubleAry[2*i] = genomeList[i]*2 - 1;
            doubleAry[2*i+1] = genomeList[i]*2;
        }else{
            doubleAry[2*i] = genomeList[i]*(-1)*2;
            doubleAry[2*i+1] = genomeList[i]*2*(-1) - 1;
        }
    }

    for (int i=0; i<OrignalLength*2; i++){
        listToGenome[i+1] = doubleAry[i];
    } 

    //static int genome[OrignalLength*2 + 2] = {0};
    //for (int i=0; i<OrignalLength*2; i++){
    //  genome[i+1] = doubleAry[i];
    //}

    listToGenome[0] = HEAD;
    listToGenome[genomeLength-1] = TAIL;

    return;
} 

void initialDoubleLinkedList(node **doublyLinkedList,  int *listNumber, int genomeNum)
{

    /* set nodeType and genomeTag to specific value for the first genome*/

    for (int i=0; i<(*listNumber); i++){
        node *ptr = (node *) malloc(sizeof(node));

        ptr = doublyLinkedList[i];

        if (ptr == NULL){
            return;
        }

        
        while (ptr != NULL){
            if (ptr->fvalue == HEAD || ptr->svalue == HEAD || ptr->fvalue == TAIL || ptr->svalue == TAIL){
                ptr->nType = thNode;
            }else{
                ptr->nType = generalNode;
            }
            if (genomeNum == 1)
            {
                ptr->gTag = firstGenome;
            }
            else
            {
                ptr->gTag = secondGenome;
            }
            ptr = ptr->next;
        }
    }
}

int doCalculation(int a, int b)
{
    char *fileName = ".genome";

    int *genomeA = (int *)malloc(GENESIZE*sizeof(int));
    char str_a[20];
    snprintf(str_a, 20, "%d", a);
    char genomeA_Name[20];
    strcpy(genomeA_Name, str_a);
    strcat(genomeA_Name, fileName);
    readDataFromFile(genomeA, genomeA_Name);

    int *genomeB = (int *)malloc(GENESIZE*sizeof(int));
    char str_b[20];
    snprintf(str_b, 20, "%d", b);
    char genomeB_Name[20];
    strcpy(genomeB_Name, str_b);
    strcat(genomeB_Name, fileName);
    readDataFromFile(genomeB, genomeB_Name);

    node** genomeArrayA = (node **)calloc(1, sizeof(node*));
    node** genomeArrayB = (node **)calloc(1, sizeof(node*));

    node* headA = NULL;
    node* headB = NULL;

    int returnListA[OrignalLength*2+2] = {0};
    int returnListB[OrignalLength*2+2] = {0};

    initialData(genomeA, returnListA);
    initialData(genomeB, returnListB);

    initialGenome(returnListA, &headA);
    initialGenome(returnListB, &headB);

    //allocate memory for 1st genome 
    if (genomeArrayA != NULL)
    {
        genomeArrayA[0] = NULL;
        genomeArrayA[0] = (node *)calloc(1, sizeof(node));
    }

    //alocate memory for 2nd genome
    if (genomeArrayB != NULL)
    {
        genomeArrayB[0] = NULL;
        genomeArrayB[0] = (node *)calloc(1, sizeof(node));
    }

    genomeArrayA[0] = headA;
    genomeArrayB[0] = headB;

    int listNumberA = 1;
    int listNumberB = 1;

    initialDoubleLinkedList(genomeArrayA, &listNumberA, 1);
    initialDoubleLinkedList(genomeArrayB, &listNumberB, 1);

    int distance = 0;
    distance = dcjDistance(genomeArrayA, genomeArrayB, &listNumberA, &listNumberB, OrignalLength);
    //printf("Distance: %d\n", distance);

    free(genomeA);
    free(genomeB);

    return distance;
}

int main(){

    double start, end;

    int        comm_sz;               /* Number of processes    */
    int        my_rank;               /* My process rank        */

    /* Start up MPI */
    MPI_Init(NULL, NULL);

    /* Get the number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* Get my rank among all the processes */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //set start time
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    //store all indices pairs into array
    int calculation_num = (GENOMENUMBER*(GENOMENUMBER-1))/2;
    //printf("cal: %d\n", calculation_num);
    int **pair_indices = (int **)malloc(calculation_num*sizeof(int*));
    int **ptr = pair_indices;

    //printf("****************************************************\n");

    /*  
    if (comm_sz == 0)
    {
        printf("commsz is 0\n");
        exit(0);
    }
    */


    for (int i=0; i<GENOMENUMBER; i++)
    {
        for (int j=i+1; j<GENOMENUMBER; j++)
        {
            int *temp_indices = (int*)malloc(sizeof(int));
            temp_indices[0] = i+1;
            temp_indices[1] = j+1;
            *pair_indices = temp_indices;
            pair_indices++;
        }

    }

    /*
    //print out
    for (int i=0; i<calculation_num; i++)
    {
        int *temp_indices;
        temp_indices = *ptr;
        ptr++;
        printf("index: %d, %d\n", temp_indices[0], temp_indices[1]);
    }
    */

    int local_n = calculation_num/comm_sz ;
    //local_a = my_rank*local_n;
    //local_b = local_a + local_n;

    int index_s = my_rank*local_n; //start point
    int index_e = index_s + local_n; //end point
    int *local_result = (int *)malloc(local_n*sizeof(int));
    int *global_result = (int *)malloc(calculation_num*sizeof(int));
    int *ptr_global_result = global_result;

    if (my_rank != 0)
    {
        int j=0;
        for(int i=index_s; i<index_e; i++)
        {
            int *temp_indices = (int*)malloc(sizeof(int));
            temp_indices = *(ptr+i);
            int a = temp_indices[0];
            int b = temp_indices[1];
            local_result[j] = doCalculation(a, b);
            j++;
        }
        MPI_Send(local_result, local_n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        //do calculation
        //send it out to rank = 0
    }
    else
    {
        //printf("inside rank equal 0\n");
        //printf("size of global  %d\n", sizeof(global_result));
        //proecessor 0 calculation
        for (int i=0; i<local_n; i++)
        {
            int *temp_indices = (int*)malloc(sizeof(int));
            temp_indices = *(ptr+i);
            //printf("temp 0 %d, temp 1 %d\n", temp_indices[0], temp_indices[1]);
            int distance = doCalculation(temp_indices[0], temp_indices[1]);
            *(global_result+i) = distance;
            //printf("!!!!!!!!!!!!!!!!!\n");
        }

        for (int i=1; i<comm_sz; i++)
        {
            MPI_Recv(local_result, local_n, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j=0; j<local_n; j++)
            {
                *(global_result+i*local_n+j) = local_result[j];
            }
        }
    }

    if (my_rank == 0)
    {
        //print out the results
        /*
        for (int i=0; i<GENOMENUMBER; i++)
        {
            int start_index = 0;
            for (int k=0; k<=i; k++)
                printf("0 ");
            for (int j=0; j<GENOMENUMBER-i-1; j++)
                printf("%d ", *(global_result+start_index+j));
            printf("\n");
            start_index += GENOMENUMBER - i -1;
        }*/
        for (int i=0; i<calculation_num; i++)
        {
            printf("%d ", *(global_result+i));
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    /* Shut down MPI */
    MPI_Finalize();

    //t = clock() - t;
    //double time_taken = ((double)t)/CLOCKS_PER_SEC;
    //printf("time: %f\n", time_taken);
    if (my_rank == 0)
    {
        printf("Running time = %f\n", end-start);
    }

    return 0;
}   