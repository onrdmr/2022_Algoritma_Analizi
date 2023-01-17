#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LINE_BUFFER 500
#define false 0
#define true 1

typedef long long Id;
typedef long Number;
typedef int bool;

typedef struct idPointer {
    Id Id;
    struct idPointer *next;
} IdPtr;

typedef struct  id_list {
    int size;
    IdPtr * begin;
    IdPtr * last;
} IdList;



typedef IdList IdStack;

typedef struct idStackPtr {
    IdStack * stack;
    struct idStackPtr *next; 
} IdStackPtr;

typedef struct idStackList {
    Number indirectConnectionSize;
    IdStackPtr * IdStacks;
} IdStackList; 

typedef struct node {
    char * name;
    char * surname;
    Id id;
    IdList * inDegreeList;
    IdList * outDegreeList;
    Number connectivity;
    bool * visited; // exploration purpose
    IdStackList * connections; // copy of in degree connections
} Node;


int getNumberOfPersonInSystem(char * filename) {
     FILE *fp;
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
 
 
    // Open the file
    fp = fopen(filename, "r");
 
    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
 
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
 
    // Close the file
    fclose(fp);
    printf("System %s has %d people.\n ", filename, count/2+1);
 
    return count/2;
}

// typedef struct pair {
//     Id id;
//     Node * node;
// } Pair;

void pushBack(IdList * list, Id id) {
    
    if( list->size == 0 ) {
        list->begin = (IdPtr*) malloc(sizeof(IdPtr));
        list->begin->Id = id;
        list->last = list->begin;
    } else {
        list->last->next = (IdPtr*) malloc(sizeof(IdPtr));
        list->last->next->Id = id;
        list->last = list->last->next;
    }
    list->last->next=NULL;
    list->size++;
}

char** strSplit(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


int strCount(char * s, char delim) {
    int i = 0;
    for (i=0; s[i]; s[i]==delim ? i++ : *s++);
    return i;
}

IdList* allocateVectorId() {
    
    IdList* idVector = (IdList*) malloc(sizeof(IdList)); idVector->size=0;
    return idVector;
}

void dumpGraph(Node ** nodeMap, int nodeMapSize) {
    printf( "\n Dumping....\n" );
    int i, j;
    Node* node;

    for (i = 1 ; i < nodeMapSize ; i++) {
        if(nodeMap[i] != NULL) {

            Node* node = nodeMap[i];
            printf("--------------------------------------------------\n");
            printf("Account id : %d \n" , node->id );
            printf("Accunt name : %s \n " , node->name );
            printf( "Account surname : %s \n", node->surname );
            printf("Connectivity : %d\n" , node->connectivity );
            printf( "Visited(debug purpose) : %d \n",  *node->visited );
            
            
            // std::cout << "Follewee" << node << std::endl;

            printf("\nFollowing " );
            IdList * outDegreeList = node->outDegreeList;
            IdPtr * itr = outDegreeList->begin;
            for( j = 0 ; j < outDegreeList->size ; j++ ) {
                printf("%d", itr->Id);
                printf(",");
                itr = itr->next;
            }
            printf("\n");


            printf( "\nFollower " );
            IdList * inDegreeList = node->inDegreeList;
            itr = inDegreeList->begin;
            for( int j = 0 ; j < inDegreeList->size ; j++ ) {
                printf( "%d" , itr->Id);
                printf(",");
                itr = itr->next;
            }
            printf("\n");


            printf("--------------------------------------------------\n");
        }
    }
}


Node ** copy (Node ** nodeMap, int nodeMapSize) {
    int i, j ;
    Node ** explorationMap = (Node**) malloc(nodeMapSize*sizeof(Node*));
    IdPtr* idPtr, *odPtr;

    for(i = 1 ; i < nodeMapSize ; i++) {
        
        Node * node = (Node*) malloc(sizeof(Node));
        node->id = nodeMap[i]->id;
        node->connectivity =  nodeMap[i]->connectivity;
        // node->name = (char*) malloc(sizeof(LINE_BUFFER/3));
        node->name =  strdup(nodeMap[i]->name);
        node->surname = strdup(nodeMap[i]->surname);
        node->visited = (bool*) malloc(sizeof(bool));
        node->connections = NULL;
        *node->visited=*(nodeMap[i]->visited);
        // indegree copy
        IdList* idList = (IdList*) malloc(sizeof(IdList));
        int iSize = nodeMap[i]->inDegreeList->size  ;
        idList->size = iSize;
        explorationMap[i] = (Node*) malloc(sizeof(Node));
        explorationMap[i]->inDegreeList = idList;
    
        // if( i == 4) {
        //     printf("adw");
        // }
        if(iSize != 0) {
            idPtr = (IdPtr*) malloc(sizeof(IdPtr));
            idPtr->Id = nodeMap[i]->inDegreeList->begin->Id;
            idList->begin = idPtr;
            idList->last = idPtr;
        }
        IdPtr* itr = nodeMap[i]->inDegreeList->begin->next;
        for(j = 1 ; j < iSize ; j++) {
            idPtr = (IdPtr*) malloc(sizeof(IdPtr));
            idPtr->Id = itr->Id;
            // explorationMap[i]->inDegreeList->size++;
            idList->last->next = idPtr;
            idList->last = idList->last->next;
            itr = itr->next;
        }
        idList->last = idPtr;
        idList->last->next = NULL;

        // outdegree copy
        IdList* odList = (IdList*) malloc(sizeof(IdList));
        int oSize = nodeMap[i]->outDegreeList->size;
        odList->size = oSize;
        explorationMap[i]->outDegreeList = odList;


        if(iSize != 0) {
            odPtr = (IdPtr*) malloc(sizeof(IdPtr));
            odPtr->Id = nodeMap[i]->outDegreeList->begin->Id;
            odList->begin = odPtr;
            odList->last = odPtr;
        }
        itr = nodeMap[i]->outDegreeList->begin->next;

        for(j = 1 ; j < oSize ; j++) {
            odPtr = (IdPtr*) malloc(sizeof(IdPtr));
            odPtr->Id = itr->Id;
            // explorationMap[i]->outDegreeList->size++;
            odList->last->next = odPtr;
            odList->last = odList->last->next;
            itr = itr->next;
        }
        odList->last = odPtr;
        odList->last->next = NULL;

        node->outDegreeList = odList;
        node->inDegreeList = idList;

        explorationMap[i] = node;
         
    }
    return explorationMap;
}

Node ** allocateNodeMap(int peopleSize) {
    Node ** nodeMap = (Node**) malloc(peopleSize * sizeof(Node*));
    int i = 0;
    for ( i = 0 ; i < peopleSize+2 ; i++ ) {
        nodeMap[i] = NULL;
        
    }
    return nodeMap;
}

char ** strSplit_r(char * line, char c) {
    char * block = strdup(line);
    char ** result = strSplit(block, c);
    return result;
}

void removeIdList(IdList * idList, int id) {
    IdPtr * itr = idList->begin;
    int i ;
    
    if( itr->Id == id && idList->begin == idList->last ) {
        idList->begin = idList->last = NULL;
        free(idList->begin);
        idList->size--;
        return;
    } else if(itr->Id == id) {
        idList->begin=itr->next;
        IdPtr * removed = itr;
        itr = itr->next;
        free(removed);
        idList->size--;
        return;
    } else {
        IdPtr * tmp = idList->begin;
        itr = itr->next;
        for(i = 1; i < idList->size ; i++, itr=itr->next, tmp = tmp->next) {
            if(itr->Id == id) {
                if( itr->next == NULL ) {
                    tmp->next = itr->next;
                    idList->last = tmp; 
                    free(itr);
                }else {
                    tmp->next = itr->next;
                    free(itr);
                }
                idList->size--;
                return;
            }
        }
    }

}

void pop(IdStack * stack) {
    int i;

    if(stack->size == 1 ) {
        stack->last = NULL;
        stack->begin = NULL;
        stack->size = 0;
        return;
    } else {
        IdPtr * itr = stack->begin;
        for(i = 0 ; i < stack->size ; i++ ,itr = itr->next) {
            if(itr->next == stack->last ) {
                itr->next = NULL;
                
                stack->last = itr;
                stack->size--;
                return ;
            }
        }

    }
    
}

Number top (IdStack * stack) {
    return stack->last->Id;
}

IdStack * copyStack(IdStack * stack) {
    IdStack * copy = (IdStack*) malloc(sizeof(IdStack));
    copy->size = stack->size;
    int i ;
    copy->begin = (IdPtr*) malloc(sizeof(IdPtr));
    copy->last = (IdPtr*) malloc(sizeof(IdPtr));
    IdPtr * citr = copy->begin;
    IdPtr * itr = stack->begin;
    for(i = 0 ; i < copy->size ; i++) {
        citr->Id = itr->Id;
        citr->next = (IdPtr*) malloc(sizeof(IdPtr));
        copy->last = citr;
        citr = citr->next;
        itr=itr->next;
    }
    copy->last->next = NULL;
    
    copy->begin->Id=stack->begin->Id;
    copy->size = stack->size;

    return copy;
}

void dfs( IdStack * dfsStack, IdStack * visitedPointers, Node ** nodeMap, Node* nodeMain ) {
    int i , j;
    IdPtr * itr = nodeMain->inDegreeList->begin;
    for ( i = 0 ; i < nodeMain->inDegreeList->size ; i++, itr=itr->next) {
        Node * node = nodeMap[itr->Id];
        if(*node->visited) {
            Node * nodeTracked = nodeMap[dfsStack->begin->Id];
            if(nodeTracked->connections == NULL) {
                nodeTracked->connections = (IdStackList*) malloc(sizeof(IdStackList));
                nodeTracked->connections->IdStacks = (IdStackPtr* ) malloc(sizeof(IdStackPtr));
                nodeTracked->connections->indirectConnectionSize = 1;
                nodeTracked->connections->IdStacks->next = NULL;
                pushBack(dfsStack, itr->Id);    
                IdStack * stack = copyStack(dfsStack);
                nodeTracked->connections->IdStacks->stack = stack;
            }
            else {
                pushBack(dfsStack, itr->Id);    
                IdStackPtr * itr = nodeTracked->connections->IdStacks;
                for ( j = 0 ; j < nodeTracked->connections->indirectConnectionSize-1 ; j++) {
                    itr = itr->next ; 
                }
                itr->next = (IdStackPtr* ) malloc(sizeof(IdStackPtr));
                itr->next->stack =  copyStack(dfsStack);
                itr->next->next=NULL;
                nodeTracked->connections->indirectConnectionSize++;
            }
            pop(dfsStack);                
            continue;
        }
        *nodeMain->visited =  true;
        pushBack(dfsStack, itr->Id);
        dfs(dfsStack, visitedPointers, nodeMap, node) ;
    }
   *nodeMain->visited =  true;
    pushBack(visitedPointers, nodeMain->id);
    pop(dfsStack);
    return;
}

IdStack * getIdxsExploreMap(Node** nodeMap, int size) {
    IdStack * idxs = allocateVectorId();
    int i;
    for(i = 1 ; i < size ; i++) {
        if(nodeMap[i] != NULL) {
            pushBack(idxs, i);
        }
    }
    return idxs;
}

void writeConnectivity(Node ** nodeMap, int nodeMapSize) {
    int i , j, maxConnection;
    for (i = 1 ; i < nodeMapSize ; i++) {
        if(nodeMap[i] != NULL) {
            IdStackPtr * itr=  nodeMap[i]->connections->IdStacks;
            maxConnection = 0;
            for ( j = 0 ; j < nodeMap[i]->connections->indirectConnectionSize ; j++ , itr=itr->next ) {
                
                if(itr->stack->size > maxConnection) {
                    maxConnection = itr->stack->size;
                }
            }

            nodeMap[i]->connectivity = maxConnection;
        }
    }
    
}

void dumpGraphDetail(Node ** nodeMap, int nodeMapSize) {
    printf( "\nDumping....\n" );
    int i, j, k;
    Node* node;

    for (i = 1 ; i < nodeMapSize ; i++) {
        if(nodeMap[i] != NULL) {

            Node* node = nodeMap[i];
            printf("--------------------------------------------------\n");
            printf("Account id : %d \n" , node->id );
            printf("Accunt name : %s \n " , node->name );
            printf( "Account surname : %s \n", node->surname );
            printf("Connectivity : %d\n" , node->connectivity );
            printf( "Visited(debug purpose) : %d \n",  *node->visited );


            // std::cout << "Follewee" << node << std::endl;

            printf("\nFollowing " );
            IdList * outDegreeList = node->outDegreeList;
            IdPtr * itr = outDegreeList->begin;
            for( j = 0 ; j < outDegreeList->size ; j++ ) {
                printf("%d", itr->Id);
                printf(",");
                itr = itr->next;
            }
            printf("\n");


            printf( "\nFollower " );
            IdList * inDegreeList = node->inDegreeList;
            itr = inDegreeList->begin;
            for( int j = 0 ; j < inDegreeList->size ; j++ ) {
                printf( "%d" , itr->Id);
                printf(",");
                itr = itr->next;
            }
            printf("\n");


            printf("Writing all connection string in this row :  ");
            IdStackPtr * itr2=  nodeMap[i]->connections->IdStacks;
            for ( j = 0 ; j < nodeMap[i]->connections->indirectConnectionSize ; j++ , itr2=itr2->next ) {
                IdPtr * itr3 = itr2->stack->begin;
                printf("connection %d : ", j);
                for ( k = 0 ; k < itr2->stack->size; k++, itr3=itr3->next ) {
                    printf("%d ", itr3->Id);
                }
                printf("|| ");
            }

            printf("--------------------------------------------------\n");
        }
    }
}

void removeNodesConnectivityLowerThan(Node ** nodeMap, int nodeMapSize, int Y) {
    int i , j, maxConnection;
    for (i = 1 ; i < nodeMapSize ; i++) {
        if(nodeMap[i] != NULL) {
            if(nodeMap[i]->connections->indirectConnectionSize <= Y) {
                Node * node = nodeMap[i];
                nodeMap[i] = NULL;
                free(node);
            }
        }
    }
}

void dumpStack(Node ** nodeMap , IdStack * idStack) {
    printf("\nSelecting values from X elimination \n");
    IdPtr * itr = idStack->begin;
    for(int i = 0 ; i < idStack->size ; i++, itr=itr->next) {
        printf("Id :%d, ", itr->Id, nodeMap[itr->Id]->name );
        printf("Name: %s,", nodeMap[itr->Id]->name);
        printf("Surname : %s\n", nodeMap[itr->Id]->surname);
    }
    
}

void removeNodesLowerThanFollowerCount(Node ** nodeMap, int nodeMapSize, IdList * selector ) {
    IdPtr * itr = selector->begin;
    int i = 0;
    for(i = 0 ; i < selector->size ; i++, itr=itr->next) {
        Node * node = nodeMap[itr->Id];
        nodeMap[itr->Id]= NULL;
        free(node);
    }
}

int main() {

    printf("\t\t\tWELCOME INFLUENCER DETECTION PROGRAM\n");
    printf("\t\tTerms need to know\n");
    printf("\t\tSTEP 1 -- M : follower elimination until all follower count decreased that value(counting)\n");
    printf("\t\tSTEP 2 -- X : truncate follower number you want (without recounting)\n");
    printf("\t\tSTEP 3 -- Y : number of maximum indirect connection in degree.\n");

    char mode[LINE_BUFFER];

    while(true) {

        printf("Normal mod or Detail mode(normal/detail)");
        scanf("%s",&mode);
        if(!strcmp(mode,"detail")){
            printf("Enter M X and Y values.\n");
            int M = 1;
            int X = 3;
            int Y = 4;
            
            scanf("%d %d %d", &M, &X, &Y);
            printf("CALCULATING INFLUENCER FOR GIVEN %d %d %d ...\n", M, X, Y);


            // STEP 1
            char* filename = "socialNet.txt";
            int peopleSize = getNumberOfPersonInSystem(filename);

            char line[LINE_BUFFER];

            Node ** nodeMap = allocateNodeMap(peopleSize) ;

            int nodeMapSize = 0;
            int len = 0;
            
            // Open the file
            FILE* socialNetFile = fopen(filename, "r");

            // Check if file exists
            if (socialNetFile == NULL)
            {
                printf("Could not open file %s", filename);
                return 0;
            }
                int read;

            while (fgets(line, LINE_BUFFER, socialNetFile)) {
                
                Id id ;

                // printf("%s is taken\n", line);
                int resultSize = strCount(line, ',') ;
                


                if(!(nodeMapSize % 2)) {
                    char ** result = strSplit_r(line, ',');
                    
                    printf("%s %s %s", result[0],result[1],result[2]  );
                    Node * myNode = (Node*) malloc(sizeof(Node));
                    myNode->name = result[1];
                    myNode->surname = result[2];
                    myNode->id = atoi(result[0]);
                    myNode->connectivity = 0;
                    bool * isVisited = (bool*) malloc(sizeof(bool));
                    myNode->visited = isVisited;
                    *myNode->visited = false;
                    IdList * idVector = allocateVectorId();
                    IdList * outVector = allocateVectorId();
                    myNode->inDegreeList = idVector;
                    myNode->outDegreeList = outVector; 
                    id = myNode->id; 
                    if( nodeMap[myNode->id] != NULL ) {
                        nodeMap[myNode->id]->name = myNode->name; 
                        nodeMap[myNode->id]->surname = myNode->surname;
                        nodeMap[myNode->id]->connectivity = myNode->connectivity;
                        nodeMap[myNode->id]->visited = myNode->visited;
                        // nodeMap[myNode->id]->inDegreeList=myNode->inDegreeList;
                        // nodeMap[myNode->id]->outDegreeList = myNode->outDegreeList;
                        free(myNode);
                    } else {
                        nodeMap[myNode->id] = myNode;
                    }

        
                } else {
                        // nodeMap[myNode->id] = (Node*) malloc(sizeof(Node));
                        // nodeMap[myNode->id] = myNode;
                        
                        char ** result = strSplit_r(line,',');
                        
                        for(size_t i = 0 ; i < resultSize ; i++) {
                            if( nodeMap[atoi(result[i])] != NULL ) {

                                pushBack(nodeMap[atoi(result[i])]->inDegreeList, id); 
                                pushBack(nodeMap[id]->outDegreeList, atoi(result[i]));

                            } 
                            else {
                                Node * myNode = (Node*) malloc(sizeof(Node));
                                myNode->id = atoi(result[i]); 
                                myNode->connectivity=0;
                                myNode->visited = (bool*) malloc(sizeof(bool));
                                *myNode->visited = false;
                                IdList * idVector = allocateVectorId();
                                IdList * outVector = allocateVectorId();
                                myNode->inDegreeList = idVector;
                                myNode->outDegreeList = outVector; 
                                


                                pushBack(myNode->inDegreeList, id); 
                                nodeMap[atoi(result[i])] = myNode;
                                pushBack(nodeMap[id]->outDegreeList, atoi(result[i]));
                            }
                        }
                    }

                nodeMapSize++;
            }
            fclose(socialNetFile);

            printf("\n\n\nSTEP 1 elimination \n\n\n") ;
            dumpGraph(nodeMap, nodeMapSize/2+1);
            bool searchTag = true;

            Node ** exploreMap = copy(nodeMap, nodeMapSize/2+1);
            

            // printf("\n\n\n After Copying \n\n\n"); dumpGraph(exploreMap, nodeMapSize/2+1);
            
            int exploreMapSize = nodeMapSize/2+1;

            IdStack* removed = allocateVectorId();

            // Step 1
            int i, j;
            while (searchTag) {
                searchTag = false;
                for( i = 1 ; i < exploreMapSize ; i++ ) {
                    if(exploreMap[i] != NULL) {
                        Node * node = exploreMap[i];
                        if( node->inDegreeList->size < M ) {
                            pushBack(removed, node->id);
                            int nodeId = node->id;
                            IdPtr *itr = node->outDegreeList->begin;
                            for ( j = 0 ; j < node->outDegreeList->size ; j++, itr = itr->next) {
                                IdList * inDegreeList =  exploreMap[itr->Id]->inDegreeList;
                                if(inDegreeList->size ) {
                                    removeIdList(inDegreeList, nodeId);
                                }
                            }
                            itr = node->inDegreeList->begin;
                            for ( j = 0 ; j < node->inDegreeList->size ; j++, itr = itr->next) {
                                IdList * outDegreeList =  exploreMap[itr->Id]->outDegreeList;
                                if(outDegreeList->size) {
                                    removeIdList(outDegreeList, nodeId);
                                }

                            }
                            searchTag = true;
                        }
                    }
                }

                if(removed->size > 0) {
                    IdPtr * itr = removed->begin;
                    for(j = 0 ; j <  removed->size ; j++, itr=itr->next) {
                        exploreMap[itr->Id] = NULL;
                        // exploreMapSize--;
                    }
                    free(removed);
                    removed = allocateVectorId();
                }

            }

            
            IdStack * exploreMapIdx = getIdxsExploreMap(exploreMap, nodeMapSize/2+1);

            // STEP 2
            IdPtr * itr = exploreMapIdx->begin;
            
            IdList * selector =  allocateVectorId();

            IdPtr * lastIdPtr;
            for( i = 1 ; i < exploreMapIdx->size ; i++, itr=itr->next ) {
            
                    Node * node = exploreMap[itr->Id];
                    // std::cout << node->name << " " << node->surname << std::endl;
                    if( node->inDegreeList->size < X ) {
                        pushBack(selector, node->id);
                    } 

            }
            
            printf("\n\n\nSTEP 2 elimination \n\n\n") ;dumpStack(exploreMap, selector);

            // Step 3
            itr = exploreMapIdx->begin;
            
            for ( i = 0 ; i < exploreMapIdx->size ; i++,itr=itr->next  ) {
                Node * node = exploreMap[itr->Id];
                IdStack * stack = allocateVectorId();
                IdStack * visitedPointers = allocateVectorId();
                pushBack (stack, node->id);
                // *node->visited = true;
                // visitedPointers.push(node->visited);
                dfs(stack, visitedPointers, exploreMap, node); // return all tree edge and back edges of node
                // node->connectivity = connectivity;
                
                while(visitedPointers->size!= 0) {
                    *exploreMap[top(visitedPointers)]->visited =false;  
                    pop(visitedPointers);
                }

            }
            
            

            writeConnectivity(exploreMap, exploreMapSize);
            removeNodesConnectivityLowerThan(exploreMap, exploreMapSize, Y);

            printf("\n\n\nSTEP 3 elimination \n\n\n") ;
            removeNodesLowerThanFollowerCount(exploreMap, exploreMapSize, selector);
            dumpGraphDetail(exploreMap, nodeMapSize/2 + 1);

        } else if(!strcmp(mode, "normal")){
            printf("Enter M X and Y values.\n");
            int M = 1;
            int X = 3;
            int Y = 4;
            
            scanf("%d %d %d", &M, &X, &Y);
            printf("CALCULATING INFLUENCER FOR GIVEN %d %d %d ...\n", M, X, Y);


            // STEP 1
            char* filename = "socialNet.txt";
            int peopleSize = getNumberOfPersonInSystem(filename);

            char line[LINE_BUFFER];

            Node ** nodeMap = allocateNodeMap(peopleSize) ;

            int nodeMapSize = 0;
            int len = 0;
            
            // Open the file
            FILE* socialNetFile = fopen(filename, "r");

            // Check if file exists
            if (socialNetFile == NULL)
            {
                printf("Could not open file %s", filename);
                return 0;
            }
                int read;

            while (fgets(line, LINE_BUFFER, socialNetFile)) {
                
                Id id ;

                // printf("%s is taken\n", line);
                int resultSize = strCount(line, ',') ;
                


                if(!(nodeMapSize % 2)) {
                    char ** result = strSplit_r(line, ',');
                    
                    printf("%s %s %s", result[0],result[1],result[2]  );
                    Node * myNode = (Node*) malloc(sizeof(Node));
                    myNode->name = result[1];
                    myNode->surname = result[2];
                    myNode->id = atoi(result[0]);
                    myNode->connectivity = 0;
                    bool * isVisited = (bool*) malloc(sizeof(bool));
                    myNode->visited = isVisited;
                    *myNode->visited = false;
                    IdList * idVector = allocateVectorId();
                    IdList * outVector = allocateVectorId();
                    myNode->inDegreeList = idVector;
                    myNode->outDegreeList = outVector; 
                    id = myNode->id; 
                    if( nodeMap[myNode->id] != NULL ) {
                        nodeMap[myNode->id]->name = myNode->name; 
                        nodeMap[myNode->id]->surname = myNode->surname;
                        nodeMap[myNode->id]->connectivity = myNode->connectivity;
                        nodeMap[myNode->id]->visited = myNode->visited;
                        // nodeMap[myNode->id]->inDegreeList=myNode->inDegreeList;
                        // nodeMap[myNode->id]->outDegreeList = myNode->outDegreeList;
                        free(myNode);
                    } else {
                        nodeMap[myNode->id] = myNode;
                    }

        
                } else {
                        // nodeMap[myNode->id] = (Node*) malloc(sizeof(Node));
                        // nodeMap[myNode->id] = myNode;
                        
                        char ** result = strSplit_r(line,',');
                        
                        for(size_t i = 0 ; i < resultSize ; i++) {
                            if( nodeMap[atoi(result[i])] != NULL ) {

                                pushBack(nodeMap[atoi(result[i])]->inDegreeList, id); 
                                pushBack(nodeMap[id]->outDegreeList, atoi(result[i]));

                            } 
                            else {
                                Node * myNode = (Node*) malloc(sizeof(Node));
                                myNode->id = atoi(result[i]); 
                                myNode->connectivity=0;
                                myNode->visited = (bool*) malloc(sizeof(bool));
                                *myNode->visited = false;
                                IdList * idVector = allocateVectorId();
                                IdList * outVector = allocateVectorId();
                                myNode->inDegreeList = idVector;
                                myNode->outDegreeList = outVector; 
                                


                                pushBack(myNode->inDegreeList, id); 
                                nodeMap[atoi(result[i])] = myNode;
                                pushBack(nodeMap[id]->outDegreeList, atoi(result[i]));
                            }
                        }
                    }

                nodeMapSize++;
            }
            fclose(socialNetFile);

            // printf("\n\n\nSTEP 1 elimination \n\n\n") ;
            // dumpGraph(nodeMap, nodeMapSize/2+1);
            bool searchTag = true;

            Node ** exploreMap = copy(nodeMap, nodeMapSize/2+1);
            

            // printf("\n\n\n After Copying \n\n\n"); dumpGraph(exploreMap, nodeMapSize/2+1);
            
            int exploreMapSize = nodeMapSize/2+1;

            IdStack* removed = allocateVectorId();

            // Step 1
            int i, j;
            while (searchTag) {
                searchTag = false;

                    

                for( i = 1 ; i < exploreMapSize ; i++ ) {
                    
                    if(exploreMap[i] != NULL) {

                        Node * node = exploreMap[i];
                        // std::cout << node->name << " " << node->surname << std::endl;
                        if( node->inDegreeList->size < M ) {
                            pushBack(removed, node->id);
                            int nodeId = node->id;
                            IdPtr *itr = node->outDegreeList->begin;
                            for ( j = 0 ; j < node->outDegreeList->size ; j++, itr = itr->next) {
                                IdList * inDegreeList =  exploreMap[itr->Id]->inDegreeList;
                                if(inDegreeList->size ) {
                                    removeIdList(inDegreeList, nodeId);
                                }
                            }
                            itr = node->inDegreeList->begin;
                            for ( j = 0 ; j < node->inDegreeList->size ; j++, itr = itr->next) {
                                IdList * outDegreeList =  exploreMap[itr->Id]->outDegreeList;
                                if(outDegreeList->size) {
                                    removeIdList(outDegreeList, nodeId);
                                }

                            }

                            searchTag = true;
                        }
                    }

                }

                if(removed->size > 0) {
                    IdPtr * itr = removed->begin;
                    for(j = 0 ; j <  removed->size ; j++, itr=itr->next) {
                        exploreMap[itr->Id] = NULL;
                        // exploreMapSize--;
                    }
                    free(removed);
                    removed = allocateVectorId();
                }

            }

            
            IdStack * exploreMapIdx = getIdxsExploreMap(exploreMap, nodeMapSize/2+1);

            // STEP 2
            IdPtr * itr = exploreMapIdx->begin;
            
            IdList * selector =  allocateVectorId();

            IdPtr * lastIdPtr;
            for( i = 1 ; i < exploreMapIdx->size ; i++, itr=itr->next ) {
            
                    Node * node = exploreMap[itr->Id];
                    // std::cout << node->name << " " << node->surname << std::endl;
                    if( node->inDegreeList->size < X ) {
                        pushBack(selector, node->id);
                    } 

            }
            
            // printf("\n\n\nSTEP 2 elimination \n\n\n") ;dumpStack(exploreMap, selector);

            // Step 3
            itr = exploreMapIdx->begin;
            
            for ( i = 0 ; i < exploreMapIdx->size ; i++,itr=itr->next  ) {
                Node * node = exploreMap[itr->Id];
                IdStack * stack = allocateVectorId();
                IdStack * visitedPointers = allocateVectorId();
                pushBack (stack, node->id);
                // *node->visited = true;
                // visitedPointers.push(node->visited);
                dfs(stack, visitedPointers, exploreMap, node); // return all tree edge and back edges of node
                // node->connectivity = connectivity;
                
                while(visitedPointers->size!= 0) {
                    *exploreMap[top(visitedPointers)]->visited =false;  
                    pop(visitedPointers);
                }

            }
            
            

            writeConnectivity(exploreMap, exploreMapSize);
            removeNodesConnectivityLowerThan(exploreMap, exploreMapSize, Y);

            printf("\n\n\nResults \n\n\n") ;
            removeNodesLowerThanFollowerCount(exploreMap, exploreMapSize, selector);
            dumpGraph(exploreMap, nodeMapSize/2 + 1);
        } else {
            printf("please enter again.");
        }   
    }

    return 0;
}