#ifndef MPI_PROJECT_H_
#define MPI_PROJECT_H_

#define HEAD 0
#define TAIL 10000
#define OrignalLength 1000
#define GENESIZE 1000
#define GENOMENUMBER 100

typedef enum { false=0, true=1} bool;

typedef enum { left=1, right=2, both=3, none=4} visited;

typedef enum { generalNode=0, thNode=1} nodeType;

typedef enum { firstGenome=1, secondGenome=2} genomeTag;

typedef struct node
{
	int fvalue;
	int svalue;
	struct node* next;
	struct node* prev;
	visited status;
	nodeType nType;
	genomeTag gTag;
} node;

/******function define****************/
void readDataFromFile(int *arr, char *fileName);
int lookupStartValue(node **doublyLinkedList, int *listNumber);
int dcjDistance(node **doublyLinkedListA, node **doublyLinkedListB, int *listNumberA, int *listNumberB, int n);
void initialData(int *genomeList, int *listToGenome);
void appendDLL(node **head_ref, int fvalue, int svalue);
void initialGenome(int *arrayList, node **head);
int lookupNode(node **doublyLinkedList, int *listNumber, int startValue);
int checkIfStop(node **doublyLinkedList, int *listNumber, int getValue, int startValue);
int checkLinkedListType(node *doublyLinkedList);
void initialDoubleLinkedList(node **doublyLinkedList,  int *listNumber, int genomeNum);
int doCalculation(int a, int b);

#endif // MPI_PROJECT_H_