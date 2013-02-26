/* 
/**
 * File:   main.c
 * Author: Sharad Sinha
 * Email: sharad_sinha@pmail.ntu.edu.sg
 * Rev: 1.0
 * Release Date: July 31,2012 (Rev 1.0)
 * This program creates a DFG randomly of 'NUM_NODES' number of nodes.
 * The in-degree for each node belongs to the set [indegmin,indegmax] where
 * the two values are user defined. One of these is assigned randomly
 * The program allows to "load" the graph with operations of a certain type. For instance, 
 * there are a large numbers of multipliers
 * © Center for High Performance Embedded Systems(CHiPES),
 * N4-02A-32,School of Computer Engineering, Nanyang Avenue
 * Nanyang Technological University
 * Singapore, 639798
 * This software is released without any warranty and CHiPES and
 * the author disclaims all liabilities arising out of its usage
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"
#define NUM_NODES 200


/*
 *
 */

typedef struct Node
{ char id[6];
  int cstep;
  int in_degree;
  int in_degree_temp;
  int out_degree;
};

void PrintDigraphinDOTlang(struct Node *,int A[][], int *B, int );

int main()
{
    int AdjMat[NUM_NODES][NUM_NODES+1];  // One last column to store the c-step value
    int i,j,k,l,m,n,t,z;
    int randomnum,p,q;
    int indegmin,indegmax;
    int regNum;                          //Stores the register number
    int cstepVal;
    int TotalNumCsteps;
    int numLoadedNodes;                  //Stores the number of loaded nodes
    int randIndex;
    
    FILE *fpGraphAdjMat;
    FILE *fpGraphAdjReg;
    FILE *fpGraphDetails;
    FILE *fpGraphAdjMatOnly;
    
    fpGraphAdjMat = fopen("Graph_Adjacency_List_withNodeID.txt","w"); //This file stores the Adjacency List withNodeID
    fpGraphAdjReg = fopen("Graph_Adjacency_Registers.txt","w");  //This file stores the connection between nodes and registers
    fpGraphDetails = fopen("Graph_Supplemental_Details.txt","w");//This file stores all the supplemental info about the graph
    fpGraphAdjMatOnly = fopen("Graph_Adjacency_List_Only.txt","w"); //This file stores only the Adjacency List 


    struct Node *NodeChar;
    NodeChar = (struct Node*)malloc(sizeof(struct Node) * NUM_NODES);
    if(NodeChar == NULL)
    {
        printf("No memory available");
        exit(1);
    }
 
    //Initialize the in-degree values of all nodes
    for (i=0;i<NUM_NODES ;i++)
    {
        NodeChar[i].in_degree =0;
        NodeChar[i].in_degree_temp=0;
    }
    //Initialize AdjMat array
    for(i=0;i<NUM_NODES;i++)
    {
        for(j=0;j<(NUM_NODES+1);j++)
        {
            AdjMat[i][j]=0;
        }
    }

   //User Inputs
    printf("Please enter the min and the max number of nodes in any c-step:");
    scanf("%d%d", &p,&q);
    printf("Please enter the min and the max in-degree of nodes in any c-step:");
    scanf("%d%d",&indegmin, &indegmax);
    printf("Please enter the number of nodes out of %d that you want to load with operation %s:",NUM_NODES,loadedOperator);
    scanf("%d",&numLoadedNodes);
    
    //The following code with select numLoadedNodes out of NUM_NODES randomly and exclusively
    //This is an implementation of Reservoir Sampling Algorithm
    int NodesforLoad[numLoadedNodes];
    int Nodes[NUM_NODES];
    int NodesFlagged[NUM_NODES];                //A value of 1 means that the node is to be bound to the "loaded op"
    //Initialize the arrays
    for(i=0;i<NUM_NODES;i++)
    {
        Nodes[i]=i;
        NodesFlagged[i]=0;
    }
        
    z=NUM_NODES;
    for(i=0;i<numLoadedNodes;i++)
    {
        randIndex = randomNumGen(0,z);
        NodesforLoad[i]=Nodes[randIndex];
        NodesFlagged[Nodes[randIndex]]=1;
        for(j=randIndex;j<(z-1);j++)
        {
            Nodes[j]=Nodes[j+1];
        }
        z=z-1;        
    }
    
    //The following array is a worst case array for holding the register numbers
    // to which are connected the nodes. All register numbers are unique
    int AdjVar[NUM_NODES][indegmax];
    
    //Initialize AdjVar[NUM_NODES][indegmax]
    for (i=0;i<NUM_NODES ;i++)
    {
        for(k=0;k<indegmax;k++)
        {
          AdjVar[i][k]=0;
        }       
    }
    
    // The following loop randomly assigns an in-degree value to each node
    // from the set [indegmin,indegmax]
    fprintf(fpGraphDetails,"Min/Max in-degree for any node = %d/%d\n",indegmin,indegmax);
    for (i=0;i<NUM_NODES ;i++)
    {
        NodeChar[i].in_degree =randomNumGen(indegmin,indegmax);
        fprintf(fpGraphDetails,"NodeChar[%d].in_degree=%d\n",i,NodeChar[i].in_degree);
    }
    fprintf(fpGraphDetails,"====================================\n");

    // The following loop randomly assigns a control step (cstep)number to each node
    // Based on the number of nodes (=randomnum) selected to have same cstepvalue
    k=0;l=0;
    cstepVal=1;
    while(k<NUM_NODES)                      // Loop over all the nodes
    {
        randomnum = randomNumGen(p,q);
        printf("%d",randomnum);
        printf("\n");
        if((k+randomnum-1) < NUM_NODES)     //Boundary check for number of nodes
            l = k+randomnum-1;
        else
            l = NUM_NODES-1;  
        for(i=k;i<= l;i++)
         {
              AdjMat[i][NUM_NODES]= cstepVal;
              NodeChar[i].cstep = cstepVal;
              printf("NodeChar[%d].cstep=%d",i,NodeChar[i].cstep);
              printf("\n");  
         }
        k=k+randomnum;
        cstepVal++;
    }
    
    TotalNumCsteps = cstepVal-1;
    fprintf(fpGraphDetails,"Min/Max No. of Nodes in any c-step = %d/%d\n",p,q);
    fprintf(fpGraphDetails,"Total number of csteps=%d\n",TotalNumCsteps);
    
 // Store the number of nodes in each cstep in an array CstepParallel[]
 // Say,TotalNumCsteps = 8, then CstepParallel[8] is defined
    int CstepParallel[TotalNumCsteps];

    for(i=0;i<TotalNumCsteps;i++)
    {
        k=0;
        for(j=0;j<NUM_NODES;j++)
        {
            if(AdjMat[j][NUM_NODES] == (i+1)) // Because actual cstep is (i+1) as index in C starts from 0
                k=k+1;
        }
       CstepParallel[i] = k;
    }

    for(i=0;i<TotalNumCsteps;i++)
    {
      fprintf(fpGraphDetails,"Number of Nodes in cstep-%d = %d\n", (i+1),CstepParallel[i]); 
    }
    fprintf(fpGraphDetails,"====================================\n");
 // The code at labels Create1 and Create2 create the adjacency matrix
 // Create1: A(i,j)=0 where i >= j
 // Create2: A(i,j)=1 where i < j and i(c-step) < j(c-step)
 Create1:
 for(i=0;i<NUM_NODES;i++)
 {
  for(j=0;j<=i;j++)
        {
            AdjMat[i][j]=0;
        }
 }

 Create2:   // Assigns directed edges to nodes
 j=0;m=0;n=0;t=0;regNum=0;
 for(i=0;i<(TotalNumCsteps-1);i++)
 {
     m=j+CstepParallel[i];
             for(k=j;k<(j+CstepParallel[i]);k++)
             {
                   if(AdjMat[k][m] !=1)
                    {
                        if(NodeChar[m].in_degree_temp <NodeChar[m].in_degree)
                        {
                            AdjMat[k][m] = 1;  
                            NodeChar[m].in_degree_temp = NodeChar[m].in_degree_temp +1;
                        }       
                    }
                    if(m < (j+CstepParallel[i]+CstepParallel[i+1]-1))
                        m++;
                    else
                        m=j+CstepParallel[i];;
             }
             for(n=j+CstepParallel[i]; n<=(j+CstepParallel[i]+CstepParallel[i+1]-1);n++)
             {
                     if(NodeChar[n].in_degree_temp <NodeChar[n].in_degree)
                     {
                         for(k=j;k<(j+CstepParallel[i]);k++)
                         {
                             if(NodeChar[n].in_degree_temp <NodeChar[n].in_degree)
                             {
                              if(AdjMat[k][n] !=1)  
                              {
                                AdjMat[k][n]=1 ;  
                                NodeChar[n].in_degree_temp = NodeChar[n].in_degree_temp +1;  
                              }
                              else
                                  ;
                             } 
                             else
                                 ;
                         }
                         if(NodeChar[n].in_degree_temp <NodeChar[n].in_degree)
                         {
                             for(t=0;t<(NodeChar[n].in_degree-NodeChar[n].in_degree_temp);t++)
                             {
                                AdjVar[n][t] = regNum+1;
                                regNum=regNum+1;
                             }                               
                         }
                     }
             }
        j = j+CstepParallel[i];         
 }
 
 
// Assign Operators to each node
 for(i=0;i<NUM_NODES;i++)
 {
     randomnum=randomNumGen(0,(NumOpcode));
     if(NodesFlagged[i]==1)
         strcpy(NodeChar[i].id,loadedOperator);
     else
         strcpy(NodeChar[i].id,operators[randomnum]);
 }

 for(i=0;i<NUM_NODES;i++)
 {
    fprintf(fpGraphAdjMat,"%2d  %6s",i,NodeChar[i].id);
    for(j=0;j<(NUM_NODES+1);j++)
    {
        fprintf(fpGraphAdjMat,"%3d",AdjMat[i][j]);
        fprintf(fpGraphAdjMatOnly,"%3d",AdjMat[i][j]);
    }   
    fprintf(fpGraphAdjMat,"\n");
    fprintf(fpGraphAdjMatOnly,"\n");
 }
 
 for(i=0;i<NUM_NODES;i++)
 {
     fprintf(fpGraphAdjReg,"%2d  %6s",i,NodeChar[i].id);
     for(j=0;j<indegmax;j++)
         fprintf(fpGraphAdjReg,"%3d",AdjVar[i][j]);
     fprintf(fpGraphAdjReg,"\n");
 }
 
 //For test only
 for(i=0;i<numLoadedNodes;i++)
     printf("loaded nodes =%d\n",NodesforLoad[i]);
 for(k=0;k<NUM_NODES;k++)
     printf("Nodes flagged = %d\n",NodesFlagged[k]);
 
 fclose(fpGraphAdjReg);
 fclose(fpGraphAdjMat);
 fclose(fpGraphAdjMatOnly);
 
 //The following function call calls the function PrintDigraphinDOTlang()
 PrintDigraphinDOTlang(NodeChar, AdjMat, &AdjVar[0][0], indegmax);
 free(NodeChar);
 
 return (EXIT_SUCCESS);
}


/** This function prints the graph in DOT language to be processed by GraphViz for graph visualization
 */
void PrintDigraphinDOTlang(struct Node *NodeChar1, int AdjMat1[][NUM_NODES+1],int *AdjVar1, int indegMax)
{
   
    FILE* graphfile;
    graphfile = fopen("Graphfile.dot","w");
    
    fprintf(graphfile,"digraph G {\n");
    fprintf(graphfile,"size =\"8,8\";\n");
    
    int a, b;
    int count=0;
    
    //Print the labels of nodes
    for(a=0;a<NUM_NODES;a++)
    {
      fprintf(graphfile,"%d[label=\"%s\"];\n",a,NodeChar1[a].id);  
    }
    
    //Print the labels of registers
    for(a=0;a<NUM_NODES;a++)
    {
        for(b=0;b<indegMax;b++)
        {
            if(*(AdjVar1+(indegMax*a)+b) >0)
               fprintf(graphfile,"R%d[label=\"R%d\"];\n",*(AdjVar1+(indegMax*a)+b),*(AdjVar1+(indegMax*a)+b));
            else
                ;
        }
    }
    
    for(a=0;a<NUM_NODES;a++)
    {
        count = 0;
        for(b=0;b<NUM_NODES;b++)
        {
            if(AdjMat1[a][b] == 1)
            {
                count++;                            // Stores the numbers of out edges of each node temporarily
            }
        }
        for(b=0;b<NUM_NODES;b++)
        {
            if(AdjMat1[a][b] == 1)
            {
                 fprintf(graphfile,"%d -> %d;\n",a,b);            
            }
        }
        for(b=0;b<indegMax;b++)
        {
            if(*(AdjVar1+(indegMax*a)+b) >0)
               fprintf(graphfile,"R%d -> %d;\n",*(AdjVar1+(indegMax*a)+b),a);
            else
                ;
        }
    }
    fprintf(graphfile,"}\n");
    fclose(graphfile);
}


  

