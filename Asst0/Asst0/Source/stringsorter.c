#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct linked_list{  //structure for node that contains char* (star) and a next pointer.
	char * data;
	struct linked_list * next;
}node; 

bool isCharacter(char letter){ //simple boolean method to tell if a char variable[a] = character with ascii table reference.

	int asciiVal = letter;
	if (letter >= 65 && letter <= 90){
		return true;
	}
	else if (letter >= 97 && letter <= 122){
		return true;
	}
	
	return false;
}

bool isUpperCase(char letter){ //simple boolean method to tell if the character is uppercase, uses same code as isCharacter from above.

	int asciiVal = letter;
	if (asciiVal >= 65 && asciiVal <= 90){
		return true;
	}
	
	return false;
}

void insertChar(char * string, node ** hashtable){ // void method taking a string and inserting and sorting it into the appropriate space into the given node hashtable.
	char key = string[0]; 
	int asciiVal = key; //int asciiVal will be our key.  
	node * insertNode = (node*)malloc(sizeof(node));
	insertNode -> data = (char*)malloc(strlen(string)*sizeof(char) + 1);
	strcpy(insertNode->data, string);
	insertNode -> next = NULL;
	
	if (isUpperCase(key)){ //Uppercase > lowercase in terms of ranking, therefore holds higher in the array table when printf'ing.
		asciiVal = asciiVal - 65;
	}
	else{
		asciiVal = asciiVal - 71;
	}
	 
	if (hashtable[asciiVal] == NULL){ // if nothing exists in the hashtable, first element is whatever should be inserted.
		hashtable[asciiVal] = insertNode;
	}
	else{
		node *firstEntry = hashtable[asciiVal];
		node *temp = firstEntry;
		while (strcmp(firstEntry->data, string) <= 0){ //cmp method, we have temp pointer always -> next'ing the new node, unless the new node is alphabetically lesser than the firstEntry.
			temp = firstEntry;
			firstEntry = firstEntry->next;
			if (firstEntry == NULL){
				break;
			}
		}
		if (temp == firstEntry){ // if temp == firstEntry, our string didn't pass one iteration of the while loop above, and therefore is the new head.
			insertNode -> next = temp;
			hashtable[asciiVal] = insertNode;
		} 
		else if (temp -> next == NULL){ // simple if firstEntry == null scenario.
			temp -> next = insertNode;
			}
		else{ // every other case
			insertNode -> next = firstEntry;
			temp->next = insertNode;
		}	
	}				
}


int main(int argc, char ** argv){

	if (argc != 2){ // if argc != 2 (a.out + our single string), return error.
		printf("Incorrect amount of Arguments: Error");
		return 0;
	}

	int i;
	int j = 0;
	int counter1 = 0;
	int counter2 = 0;
	bool wordstart = false;

	char * subbuffer; //initialize subbuffer here, malloc later when we have size of each individual string.
	char * word = argv[1]; //our complete string = the second argument of our terminal input.
	
	node *alphatable[52];//initialize our 52 character node array.  We use a simple 52 character array to have a starting sort optimization instead of a increasing arraylist for Uppercase A - Z, followed by lowercase a - z, with a sort method linking the nodes together.

	for (i = 0; i < 52; i++){
		alphatable[i] = NULL;
	}
	
	for (i = 0; i < strlen(word); i++){ //iterate through entire string by character.
		if (isCharacter(word[i]) == true){//finds a character, start the subbuffer counter.
			if (wordstart == false){//if its the first character.
				counter1 = i;
				counter2 = i;
				wordstart = true;
			}
			else if (wordstart == true){//if its not the first character, but the iteration still fits under a character.
				counter2 = i;
			}
		}
		else{
			if (wordstart == true){//finds delimiter, ends string counter.
				subbuffer = (char*)malloc(sizeof(char) * (counter2 - counter1 + 2)); //first malloc subbuffer to be the size of the string.
				strncpy(subbuffer, &word[counter1], (counter2 - counter1 + 1)); //copy the string.
				subbuffer[strlen(subbuffer)] = '\0'; 
				insertChar(subbuffer, alphatable); //insert into hashtable
				free(subbuffer); //free memory after usage.
				wordstart = false; //sets wordstart to false.
			}
 		}
	}
	
	if (wordstart == true){ //if the loop above ends on a character, then the loop will end, however, the last string will not be inputted into the hashtable.  Therefore, we add a exception statement here to check if there is a string that was cut off.
		subbuffer = (char*)malloc(sizeof(char) * (counter2 - counter1 + 2));
		strncpy(subbuffer, &word[counter1], (counter2 - counter1 + 1));
		subbuffer[strlen(subbuffer)] = '\0';
		insertChar(subbuffer, alphatable);
		free(subbuffer); // free memory after usage; 
	}
	

	for (i = 0; i < 52; i++){ //we iterate through our finished array and print out every single linked list that the hashtable[i] exists;
		if (alphatable[i] != NULL){
			node * head = alphatable[i];
			while (head != NULL){
				printf("%s\n", head->data);
				node * temp = head;
				head = head->next;
				free(temp);
			}
		}
	}
}
