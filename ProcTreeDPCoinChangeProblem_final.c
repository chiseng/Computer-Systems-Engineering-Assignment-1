//
//  ProcTreeDPCoinChangeProblem.c
//  CSE_CCodes
//
//  Created by Natalie Agus on 4/1/19.
//  Copyright © 2019 Natalie Agus. All rights reserved.
//

#include "ProcTreeDPCoinChangeProblem.h"

/*
 Entry function. DO NOT CHANGE THIS FUNCTION
 */
void getCoinChangeCombination_test(int* coinSupplyValue, int value, int coinTypes, methodType method){
    
    //allocate memory for (value+1) * coinTypes ProcessTreeNode struct (each represents a cell in the DP table)
    //create (value+1) * coinTypes ProcessTreeNode structs (each represents a cell in the DP table)
    int totalNodes = (value+1) * coinTypes;
    ProcessTreeNode nodesDP[totalNodes];
    
    int counter = 0;
    for(int i = 0; i<(coinTypes); i++){
        for (int j = 0; j<(value+1); j++){
            initProcessTreeNode(&nodesDP[counter], j, i+1, counter, totalNodes, 0);
            counter ++;
        }
    }
    
    //assert the total number of nodes created is the same as totalNodes
    assert(totalNodes == counter);
    
    //link children and parents to each node
    linkRelationshipProcessTreeNode(nodesDP, coinSupplyValue, value, coinTypes);
    
    
    switch (method){
        case (USESEQUENTIAL):
            runFunctionSequentialMethod_Create(nodesDP);
            break;
        case (USETHREAD):
            runFunctionThreadMethod_Create(nodesDP);
            break;
        case (USEPROCESS):
        {
            // int forkReturnVal = fork();
            // if (forkReturnVal == 0)
                runFunctionProcessMethod_Create(nodesDP);
            // else{
            //     wait(NULL);
            // }
            break;
        }
    }
    
    
    printTreeForDebugging(nodesDP, counter);
}

void initProcessTreeNode(ProcessTreeNode* P, int value, int coinTypes, int number, int totalNodes, int initVal){
    P->numberOfParents = 0;
    P->numberOfChildren = 0;
    P->nodeNumber = number;
    sprintf(P->name, "S%dV%d", coinTypes, value);
    P->totalNumberOfNodes = totalNodes;
    P->cellValue = initVal;
    P->processStatus = UNDEFINED;
}

void printTreeForDebugging(ProcessTreeNode* nodesDP, int items){
    //try printing their names and children, as well as parents,
    //to see if they have been created correctly
    printf("Total number of nodes is %d \n", items);
    for (int i = 0; i<items; i++){
        printf("\nNode number %d with name: %s ", nodesDP[i].nodeNumber, nodesDP[i].name);
        printf("has %d children. The children are: ", nodesDP[i].numberOfChildren);
        for (int j = 0; j < nodesDP[i].numberOfChildren; j++){
            printf("%s, ", nodesDP[i].children[j]->name);
        }
        printf("and has %d parents. The parents are: ", nodesDP[i].numberOfParents);
        for (int j = 0; j < nodesDP[i].numberOfParents; j++){
            printf("%s, ", nodesDP[i].parents[j]->name);
        }
        printf("\nThe value of this node is: %d ", nodesDP[i].cellValue);
        printf("\nthe node type is: %s", nodesDP[i].type_of_node);
        printf("\n\n");


        
    }
}


/**
 Part 2
 @param int* coinSupplyValue: gives the denomination of coins (unlimited supply for each denomination)
 @param int value: that you want to make up using the coins
 @param int cointTypes: the total size of array coinSupplyValue (how many different coin types are there)
 **/

void linkRelationshipProcessTreeNode(ProcessTreeNode* nodesDP, int* coinSupplyValue, int value, int coinTypes)
{
 //root node should be SmVn
	int newvalue=value+1;
	int count = 0;
 //nodesDP represents initialised structs
	for(int i=0;i<coinTypes; i++){ //S
		for(int j=0; j<(value+1); j++){ //V
			char S[coinTypes];
			char V[value];
			int marker=coinSupplyValue[i]; //this one marks the row we're at. e.g. S=1, S=2...
			sprintf(S, "%d", marker);
			sprintf(V, "%d", j);

		//initialising the names of the nodes
			strcpy(nodesDP[count].name, "S");
			strcat(nodesDP[count].name, S);
			strcat(nodesDP[count].name, "V");
			strcat(nodesDP[count].name, V);

			nodesDP[count].m=coinSupplyValue[i]; //m refers to the supply value
			nodesDP[count].v=j; //v is value to be found
			nodesDP[count].totalValue=value;



			if(count==0){
				strcpy(nodesDP[count].type_of_node, "root");
				//nodesDP[count].root=true;
			}
			else if(count==(value)*(coinTypes-1)-1){
				strcpy(nodesDP[count].type_of_node, "leaf");	
			}
			else{
				strcpy(nodesDP[count].type_of_node, "normal");
			}
			if(count<=value)
				nodesDP[count].toprow=true; //distinguish the top row for later methods
			else{
				nodesDP[count].toprow=false;
			}

			if(count % newvalue >= marker) {
                nodesDP[count].parents[nodesDP[count].numberOfParents]=&nodesDP[count-marker];
                nodesDP[count].numberOfParents++;

                nodesDP[count-marker].children[nodesDP[count-marker].numberOfChildren]=&nodesDP[count];
                nodesDP[count-marker].numberOfChildren++;
            }


			//top parents
				if(i>0){
					nodesDP[count].parents[nodesDP[count].numberOfParents]=&nodesDP[count-newvalue];
					nodesDP[count].numberOfParents++;

					nodesDP[count-newvalue].children[nodesDP[count-newvalue].numberOfChildren]=&nodesDP[count];
					nodesDP[count-newvalue].numberOfChildren++;

				}

			if(count ==0)
				nodesDP[count].cellValue = -1; //prevent seg faults
			else
				nodesDP[count].cellValue = 0;

			count++;
		}
	}
}

/**
 Part 3
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionSequentialMethod_Create(ProcessTreeNode* root){
	char nodetype[100], test[100];
	for(int i=0; i<root->totalNumberOfNodes; i++){
		strcpy(nodetype,root->type_of_node);
		strcpy(test,"root");
		if(strcmp(nodetype, test)==0){
		root -> cellValue=1;
		// printf("Assigning root value to node: %s\n", root->name);
	}
	else{
			if(root->numberOfParents==1){
			root->cellValue=root->parents[0]->cellValue;
		}
		else if(root->numberOfParents==2){
			root->cellValue=root->parents[0]->cellValue+root->parents[1]->cellValue;
		}
		

		
		}

		root++;
	}
	for(int i=0; i<root->totalNumberOfNodes; i++){
		root--;
	}


  //use Bottom up approach, start from root node, add parent cells.

    
}


/**
 Part 4
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionThreadMethod_Create(ProcessTreeNode* root){
	pthread_t threads[root->totalNumberOfNodes];

	//root node is always set to 1
	pthread_create(&threads[0], NULL, calchelper, (void *)root);
	root++;
	pthread_join(threads[0], NULL); //wait for the first thread to finish
	for(int i=1; i<(root->totalNumberOfNodes); i++){
		pthread_join(threads[i-1], NULL); 
		//Here we use the memory address of root at [i-1] index instead of incrementing the root pointer
		pthread_create(&threads[i], NULL, calchelper, (void *)&root[i-1]);
	}
    pthread_join(threads[root->totalNumberOfNodes-1], NULL);
    /** Your code here **/
}

/**
 Part 5
 @param root is the root node of your dependency graph, i.e: the node that doesn't depend on anything else
 **/
void runFunctionProcessMethod_Create(ProcessTreeNode* root){
    int shmid;
    key_t key;
    int *shm, *sms;
    // FILE * totext=NULL;


    key=10000;
    // fPtr = fopen("~Documents/nodes.txt", "w");
	    //create shared mem segment
	    if ((shmid = shmget(key, root->totalNumberOfNodes, IPC_CREAT | 0666)) < 0) {
	        perror("shmget");
	        exit(1);
	    }

	    //attach segment to data space
	    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
	        perror("shmat");
	        exit(1);

	    }
	sms = shm;
    for(int i = 0; i < root->totalNumberOfNodes; i++)
    {
        *sms = root[i].cellValue; // initialise the shared memory array to be the current cellvalues.
        sms++;
    }

    for(int i=0; i<root->totalNumberOfNodes; i++)
	{
		pid_t pid=fork();
		//invalid operation, child process will return 0 and pid to its parent
		if(i>0) //go to the next row.
			root++;
		if(pid<0){ 
			printf("%s\n", "error");
			exit(1);
		}
		else if(pid==0){
			shm+=i; //moving the shm pointer. Cannot just shm++ like for other pointers as this 
			//one is not saved.
			if(*shm==-1){
				*shm=1; //setting root node.
			}
			else if(*shm==0){
				while(*(shm-1)<0){ //busy wait for process to complete
				}
				//top row assignment
				if(root->toprow){
					if(root->numberOfParents==0){
						*shm=0; //no parent no fun :<
					}
					else{
					*shm=*(shm-root->m); //assigns value of the cell to the left, according to m.
					}
				}
				//fill up the cells according to the parent values.
				else{
					if(root-> numberOfParents==1){
						*shm=*(shm-(root->totalValue+1)); //fill top
						
					}
					else if(root -> numberOfParents==2){
						*shm=*(shm-(root->totalValue+1)) + *(shm-root->m); //fill top and fill left
						
					}
				}
			}
			exit(0); //return 0 to the childprocess
		}
		else wait(NULL); //wait for child to finish working
	}
	for(int j=0; j< root->totalNumberOfNodes-1; j++){
		root--; //bring back the pointer of root to the start 
	}
	for(int i = 0; i < root->totalNumberOfNodes; i++){
        root->cellValue = shm[i];
        root++;
    }
    for(int j=0; j< root->totalNumberOfNodes-1; j++){
		root--; //bring back the pointer of root to the start 
	}
    if(shmdt(shm)==-1){
    	printf("%s\n", "failed to detach shared memory");
    }

    shmctl(shmid, IPC_RMID, NULL);
    /** Your code here **/
}

/**YOU ARE FREE TO CREATE OTHER HELPER METHODS BELOW THIS LINE**/

void* calchelper(void *arg){
	ProcessTreeNode * node= (ProcessTreeNode *) arg;
	char nodetype[100], test[100];
	strcpy(nodetype,node->type_of_node);
	strcpy(test,"root");
	if(strcmp(nodetype, test)==0){
		memset(nodetype, 0, strlen(nodetype));
		memset(test, 0, strlen(test));
		node -> cellValue=1;
		//printf("Assigning root value to node: %s\n", node->name);
		return NULL;
		}
	else{
		memset(nodetype, 0, strlen(nodetype));
		memset(test, 0, strlen(test));
		if(node->numberOfParents==0){
			node->cellValue=0;
		}
		if(node->numberOfParents==1){
			node->cellValue=node->parents[0]->cellValue;
			return NULL;
		}
		else if(node->numberOfParents==2){
			
			node->cellValue=node->parents[0]->cellValue+node->parents[1]->cellValue;
			return NULL;
		}
	}

}
