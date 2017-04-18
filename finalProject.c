#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define HEAD 0
#define TAIL 10000
#define OrignalLength 1000
#define T 1250
#define NODENUM 2000

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



/******************************dcj function definition***************************/
int lookupStartValue(node **doublyLinkedList, int *listNumber);
int dcjDistance(node **doublyLinkedListA, node **doublyLinkedListB, int *listNumberA, int *listNumberB, int n);
void initialData(int *genomeList, int *listToGenome);
void appendDLL(node **head_ref, int fvalue, int svalue);
void initialGenome(int *arrayList, node **head);
int lookupNode(node **doublyLinkedList, int *listNumber, int startValue);
int checkIfStop(node **doublyLinkedList, int *listNumber, int getValue, int startValue);
int checkLinkedListType(node *doublyLinkedList);
void initialDoubleLinkedList(node **doublyLinkedList,  int *listNumber, int genomeNum);

/*****************************function implementation********************/
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
					// 	flag = 3;
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
	// 	printf("NULL\n");
	// 	printf("ntype: %d\n", temp->fvalue);
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
	//	genome[i+1] = doubleAry[i];
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

int main()
{
	int genomeA[OrignalLength] = {-650,-202,-723,107,-344,-356,-726,-725,109,-153,-892,309,-597,-585,-584,-518,-714,-20,-456,-553,856,-953,21,681,-41,-374,-373,-127,-549,-330,152,721,-68,410,-329,494,-659,-855,-495,85,-289,965,-849,-457,-19,-354,-935,-211,-210,28,86,-550,-199,-599,-493,722,713,209,-369,87,-337,-293,208,355,538,-819,-818,510,-919,27,598,-701,-277,-54,-580,732,-353,807,148,649,936,547,548,420,421,422,-432,794,-254,436,-147,-857,-394,916,194,-608,-511,-930,411,412,-773,-22,546,357,858,696,697,-582,393,-486,-671,310,620,-793,772,-663,-581,-648,331,-742,-294,-336,-243,-609,884,372,-907,-672,897,386,283,284,770,771,77,683,684,-778,-570,-250,-249,268,-888,780,-822,-317,-781,-741,326,327,519,-158,739,-91,-90,-227,558,-720,-6,-460,646,-43,137,126,-920,915,-783,779,539,540,541,542,-446,161,-196,166,-162,-111,544,898,-552,-437,447,-710,-444,181,-110,923,924,-5,-430,255,569,-728,429,4,138,651,-31,-543,139,140,141,-382,-996,328,941,98,612,-459,-606,-963,-448,-715,128,318,-886,-445,381,-296,-295,508,964,99,-534,-299,-571,-136,814,-607,887,157,-990,226,198,733,734,-403,617,-628,559,380,-379,-428,-378,-151,142,820,962,414,212,692,-119,-118,-117,-116,729,730,731,413,-225,896,997,555,-932,-251,-782,143,942,-533,709,247,248,-784,-727,943,944,635,777,689,499,-991,-36,-35,-160,300,647,583,828,-899,927,-288,-290,-705,449,704,246,275,276,-458,988,-30,135,-305,44,586,-308,-184,-823,-65,-824,706,307,187,83,-433,69,984,669,595,-564,502,594,-185,368,982,9,-827,505,-367,-670,711,435,-914,84,-933,450,-434,72,842,-287,-979,-92,-239,588,-274,-94,261,928,-97,-241,-641,-891,-987,223,989,-306,-860,587,252,-487,900,901,240,-93,-601,-929,-17,-186,-64,712,848,-71,556,-45,253,123,-319,-918,156,-625,644,193,-312,488,224,-753,-913,-754,173,873,-396,668,-512,-874,-80,174,688,-854,707,-665,-762,-761,-940,262,693,183,-474,851,177,-391,-163,-666,-917,304,-78,-911,850,-409,992,-390,169,878,-737,-736,760,667,176,853,-630,489,513,639,-735,-168,-626,-201,912,-16,-167,-389,334,408,473,-611,-756,-627,15,-407,-759,890,-852,-610,-676,-431,910,-79,-504,-503,-931,-175,178,841,-2,-1,-400,-634,-758,-10,23,-791,-835,-834,-682,-934,664,-426,-425,788,-985,-179,-776,-600,-613,-12,-57,-399,-638,8,937,-267,-816,-796,-24,-938,100,-961,-960,-959,615,270,271,948,-346,619,-950,867,-206,26,-803,-1000,-840,496,-320,-358,949,-662,-25,631,11,-839,105,285,661,-112,-790,-789,983,-122,-592,-273,-272,-263,673,-680,-679,-994,115,703,-535,-632,837,114,-806,302,-752,-537,200,836,-757,180,545,-637,121,-102,-509,485,-645,286,882,993,-802,-801,-958,-591,-567,-566,207,-7,-861,660,-817,113,-838,785,786,103,104,787,536,-481,-614,-716,947,264,526,527,-596,205,265,-702,995,618,-590,-577,-134,301,311,879,-345,42,999,885,13,-951,774,775,291,341,172,-976,-63,-530,-342,-740,-343,-975,719,-977,-340,-593,-303,516,-876,642,966,-970,694,-973,-171,967,969,124,66,-978,-52,-427,377,-708,-718,297,514,-18,-972,51,-70,-60,-515,-894,981,-859,500,61,50,815,392,843,-531,602,971,-170,724,-532,-298,551,-974,-674,517,-49,-875,-572,-695,-501,-529,636,384,385,-946,-865,-236,-750,-749,-748,257,-629,792,-352,767,768,-945,560,-438,498,418,419,-561,-799,-471,-470,-469,986,-557,120,467,-653,-864,-863,472,439,324,-48,743,-766,375,75,605,-189,47,228,-466,-798,-14,462,464,465,-604,-3,-440,751,562,-952,55,-81,-417,370,-769,-862,922,-416,-415,563,338,339,-88,190,191,231,-677,-46,53,-323,144,145,-497,-603,-164,-242,-192,463,-998,244,-188,-821,-388,398,889,800,795,-492,82,921,182,-233,165,-387,-149,383,-826,-825,230,325,-880,468,203,256,-866,-32,764,678,89,59,33,34,-229,146,150,397,-56,-658,-371,893,-195,106,461,232,76,652,765,579,395,-909,197,939,376,-578,155,-58,895,350,747,872,809,-832,218,-956,-955,-266,335,-154,565,868,698,-763,351,215,574,575,576,-525,-700,159,-67,656,657,633,221,222,521,-443,-442,405,-214,131,833,-624,234,523,38,39,483,484,-871,37,359,507,129,-870,-808,968,675,133,755,528,-589,-130,475,40,-360,406,258,881,-478,-477,-482,744,-640,-108,905,-404,690,-314,-366,-365,-364,-363,691,-622,-95,-844,738,-804,480,-453,-452,903,877,-347,506,-745,-451,29,-349,-348,-883,332,333,-906,-980,-74,846,-908,441,-454,-73,-520,269,259,810,621,-235,902,-219,-831,-830,573,869,-904,-686,-132,811,812,-805,522,-220,479,321,-238,476,-926,-925,96,568,-245,699,-957,-217,-237,-797,-655,213,204,524,281,282,-313,315,455,654,554,845,-847,-491,616,125,717,316,-687,-292,-101,322,685,362,216,-423,954,-280,-279,-278,-623,-361,829,260,746,490,-813,62,424,-643,-402,-401};
	int genomeB[OrignalLength] = {756,757,758,-610,-609,-608,815,-894,-893,-892,-891,-890,-889,-888,-887,-886,777,778,779,780,781,782,783,784,785,786,787,885,-776,-775,-774,-773,-772,-771,-770,-769,-768,-767,-766,-765,-764,-763,-762,-761,-760,-759,611,-805,-804,-803,-802,-801,-617,-616,-615,-614,-613,-612,806,795,796,797,798,799,800,601,602,603,604,605,606,607,-814,-813,-812,-811,-810,-809,-808,-807,-794,-793,-792,-791,-790,-789,-788,-884,-883,-882,878,879,880,881,-877,-876,-875,-874,-873,-872,-871,-870,-869,-868,-867,-866,-865,-864,-863,-862,-861,-860,-859,-858,-857,-856,-855,-854,-853,-852,-851,-850,-849,-848,-847,-846,-845,-844,-843,-842,-841,-840,-839,-838,-837,-836,-835,-834,-833,-832,-831,-830,-829,-828,-827,-826,-825,-824,-823,-822,-821,-820,-819,-818,-817,-816,-754,-753,-193,910,911,912,913,917,918,919,-196,-195,-194,-916,914,915,921,922,748,749,750,751,752,-920,197,-146,-145,-144,-895,755,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,75,76,77,78,79,170,171,172,173,174,175,176,177,178,179,180,181,182,183,902,903,904,905,906,907,908,909,-192,-191,-190,-189,-188,88,89,90,91,-739,-738,-737,-736,-735,-734,-733,-732,-731,-730,-729,-728,-727,-726,-725,-724,-723,-722,-721,-720,-719,-718,-717,-716,-715,-714,-713,-712,-711,-710,-709,-708,-707,-706,-705,-704,-703,-702,-701,-700,-699,-698,-697,-696,-695,-694,-693,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,-672,-671,-670,-669,-668,-667,-666,-665,-664,-663,-662,-661,-660,-659,-658,-657,-656,-655,-654,-653,-652,-325,-324,-323,-322,-321,-320,-319,-318,-317,-316,-315,-314,-313,-312,-311,-310,-309,-308,-307,-306,-305,-304,-303,-302,-301,-300,-299,-298,-297,-296,-295,-294,-293,-292,-291,-290,-289,-288,-287,-286,-285,-576,-575,-574,-573,-131,-130,133,134,-555,-554,-553,-552,-551,499,500,501,502,503,504,505,-97,-96,-95,-94,-93,-92,740,120,121,122,123,124,125,126,127,128,129,-132,-572,-571,-570,-569,-568,-567,-566,-565,-564,-563,-562,-561,-560,-559,-558,-557,-556,135,136,137,138,139,140,141,142,143,896,897,898,899,900,901,184,185,186,187,-87,-86,-85,-84,-83,-82,-81,-80,-169,-168,-167,-166,-165,-164,-163,-162,-161,-160,-159,-158,-157,-156,-155,-154,-153,-152,-151,-150,-149,-148,-147,198,199,200,940,-34,-33,-32,-31,-30,-357,-356,-355,-354,-353,-352,-351,-350,-349,-348,-347,-346,340,341,342,343,344,345,-339,-338,-337,-336,-335,-334,-333,-332,-331,-330,-329,-328,-327,-326,-651,-650,-649,-648,-647,-646,-645,-644,-643,-642,-641,-640,-639,-638,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,962,963,964,965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,-391,-390,-389,-961,-960,-959,-958,-957,-956,-955,-954,423,424,425,426,427,428,429,430,431,432,433,434,435,436,-599,593,594,595,596,597,598,-592,-591,-590,486,487,488,489,490,491,492,493,494,495,496,-485,-484,-483,-482,-481,-480,-479,-478,-477,-476,-475,-474,-473,-472,-471,-470,-469,-468,-467,-466,-465,-464,-463,-462,-461,-460,-459,-458,-457,-456,-455,-454,-453,-452,-451,-450,-449,-448,-447,-446,-445,-444,-443,-442,-441,-440,-439,-438,-437,600,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,-7,-6,-5,-4,-3,-2,-1,-400,-399,-398,-397,-396,-395,-394,-393,-392,993,994,995,996,997,998,999,1000,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,-953,-952,-951,-950,-949,-948,-947,-946,-945,-944,-943,-942,-941,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,67,68,69,-65,-64,-72,-71,73,74,-62,-61,-60,-66,70,-63,-511,-510,-101,-100,114,-98,506,507,508,509,102,103,104,105,115,-113,-112,-111,-110,923,924,-741,-119,-118,-117,-116,-99,106,-744,108,-109,-747,-746,-745,107,-743,-742,201,202,203,204,524,525,526,527,246,247,248,249,250,251,283,284,577,578,579,580,209,210,211,212,213,-280,-279,-278,-277,-276,-275,-274,-273,-272,-271,-270,-269,-268,-267,-266,-265,-264,-263,-262,-261,281,282,252,253,-548,-547,-546,-545,-544,-543,-542,-541,-540,-539,-538,-537,-536,-535,-534,-533,-532,-531,-530,-529,-528,-245,-244,-243,-242,-241,-240,-239,-238,-237,-236,-235,223,224,225,226,227,228,229,230,231,232,233,234,-222,-221,-220,-219,-218,-217,-216,-215,-214,-260,-259,-258,-257,-256,-255,-254,549,550,-498,-497,-589,-588,-587,-586,-585,-584,-583,-582,-581,-208,-207,-206,-205,-523,521,522,-520,-519,-518,-517,-516,-515,-514,-513,-512};
	
	node* headA = NULL;
	node* headB = NULL;

	int returnListA[OrignalLength*2+2] = {0};
	int returnListB[OrignalLength*2+2] = {0};

	initialData(genomeA, returnListA);
	initialData(genomeB, returnListB);

	initialGenome(returnListA, &headA);
	initialGenome(returnListB, &headB);

	node** genomeArrayA = (node **)calloc(1, sizeof(node*));
	node** genomeArrayB = (node **)calloc(1, sizeof(node*));

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

	//initial true genome
	initialDoubleLinkedList(genomeArrayA, &listNumberA, 1);
	initialDoubleLinkedList(genomeArrayB, &listNumberB, 1);

	int distance = 0;
	distance = dcjDistance(genomeArrayA, genomeArrayB, &listNumberA, &listNumberB, OrignalLength);
	printf("Distance: %d\n", distance);
	return 0;
}
