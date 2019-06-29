#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>

// List element for Dictionary lists.
typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	char *key;
	char *value;
	int frequency;
} ListNode;

// Dictionary structure that includes the lists of elements and their number.
typedef struct Dictionary {
	ListNode **lists;		// lists of elements.
	int N;					// number of lists.
} Dictionary;


// Initializes an empty Dictionary structure.
Dictionary* createDictionary(int N);

// Adds an element to the Dictionary structure.
void addElement(Dictionary *dictionary, char *key, char *value, int frequency);

// Removes an element from the Dictionary structure.
void removeElement(Dictionary *dictionary, char *key, char *value);

// Prints all the elements from all the lists of the Dictionary structure.
void printDictionary(FILE *f, Dictionary *dictionary);

// Gets all the elements with the specified key and increments the frequencies.
ListNode* get(Dictionary *dictionary, char *key);

// Prints all the elements with the specified value.
void printValue(FILE *f, Dictionary *dictionary , char *value);

// Prints all the elements with the specified frequency.
void printFrequency(FILE *f, Dictionary *dictionary , int frequency);

// Returns a list containing the elements with the specified value.
ListNode* unionValues(Dictionary *dictionary, char *value);

// Returns a list containing the elements with maximum frequency in each list.
ListNode* unionMaxFrequencies(Dictionary *dictionary);

// Returns a new Dictionary with reversed lists of the input structure.
Dictionary* reverseLists(Dictionary *dictionary);

// Prints a double-linked non-circular list.
void printList(FILE *f, ListNode *list);

// Frees all the memory allocated for the Dictionary.
void freeDictionary(Dictionary *dictionary);

// Frees all the memory allocated for a double-linked non-circular list.
void freeList(ListNode *list);

//------------------------------------------------------------------------------

Dictionary* createDictionary(int N) {
	// TODO
	Dictionary* dex;
	dex=malloc(sizeof (Dictionary));
	dex->N=N;
	dex->lists=malloc(N*sizeof(ListNode*));
	for(int i=0 ; i<N;i++)
		dex->lists[i]=NULL;
	return dex;
	//return NULL;
}

int calculateR(int N,char* key){
    int s=0,r=0;
    for(int i = 0 ; i< strlen(key);i++){
        s=s+key[i];
    }
	r=s%N;
    return r;
}
int compareNodes(ListNode* b,ListNode* a){
    if(strcmp(a->value , b->value)==0 && strcmp(a->key , b->key)==0){
        //printf("compareNodes 0 %s %s\n", a->value, a->key);
        return 0;
    }
    if(a->frequency == b->frequency){
        if(strcmp(a->value , b->value)==0){
            return strcmp(a->key,b->key);

        }
        return strcmp(a->value , b->value);
    }
    return (a->frequency > b->frequency ? -1:1);
    /*if(a->frequency > b->frequency)
    return 1 ;
    else
    return -1;*/
}
int length(ListNode *aList){
    if(aList == NULL )
        return 0;
    int result=1;
    ListNode *p=aList;
    while(p->next != aList ){
        p=p->next;
        result++;
    }
    return result;

}

void addElement2(Dictionary *dictionary, char *key, char *value, int frequency) {
    ListNode *newElement;
    newElement=malloc(sizeof(ListNode));
    //newElement->key=key;
    //newElement->value=value;



    newElement->value = malloc(sizeof(char) * (strlen(value) + 1));
    newElement->key = malloc(sizeof(char) * (strlen(key) + 1));

    strcpy(newElement->value, value);
    strcpy(newElement->key, key);


    newElement->frequency=frequency;
    newElement->next=NULL;
    newElement->prev=NULL;
    int r=calculateR(dictionary->N,key);
    ListNode *startNode=dictionary->lists[r];
    ListNode *currentNode=startNode;
    if(startNode == NULL){
        dictionary->lists[r]=newElement;
        newElement->next=newElement;
        newElement->prev=newElement;
        printf("return1\n");
        return;
    }



    ListNode *p = startNode;


    int oldFrequency = -1;
    do {

        if(strcmp(p->value, value) ==0 && strcmp(p->key, key) == 0) {
            oldFrequency = p->frequency;
        }

        p = p->next;
    }while(p!= startNode);
        printf("Aici %d \n",oldFrequency);
    if(oldFrequency != -1) {
         removeElement(dictionary, key, value);
		 addElement2(dictionary, key, value, frequency + oldFrequency);
		 return;
         //frequency = frequency + oldFrequency;
         //newElement->frequency = frequency;

         //printf("f:%d \n", frequency);
    }



    ListNode *lastNode=startNode->prev;
    int cmpStart;
    cmpStart=compareNodes(newElement,startNode);
//insert in first position
    if(cmpStart > 0){
        newElement->next=startNode;
        newElement->prev=lastNode;
        dictionary->lists[r]=newElement;
        startNode->prev=newElement;
        lastNode->next=newElement;
        printf("return2\n");
        return;
    }
    if(cmpStart == 0){
        startNode->frequency+=frequency;
		printf("return2.5\n");
        return;
    }
//insert in last position
    int cmpLast;
    cmpLast=compareNodes(newElement,lastNode);
    printf("cmpLast: %d\n", cmpLast);
    if(cmpLast < 0){
        newElement->next=startNode;
        newElement->prev=lastNode;
        startNode->prev=newElement;
        lastNode->next=newElement;
        printf("return3\n");
        return;
    }
    if(cmpLast == 0){
        lastNode->frequency+=frequency;
        printf("return4\n");
        return;
    }
    int cmp;
    while(currentNode->next != startNode ){
        cmp=compareNodes(newElement,currentNode);
        if (cmp == 0){
            currentNode->frequency+=frequency;
            free(newElement);
            printf("return5\n");
            return;
        }
        ListNode *nextNode=currentNode->next;
        int cmpNext=compareNodes(newElement,currentNode->next);
        if(cmp > 0 && cmpNext < 0){
            newElement->next=nextNode;
            newElement->prev=currentNode;
            currentNode->next=newElement;
            nextNode->prev=newElement;
            printf("return6\n");
            return;
        }
        currentNode=currentNode->next;

    }


    
	printf("return7\n");
	return;
}

void removeLastElement(Dictionary *dictionary,int indexList){
    ListNode *startNode=dictionary->lists[indexList];
    if(startNode==NULL)
        return;
    if(startNode->next == startNode){
        dictionary->lists[indexList]=NULL;
        return;
        //free(startNode);
    }
    ListNode *lastNode=startNode->prev;
    ListNode *beforeLastNode=lastNode->prev;
    beforeLastNode->next=startNode;
    startNode->prev=beforeLastNode;
    //free lastNode;
}
void addElement(Dictionary *dictionary, char *key, char *value, int frequency){
    //printf("here add Element0\n");


    int r=calculateR(dictionary->N,key);
    //printf("here add Element0.5 %d\n", r);
    //printf("here add Element0.75 %d\n", dictionary->N);
    //printf("abc %d\n",(int) (dictionary->lists[r] == NULL));
    //printf("abc %d\n",(dictionary->lists[r]->frequency));


    if(length(dictionary->lists[r]) == dictionary->N){
        removeLastElement(dictionary,r);

        //printf("length11: %d\n", length(dictionary->lists[r]));
    }


    //printf("here add Element1\n");


    addElement2(dictionary,key,value,frequency);

/*
    printf("here add Element2\n");


    int r=calculateR(dictionary->N,key);
    if(length(dictionary->lists[r]) == dictionary->N+1){
        removeLastElement(dictionary,r);

        printf("length11: %d\n", length(dictionary->lists[r]));
    }
    */

    //verificam N and 2*N

    //printf("length22: %d\n", length(dictionary->lists[r]));




    int sumLength=0;
    for(int i=0 ; i < dictionary->N ; i++){
        sumLength+=length(dictionary->lists[i]);
    }
    if(sumLength == 2*dictionary->N+1){
        for(int i=0 ; i < dictionary->N ; i++){
            removeLastElement(dictionary,i);
        }
    }



}

ListNode *copyListNode(ListNode *node){
    ListNode *newNode;
    newNode=malloc(sizeof(ListNode));
    newNode->frequency=node->frequency;

    newNode->value = malloc(sizeof(char) * (strlen(node->value) + 1));
    newNode->key = malloc(sizeof(char) * (strlen(node->key) + 1));

    strcpy(newNode->value,node->value);
    strcpy(newNode->key,node->key);
    newNode->next=NULL;
    newNode->prev=NULL;
    return newNode;
}

/*
void removeElement(Dictionary *dictionary, char *key, char *value) {
	int r = calculateR(dictionary->N, key);
	ListNode *startNode = dictionary->lists[r];
	if(startNode == NULL) {
		return;
	}
	
}
*/

void removeElement(Dictionary *dictionary, char *key, char *value) {
	// TODO
	//printf("remove element start\n");
	int r=calculateR(dictionary->N,key);
	ListNode *startNode=dictionary->lists[r];
	if(startNode == NULL)
            return;
    ListNode *lastNode=startNode->prev;
    ListNode *currentNode=startNode;
    ListNode *p;
    while(currentNode != lastNode){
        // printf("while\n");
        if(strcmp(currentNode->key, key)==0 && strcmp(currentNode->value,value)==0){
            if(currentNode == startNode){
                if(currentNode->next==currentNode && currentNode->prev==currentNode){
                    dictionary->lists[r]=NULL;
                    free(currentNode);
                    return;
                }
                dictionary->lists[r]=startNode->next;
                lastNode->next=startNode->next;
                startNode->next->prev=lastNode;
                free(currentNode);
                return;
            }
            else{
                p=currentNode;
                currentNode->prev->next=currentNode->next;
                currentNode->next->prev=currentNode->prev;
                free(p);
                return;
            }



        }
        //printf("next\n");
        currentNode=currentNode->next;
    }
    if(strcmp(lastNode->key,key)==0 && strcmp(lastNode->value,value)==0){
		if(lastNode->next == lastNode) {
			//free(lastNode);
			dictionary->lists[r] = NULL;
		}
		
		
        p=lastNode;
        lastNode->prev->next=startNode;
        startNode->prev=lastNode->prev;
        free(p);
        return;

    }
   return;
}



/*void removeElement(Dictionary *dictionary, char *key, char *value) {
	// TODO
	return;
}*/
void printCircularList(FILE *f, ListNode *aList){
    ListNode *p=aList;
    do{
        fprintf(f,"(%s, %s, %d) ",p->key,p->value,p->frequency);
        printf("(%s, %s, %d) ",p->key,p->value,p->frequency);

        p=p->next;
    }while(p!=aList);
}
void printDictionary(FILE *f, Dictionary *dictionary) {
	// TODO
	for(int i=0 ; i < dictionary->N ;i++){
		fprintf(f,"List %d: ",i);

		printf("List %d: ",i);

		if(dictionary->lists[i] != NULL){
			printCircularList(f,dictionary->lists[i]);
		}
		fprintf(f,"\n");
		printf("\n");
	}

	return;
}

void sort(Dictionary* dictionary, int index) {
	ListNode* startNode = dictionary->lists[index];
	
	if(startNode == NULL) {
		return;
	}
	
	if(startNode->next == startNode) {
		return;
	}
	
	ListNode* lastNode = startNode->prev;
	
	//removeElement(dictionary, lastNode->key, lastNode->value);
	addElement2(dictionary, lastNode->key, lastNode->value, lastNode->frequency);
}

ListNode* get(Dictionary *dictionary, char *key) {
	// TODO
	//printf("start get\n");
	int r=calculateR(dictionary->N,key);
	ListNode *newListDouble = NULL;
	ListNode *startNode=dictionary->lists[r];
	if(startNode == NULL)
        return NULL;
	ListNode *lastNode=startNode->prev;
	ListNode *p=startNode;

	ListNode *newNode;
	ListNode *prevNode;
	
	//char table[100][100];
	//int nr = 0;
	
	int position = 0;
	
    //printf("before do\n");
    do{
		position++;
		int changed = 0;
        if(strcmp(p->key,key)==0){
            newNode=copyListNode(p);
			p->frequency++;
			
			
			ListNode * before = p->prev;
			
			
			
			/*
			else if(p->prev->frequency < p->frequency ){
				ListNode *p1 = p->prev->prev;
				ListNode *p2 = p->prev;
				ListNode *p3 = p;
				ListNode *p4 = p->next;
				
				p1->next = p3;
				p3->next = p2;
				p2->next = p4;
				
				p4->prev = p2;
				p2->prev = p3;
				p3->prev = p1;
			}
			*/
            //addElement2(dictionary, key, p->value, 1);
			changed = 1;
			//strcpy(table[nr], p->value);
			//nr++;

            if(newListDouble == NULL) {
                //first in the list
                newListDouble = newNode;
            }
            else {
                prevNode->next = newNode;
                newNode->prev = prevNode;

            }

            prevNode = newNode;
        }
        p=p->next;
		if(changed) {
			//ListNode* p2 = p->prev;
			
			//while(compareNodes(p2, p2->prev)<0) {
			//	ListNode* p3 = p2->prev;
			//}
			
			
			//removeElement(dictionary, prevKey, prevValue);
			//addElement2(dictionary, prevKey, prevValue, 0);
			
		}
    }while(p!=startNode);

    //printf("after do\n");
	
	//dictionary = reverseLists(reverseLists(dictionary));

	
	
	
	
	/*
	int listChanged = 1;
	while(listChanged == 1) {
		listChanged = 0;
		printf("while %d\n", listChanged);
		ListNode * a= dictionary->lists[r];;
		ListNode * b = a;
	
		do {
			printf("do %d\n", listChanged);
			if(b->frequency <  b->next->frequency) {
				printf("if %d\n", listChanged);
				ListNode *b1 = b->prev;
				ListNode *b2 = b;
				ListNode *b3 = b->next;
				ListNode *b4 = b->next->next;
				
				b1->next = b3;
				b3->next = b2;
				b2->next = b4;
				
				b4->prev = b2;
				b2->prev = b3;
				b3->prev = b1;
				
			    listChanged = 1;
				
				b = b->next;
				
			}
			
			b = b->next;
		} while(b != a && b->next != a);
	}
	*/
	
	//for(int j = 0; j < nr; j++) {
	//	addElement(dictionary, key, table[j], 0);
	//}

	//sort(dictionary, r);
	//for(ListNode* node = newListDouble; node != NULL; node = node->next) {
		//node->frequency++;
		
		//addElement2(dictionary, key, node->value, 1);
	//}
	
	ListNode *abc = newListDouble;
	//while(abc != NULL) {
		
		//addElement2(dictionary, key, abc->value, 1);
		
		//abc = abc->next;
	//}
	
	p = startNode;
	
	
	int pIndex = 0;
	do{
		pIndex++;
		
		if(pIndex > 3 && p->prev->frequency < p->frequency && p->prev->prev->frequency < p->frequency) {
			
			ListNode *p1 = p->prev->prev->prev;
			ListNode *p2 = p->prev->prev;
			ListNode *p3 = p->prev;
			ListNode *p4 = p;
			ListNode *p5 = p->next;
			
			p1->next = p4;
			p4->next = p2;
			p2->next = p3;
			p3->next = p5;
			
			p5->prev = p3;
			p3->prev = p2;
			p2->prev = p4;
			p4->prev = p1;
			
			p = p3;
		}
		else {
			if(pIndex >2 && p->prev->frequency < p->frequency) {
				ListNode *p1 = p->prev->prev;
				ListNode *p2 = p->prev;
				ListNode *p3 = p;
				ListNode *p4 = p->next;
				
				p1->next = p3;
				p3->next = p2;
				p2->next = p4;
				
				p4->prev = p2;
				p2->prev = p3;
				p3->prev = p1;
				
				p = p2;
			}
			
		}
	
	     p=p->next;
	} while(p!=startNode);
	
	return newListDouble;
}

void printCircularListByFrequency(FILE *f, ListNode *aList, int frequency){
    ListNode *p=aList;
    do{
        if(p->frequency == frequency){
            fprintf(f,"(%s, %s, %d) ",p->key,p->value,p->frequency);
            printf("(%s, %s, %d) ",p->key,p->value,p->frequency);

        }
        p=p->next;
    }while(p!=aList);
}
void printFrequency(FILE *f, Dictionary *dictionary , int frequency) {
	// TODO
	for(int i=0 ; i < dictionary->N ;i++){
        if(dictionary->lists[i] != NULL){
			printCircularListByFrequency(f,dictionary->lists[i],frequency);
		}

	}
    fprintf(f,"\n");
    printf("\n");

	return;
}

void printCircularListByValue(FILE *f, ListNode *aList, char *value){
    ListNode *p=aList;
    do{
        if(strcmp(p->value, value)==0){
            fprintf(f,"(%s, %s, %d) ",p->key,p->value,p->frequency);
            printf("(%s, %s, %d) ",p->key,p->value,p->frequency);
        }
        p=p->next;
    }while(p!=aList);
}
void printValue(FILE *f, Dictionary *dictionary , char *value) {
	// TODO
	//fprintf("print value %s\n", value);
	for(int i=0 ; i < dictionary->N ;i++){
        if(dictionary->lists[i] != NULL){
			printCircularListByValue(f,dictionary->lists[i],value);
		}

	}
    fprintf(f,"\n");
    printf("\n");

	return;
}

/*void printValue(FILE *f, Dictionary *dictionary , char *value) {
return;
}*/

ListNode* unionValues(Dictionary *dictionary, char *value) {
	// TODO
	ListNode *newListValue=NULL;
	for(int i=0; i<dictionary->N; i++){
        ListNode *startNode=dictionary->lists[i];
        if(startNode == NULL){
            continue;
        }
        ListNode *p=startNode;
        ListNode *newNode;
        ListNode *prevNode;
        do{
            if(strcmp(p->value,value)==0){
                newNode=copyListNode(p);
                if(newListValue == NULL){
                    newListValue=newNode;
                }
                else{
                    newNode->prev=prevNode;
                    prevNode->next=newNode;
                }
                prevNode=newNode;

            }
            p=p->next;
        }while(p!=startNode);


	}
	return newListValue;
}

ListNode* unionMaxFrequencies(Dictionary *dictionary) {
	// TODO
	//printf("Here!\n");
	ListNode *newListMaxFrequencies=NULL;
	for(int i=0 ;i < dictionary->N;i++){

	//printf("Here  %d!\n",i);
        ListNode *startNode=dictionary->lists[i];
        if(startNode == NULL){
            continue;
        }
        int maxFrequency=startNode->frequency;
        ListNode *p=startNode;
        ListNode *newNode;
        ListNode *prevNode;
        do{ //printf("Herrrrrrre!\n");
            newNode=copyListNode(p);
            if(newListMaxFrequencies == NULL){
                newListMaxFrequencies=newNode;
            }
            else{
                newNode->prev=prevNode;
                prevNode->next=newNode;
            }
            prevNode=newNode;

            p=p->next;
        }while(p!=startNode && p->frequency==maxFrequency);

	}
//printf("Heeeeeeeere!\n");
	return newListMaxFrequencies;
}


Dictionary* reverseLists(Dictionary *dictionary) {
	// TODO

	//printf("Here1.....................\n");
	Dictionary *dex=createDictionary(dictionary->N);
	for( int i=0; i < dictionary->N; i++){
        //printf("Here2..%d...................\n", i);
		if(dictionary->lists[i]!=NULL){
		    //printf("Here3.....................\n");
            if(dictionary->lists[i]->next == dictionary->lists[i]){
			    addElement2(dex,dictionary->lists[i]->key,dictionary->lists[i]->value,dictionary->lists[i]->frequency);
			}
			else {
			    //printf("Here######.....................\n");
                ListNode *startNode=dictionary->lists[i];
                ListNode *lastNode = startNode->prev;
                ListNode *p = lastNode->prev;

                ListNode *lastNodeCopy = copyListNode(lastNode);
//                printf("abc\n");
                ListNode *prevCopy = lastNodeCopy;
                dex->lists[i] =  lastNodeCopy;
                do {
                    //printf("do\n");
                    ListNode* pCopy = copyListNode(p);
                    prevCopy->next = pCopy;
                    pCopy->prev = prevCopy;
                    prevCopy = pCopy;
                    p = p->prev;
                }while(p != lastNode);

                lastNodeCopy->prev = prevCopy;
                prevCopy->next = lastNodeCopy;

			}
		}

    }
    return dex;
}

/*
void printList(FILE *f, ListNode *list) {
	// TODO
	return;
}*/


void printList(FILE *f, ListNode *list){
    //printf("printList\n");
    if(list == NULL) {
        fprintf(f,"\n");
        printf("\n");
        return;

    }

    ListNode *p=list;
    do{
        fprintf(f,"(%s, %s, %d) ",p->key,p->value,p->frequency);
        printf("(%s, %s, %d) ",p->key,p->value,p->frequency);

        p=p->next;
    }while(p!=NULL);

    fprintf(f,"\n");
    printf("\n");

}

void freeDictionary(Dictionary *dictionary) {
	// TODO
	for(int i=0 ; i< dictionary->N ;i++){
		if(dictionary->lists[i] != NULL){
			free(dictionary->lists[i]->key);
			free(dictionary->lists[i]->value);
		}
	}
	free(dictionary->lists);
	free(dictionary);
	return;
}

void freeList(ListNode *list) {
	// TODO

	return;
}

#endif
