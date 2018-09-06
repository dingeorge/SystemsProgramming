#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>

typedef struct Node {
	char * data;
	struct wordNode * next;
	struct string_LL * head;
}wordNode;

typedef struct string_LL {
	int counter;
	char * file_Name;
	struct string_LL * next;
	struct string_LL * prev;
}stringLL;
wordNode * head = NULL;

bool isChar (char letter) {
	int asciiVal = letter;
	if (asciiVal >= 65 && asciiVal <= 90) {
		return true;
	}
	else if (asciiVal >= 97 && asciiVal <= 122) {
		return true;
	}
	return false;
}

bool isNum (char letter) {
	int asciiVal = letter;
	if (asciiVal >= 48 && asciiVal <= 57) {
		return true;
	}
	return false;
}

int stringcompare(char * s1, char * s2){
	char l1 = s1[0];
	char l2 = s2[0];
	if (l1 == '\0' && l2 == '\0'){
		return 0;
	}
	if (l1 == '\0'){
		return -1;
	}
	if (l2 == '\0'){
		return 1;
	}
	if(l1 >= '0' && l1 <= '9'){
		if(isChar(l2)){
			return 1;
		}
		if(l2 == '.'){
			return -1;
		}
		if(l2 > l1){
			return -1;
		}
	}
	if(l2 >= '0' && l2 <= '9'){
		if(isChar(l1)){
			return -1;
		}
		if(l1 == '.'){
			return 1;
		}
		if(l1 > l2){
			return 1;
		}
	}
	if(isChar(l1) && (l1 != l2)){
		if(isChar(l2)){
			if(l1 < l2){
				return -1;
			}
			if(l2 < l1){
				return 1;
			}
		}
		return -1;
	}
	if(isChar(l2) && (l1 != l2)){
		if(isChar(l1)){
			if(l1 < l2){
				return -1;
			}
			if(l2 < l1){
				return 1;
			}
		}
		return 1;
	}
	return stringcompare(++s1, ++s2);
}
void  insert (char * string, char * fileName) { //using hashtable of wordNodes
	char * lowerStr = (char*)malloc(strlen(string)+1);
        int i;
        for (i = 0; i<=strlen(string )+ 1; i++) {
		lowerStr[i] = tolower(string[i]);
	}
        wordNode * current = head, * previous = NULL;
        while (current && (stringcompare(current->data, lowerStr) <= 0) ) {
                if (stringcompare(current->data, lowerStr) == 0) {
                        free(lowerStr);
                        setNode(fileName, current);
                        return;
                }
                previous = current;
                current = current->next;
        }

        wordNode * new = (wordNode*)malloc(sizeof(wordNode));
        new->data = (char*)malloc(strlen(string)+1);
        strcpy(new->data, lowerStr);
        free(lowerStr);
        new->next = NULL;
        new->head = NULL;
        if(head) {
                if (previous == NULL) { //inserting to beginning of list
                        wordNode * next = head;
                        head = new;
                        new->next = next;
                }
                else {
                        wordNode * link = previous->next;
                        previous->next = new;
                        new->next = link;
                }
        }
        else {
                head = new;
	}
        setNode(fileName, new);
}
void setNode(char*str, wordNode * inp) {
        stringLL * root = inp->head;
        //Makes string lowercase
	char * lowerStr = (char*)malloc(strlen(str)+1);
	int i;
	for (i = 0;i<=strlen(str)+1;i++) {
		lowerStr[i] = tolower(str[i]);
	}
	//Sees if string is in hashmap
	stringLL * current = root,*previous = NULL;
	while (current) {
		if (stringcompare(current->file_Name, lowerStr) == 0) {
			free(lowerStr);
			current->counter++;
			return;
		}
		previous = current;
		current = current->next;
	//If string isn't in hashmap, adds a new stringLL to the end
	stringLL * new = (stringLL*)malloc(sizeof(stringLL));
	new->file_Name = (char*)malloc(strlen(str) + 1);
	strcpy(new->file_Name, lowerStr);
	free(lowerStr);
	current = previous;
	new->next = NULL;
	new->prev = NULL;
	new->counter = 1;
	if (!root) {
		root= new;
	}
	else {
		previous->next = new;
		new->prev = previous;
	}
	inp->head = root;
	}
}
void filesorter(wordNode * head) {
	stringLL * sorted = head->next;
	sorted->next = NULL;
	stringLL * headFileNode = head->next;
	stringLL * ptr = sorted;
	while (headFileNode != NULL) {
		headFileNode = headFileNode->next;
		while (ptr != NULL) {
			stringLL * prev = NULL;
			if (ptr == NULL){
				stringLL * newNode = headFileNode;
				prev->next = newNode;
				ptr = sorted;
				break;
			}
			else if (ptr->counter != headFileNode->counter) {
				if (ptr->counter < headFileNode->counter) {
					stringLL * newNode = headFileNode;
					newNode->next = prev;
					ptr->next = newNode;
					ptr = sorted;
					break;
				}
				prev = ptr;
				ptr = ptr->next;
			}
			else {
				while (strcmp(ptr->file_Name, headFileNode->file_Name) < 0 || (ptr->counter < headFileNode->counter)) {
					prev = ptr;
					ptr = ptr->next;
					if (ptr == NULL) {
						break;
					}
				}
				stringLL * newNode = headFileNode;
				newNode->next = ptr;
				prev->next = newNode;
				ptr = sorted;
				break;
			}
		}
	}
}					
void sortNode (wordNode * input) {
	stringLL * root = input->head;
	stringLL * j = root;
	stringLL * k;
	while (j) {
		k = j->next;
		while (k) {
			if (j->counter < k->counter || (j->counter == k->counter && stringcompare(j->file_Name, k->file_Name)) > 0) {
				char * temp = j->file_Name;
				int a = j->counter;
				j->file_Name = k->file_Name;
				j->counter = k->counter;
				k->file_Name = temp;
				k->counter = a;
			}
			k = k->next;
		}
		j = j->next;
	}
	input->head = root;
}
void freeNodes (stringLL * root) {
	stringLL * current = root, * next = NULL;
	while (current) {
		next = current->next;
		free(current->file_Name);
		free(current);
		current = next;
	}
}
void freeAll() {
	wordNode * current = head, * next = NULL;
	while (current) {
		next = current->next;
		free(current->data);
		freeNodes(current->head);
		free(current);
		current = next;
	}
}
void printmethod(char * output) {
	wordNode *current = head;
        FILE *fp;
        fp = fopen(output, "w+");
        fprintf(fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<fileIndex>\n");
        while(current) {
                fprintf(fp,"\t<data text=\"%s\">\n", current->data);
                sortNode(current);
                stringLL * curr = current->head;
                while(curr) {
                        fprintf(fp,"\t\t<file name=\"%s\">%d</file>\n", curr->file_Name, curr->counter);
                        curr = curr->next;
                }
                current = current->next;
                fprintf(fp,"\t</data>\n");
        }
        fprintf(fp,"</fileIndex>");
        freeAll();
        fclose(fp);
}
void indexFile (char * text, char * filename) {
	FILE * fp;
	fp = fopen (text, "r");
	if (fp == NULL) {
		printf ("Error: file cannot be opened\n");
		exit(1);
	}
	int i = 0;
	char s[1024];
	int c = 0;
	while (c != EOF) {
		c = tolower(fgetc(fp));
		if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
			s[i++] = c; //check to see if it's a token in insert
		}
		else if (i != 0 && s[0] >= 'a' && s[0] <= 'z') {
			s[i] = '\0';
			insert (s, filename);
			i = 0;
		}
		else {
			i = 0;
		}
	}
	fclose(fp);
}
void indexDirectory(char *directory) {
	struct dirent *structdirect;
	DIR * dir;
	while((dir = opendir(directory)) && (structdirect = readdir(dir))) {
		if(strcmp(structdirect->d_name,".")!=0 && strcmp(structdirect->d_name,"..")!=0) {
			if(structdirect -> d_type == DT_REG) {
				char str[strlen(structdirect->d_name) + strlen(directory) + 2];
				strcpy(str, directory);
				strcat(str, "/");
				strcat(str, structdirect->d_name);
				indexFile(str, structdirect->d_name);
			}
			else if(structdirect -> d_type == DT_DIR) {
				char str[strlen(structdirect->d_name) + strlen(directory) + 2];
				strcpy(str, directory);
				strcat(str, "/");
				strcat(str, structdirect->d_name);
				indexDirectory(str);
			}
		}
	}
	closedir(dir);
}
int main (int argc, char ** argv) {
        if (argc != 3) {
                printf ("Error: needed 2 arguments to run program\n");
                exit(1);
        }
        char * filename = argv[2];
        if (strlen(filename) != 0 && filename[0] != '/' && filename[0] != '.') {
                if (opendir(filename)) {
                        indexDirectory(filename + 1);
                }
                else {
                        char * str = malloc (sizeof(filename + 1));
                        strcpy(str, filename);
                        int counter = 0;
                        char current = str[0];
                        char * ptr = str;
                        while (current != '\0') {
                                if (current == '/') {
                                        ptr = &str[counter + 1];
                                }
                        current = str[counter++];
                        }
                indexFile(filename, ptr);
                free (str);
                }
        }
        printmethod(argv[1]);
        return 0;
}

