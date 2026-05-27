// #define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma pack(1)
typedef struct Student {
	unsigned int regNo;
	short int groupNo;
	char* name;
} Student, * PStudent;
//typedef struct Student Student;


typedef struct BST {

	struct BST* leftChild;
	Student* data;
	struct BST* rightChild;
}BinarySearchTree;

BinarySearchTree* createNode(Student* stud) {
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree));
	if (node != NULL) {
		node->data = stud;
		node->leftChild = NULL;
		node->rightChild = NULL;
	}
	return node;
}

#define LINE_BUFFER 256

Student* createStudent(unsigned int, short int, const char*);
void printStudent(Student*);
void deleteStudent(Student*);

BinarySearchTree* insertRoot(BinarySearchTree*, Student*);
void inOrder(BinarySearchTree*);
void deleteKey(BinarySearchTree**, unsigned int);
int getHeight(BinarySearchTree*);

int main()
{
	FILE* fp = fopen("Data.txt", "r");
	BinarySearchTree* root = NULL;
	if (fp != NULL)
	{
		char line[LINE_BUFFER];
		//char* delimiter = ",";
		char delimiter[] = { ',','\n','\0' };
		unsigned int regNo;
		short int groupNo;
		char* token = NULL;
		char* context = NULL;
		while (fgets(line, LINE_BUFFER, fp))
		{
			token = strtok_s(line, delimiter, &context);
			regNo = atoi(token);

			token = strtok_s(NULL, delimiter, &context);
			groupNo = atoi(token);

			token = strtok_s(NULL, delimiter, &context);

			Student* stud = createStudent(regNo, groupNo, token);

			root = insertRoot(root, stud);

		}
		inOrder(root);
		deleteKey(&root, 13000);
		printf("\n------------AFTER DELETION---------------\n");
		inOrder(root);

		int height = getHeight(root);

		printf("Height= %d", height);
	}
}

BinarySearchTree* findMin(BinarySearchTree* root) {
	while (root != NULL && root->leftChild != NULL) {
		root = root->leftChild;
	}
	return root;
}
void deleteKey(BinarySearchTree** root, unsigned int key) {
	if ((*root)->data->regNo > key)
		deleteKey(&(*root)->leftChild, key);
	else if ((*root)->data->regNo < key)
		deleteKey(&(*root)->rightChild, key);
	else {
		if ((*root)->leftChild == NULL && (*root)->rightChild == NULL) {
			deleteStudent((*root)->data);
			free((*root));
			(*root) = NULL;
		}
		else if ((*root)->leftChild == NULL || (*root)->rightChild == NULL) {
			BinarySearchTree* desc = ((*root)->leftChild != NULL) ? (*root)->leftChild :
				(*root)->rightChild;
			deleteStudent((*root)->data);
			free((*root));
			(*root) = desc;
		}
		else
		{
			BinarySearchTree* minDesc = findMin((*root)->rightChild);
			Student* aux = (*root)->data;
			(*root)->data = minDesc->data;
			minDesc->data = aux;
			deleteKey(&(*root)->rightChild, minDesc->data->regNo);
		}
	}
}

int getHeight(BinarySearchTree* root) {
	if (root == NULL)
		return 0;
	else {
		return(1 + max(getHeight(root->leftChild), getHeight(root->rightChild)));
	}
}

BinarySearchTree* insertRoot(BinarySearchTree* root, Student* stud) {
	if (root == NULL) {
		return createNode(stud);
	}

	if (root->data->regNo < stud->regNo) {
		root->rightChild = insertRoot(root->rightChild, stud);
	}
	else if (root->data->regNo > stud->regNo) {
		root->leftChild = insertRoot(root->leftChild, stud);
	}
	else {
		Student* tmp = root->data;
		root->data = stud;
		deleteStudent(tmp);
	}
	return root;
}

void inOrder(BinarySearchTree* root) {
	if (root) {
		inOrder(root->leftChild);
		printStudent(root->data);
		inOrder(root->rightChild);
	}
}

void deleteStudent(Student* pStud)
{
	if (pStud != NULL)
	{
		if (pStud->name != NULL)
			free(pStud->name);
		free(pStud);
	}
}
void printStudent(Student* pStud)
{
	if (pStud != NULL)
		printf("RegNo=%d, GroupNo=%d, Name=%s\n",
			pStud->regNo,
			pStud->groupNo,
			pStud->name);
}

Student* createStudent(unsigned int regNo,
	short int groupNo,
	const char* name)
{
	Student* pStud = (Student*)malloc(sizeof(Student));
	if (pStud != NULL)
	{
		pStud->regNo = regNo;
		pStud->groupNo = groupNo;
		pStud->name = (char*)malloc(strlen(name) + 1);
		if (pStud->name != NULL)
		{
			strcpy_s(pStud->name, strlen(name) + 1, name);
		}
		else
		{
			free(pStud);
			pStud = NULL;
		}
	}
	return pStud;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#pragma pack(1)

typedef struct Student
{
	unsigned int regNo;
	unsigned short group;
	char* name;
} Student, * PStudent;
typedef struct Neighbour
{
	struct Vertex* reference;
	struct Neighbour* next;
}Neighbour;
typedef struct Vertex
{
	Student* info;
	struct Vertex* next;
	Neighbour* neighbours;
}Vertex;
Vertex* createVertex(Student*);
Vertex* addVertex(Vertex*, Student*);
void addEdges(Vertex*, unsigned int, unsigned int);
//typedef struct Student Student;
//typedef struct Student* PStudent;
#define LINE_SIZE 256

PStudent createStudent(unsigned int, unsigned short, const char*);
void printStudent(Student*);
void deleteStudent(Student*);

int main()
{
	Vertex* graph = NULL;
	int noVertices = 0;
	FILE* pFile = fopen("Data.txt", "r");
	if (pFile != NULL)
	{
		char line[LINE_SIZE];
		//char* delimiter = ",";
		char delimiter[] = { ',','\n','\0' };
		char* token = NULL;
		char* context = NULL;
		unsigned int regNo;
		unsigned short groupNo;
		while (fgets(line, LINE_SIZE, pFile))
		{
			token = strtok_s(line, delimiter, &context);
			regNo = atoi(token);
			//printf("Remaining line: %s\n", context);

			token = strtok_s(NULL, delimiter, &context);
			groupNo = atoi(token);
			//printf("Remaining line: %s\n", context);

			token = strtok_s(NULL, delimiter, &context);
			//printf("Remaining line: %s\n", context);
			Student* pStud = createStudent(regNo, groupNo, token);

			graph = addVertex(graph, pStud);
			noVertices++;
		}

		addEdges(graph, 8700, 4500);
		addEdges(graph, 8700, 17000);
		addEdges(graph, 3000, 17000);
		addEdges(graph, 3000, 15000);
		addEdges(graph, 3000, 13000);
		addEdges(graph, 4500, 15000);
		addEdges(graph, 17000, 13000);
	}

	return 0;
}

Vertex* findVertex(Vertex* list, unsigned int key)
{
	while (list && list->info->regNo != key)
	{
		list = list->next;
	}
	return list;
}
Neighbour* insertNeighbour(Neighbour* neighbours, Vertex* vertex)
{
	Neighbour* node = (Neighbour*)malloc(sizeof(Neighbour));
	if (node != NULL)
	{
		node->reference = vertex;
		node->next = neighbours;
	}
	return node;
}
void addEdges(Vertex* listOfVertices,
	unsigned int src, unsigned int dst)
{
	Vertex* srcVertex = findVertex(listOfVertices, src);;
	Vertex* dstVertex = findVertex(listOfVertices, dst);;

	if (srcVertex != NULL && dstVertex != NULL)
	{
		srcVertex->neighbours = insertNeighbour(srcVertex->neighbours, dstVertex);
		dstVertex->neighbours = insertNeighbour(dstVertex->neighbours, srcVertex);
	}
}

Vertex* createVertex(Student* stud)
{
	Vertex* node = (Vertex*)malloc(sizeof(Vertex));
	if (node != NULL)
	{
		node->info = stud;
		node->neighbours = NULL;
		node->next = NULL;
	}
	return node;
}

Vertex* addVertex(Vertex* listOfVertices, Student* stud)
{
	Vertex* vertex = createVertex(stud);
	if (vertex != NULL)
	{
		vertex->next = listOfVertices;
		listOfVertices = vertex;
	}
	return listOfVertices;
}

void deleteStudent(Student* pStud)
{
	if (pStud != NULL)
	{
		if (pStud->name != NULL)
			free(pStud->name);
		free(pStud);
	}
}

void printStudent(Student* pStud)
{
	if (pStud != NULL)
	{
		printf("Name: %s, regNo: %d, groupNo: %d\n",
			pStud->name,
			pStud->regNo,
			pStud->group);
	}
}

PStudent createStudent(unsigned int regNo,
	unsigned short groupNo,
	const char* name)
{
	Student* stud = (Student*)malloc(sizeof(Student));
	if (stud != NULL)
	{
		stud->regNo = regNo;
		stud->group = groupNo;
		stud->name = (char*)malloc(strlen(name) + 1);
		if (stud->name != NULL)
		{
			strcpy_s(stud->name, strlen(name) + 1, name);
		}
		else
		{
			free(stud);
			stud = NULL;
		}
	}
	return stud;
}
