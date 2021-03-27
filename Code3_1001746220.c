//Tuyen Hoang 1001746220

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define MAX 10

typedef struct
{
	char label[6];
	int distance;
	int previous;
	int visited;
}Vertex;

typedef struct
{
	char path[6];
}Paths;

FILE *OpenFile(int argc,char *argv[], char mode[])
{
	FILE *FileOpened = NULL;
	FileOpened = fopen(argv[1], mode);
	if(FileOpened == NULL)
	{
		printf("\nMust be run with an input file name.");
		char FileName[20];
		do
		{
			printf("\nEnter a file name at the promt: ");
			scanf("%s", FileName);
			FileOpened = fopen(FileName, mode);
			if(FileOpened == NULL)
			{
				printf("\nCould not open a file named %s.", FileName);
			}
		}while(FileOpened == NULL);
	}
	return FileOpened;
}

int CreateAdjacencyMatrix(int AdjMatrix[][MAX], Vertex VertexArray[], char *argv[], int argc)
{
	int start = 0, end = 0, weight = 0;
	int i = 0, j = 0;
	char buffer[100] = {};
	FILE *FH = OpenFile(argc,argv, "r");
	char *label;
	int loop_count;
	int count = 0;
	while(fgets(buffer, sizeof(buffer)-1, FH))
	{
		count++;
	}
	fseek(FH, 0, SEEK_SET);
	
	//initialize adjacency matrix to -1
	for(i=0; i<MAX; i++)
		for(j=0; j<MAX; j++)
			AdjMatrix[i][j] = -1;
	
	i = 0;
	while(fgets(buffer, sizeof(buffer)-1, FH))
	{
		if(buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		
		int labelLength = strchr(buffer, ',') - buffer;
		loop_count = (strlen(buffer) - labelLength)/4;
		label = strtok(buffer, ",");
		strcpy(VertexArray[i].label, label);
		while(loop_count != 0)
		{
			start = i;
			end = atoi(strtok(NULL, ","));
			weight = atoi(strtok(NULL, ","));
			AdjMatrix[start][end] = weight;
			loop_count--;
		}
		i++;
	}
	fclose(FH);
	return count;
}

int main(int argc, char *argv[])
{
	int i, j;
	//create the VertexArray
	Vertex VertexArray[MAX] = {};
	//initialize VertexArray
	int distance = INT_MAX;	
	int previous = -1;
	int visited = 0;
	for(i=0; i<MAX; i++)
	{
		VertexArray[i].distance = distance;
		VertexArray[i].previous = previous;
		VertexArray[i].visited = visited;
	}
	//create AdjMatrix
	int AdjMatrix[MAX][MAX];
	int VertexCount = CreateAdjacencyMatrix(AdjMatrix, VertexArray, argv, argc);

	
	#ifdef PRINTIT
	printf("\n");
	for(i=0; i<VertexCount; i++)
	{
		for(j=0; j<VertexCount; j++)
		{
			printf("%6d\t", AdjMatrix[i][j]);
		}
		printf("\n");
	}
	#endif
	
	//perform Dijkstra Algorithm
	//*************************************************************************************************
	
	//promt for starting vertex
	char start_label[6] = {};
	int StartVertex;
	printf("\nWhat is the starting vertex? ");
	scanf("%s", start_label);
	for(i = 0; i < VertexCount; i++)
	{
		if((strcmp(VertexArray[i].label, start_label) == 0))
		{
			StartVertex = i;
		}
	}
	
	if(StartVertex<0 || StartVertex >= VertexCount)
	{
		printf("\nVertex not found. Exiting");
		exit(0);
	}
	
	int x, dofu=0, cofuv=0, dofv=0, SmallestVertexIndex;
	int CurrentVertex = StartVertex;
	VertexArray[StartVertex].distance = 0;
	VertexArray[StartVertex].previous = -1;
	VertexArray[StartVertex].visited = 1;
	for (x = 0; x < VertexCount-1; x++)
	{
		for(i = 0; i < VertexCount; i++)
		{
			if (AdjMatrix[CurrentVertex][i] != -1 && !VertexArray[i].visited)
			{
				dofu = VertexArray[CurrentVertex].distance;
				cofuv = AdjMatrix[CurrentVertex][i];
				dofv = VertexArray[i].distance;
			}	
			if (dofu + cofuv < dofv)
			{
				dofv = dofu + cofuv;
				VertexArray[i].distance = dofv;
				VertexArray[i].previous = CurrentVertex;
			}
		}
		
		for(i = 0; i < VertexCount; i++)
		{
			// Check all vertices for distance from CurrentVertex and update
			SmallestVertexIndex = -1;
			int SmallestVertex = INT_MAX;
			for(i = 0; i < VertexCount; i++)
			{
				if (!VertexArray[i].visited)
				{
					if (VertexArray[i].distance < SmallestVertex)
					{
					SmallestVertex = VertexArray[i].distance;
					SmallestVertexIndex = i;
					}
				}
			}
			CurrentVertex = SmallestVertexIndex;
			VertexArray[CurrentVertex].visited = 1;
		}
	}

	#ifdef PRINTIT
	printf("\n\nI\tL\tD\tP\tV\n");
	for(i=0; i<VertexCount; i++)
	{
		printf("%d\t%s\t%d\t%d\t%d\n", i, VertexArray[i].label, VertexArray[i].distance, VertexArray[i].previous, VertexArray[i].visited);
	}
	printf("\n");
	#endif
	
	//prompt for destination vertex
	char end_label[6] = {};
	int EndVertex;
	printf("\nWhat is the destination vertex? ");
	scanf("%s", end_label);
	for(i = 0; i < VertexCount; i++)
	{
		if((strcmp(VertexArray[i].label, end_label) == 0))
		{
			EndVertex = i;
		}
	}
	
	if(EndVertex<0 || EndVertex >= VertexCount)
	{
		printf("\nVertex not found. Exiting");
		exit(0);
	}
	
	CurrentVertex = EndVertex;
	int length = 0;
	length = VertexArray[EndVertex].distance;
	//char path[MAX];
	
	Paths shortest_path[MAX] = {};
	i=0;
	while(CurrentVertex != -1)
	{
		strcpy(shortest_path[i].path, VertexArray[CurrentVertex].label);
		CurrentVertex = VertexArray[CurrentVertex].previous;
		i++;
	}
	printf("\nThe path from %s to %s is %s", start_label, end_label, start_label);
	i=0;
	while((shortest_path[i]).path[0] != '\0')
	{
		i++;
	}
	for(j=i-2; j>=0; j--)
	{
		printf("->%s", shortest_path[j].path);
	}
	printf(" and has length of %d.", length);
	return 0;
}
