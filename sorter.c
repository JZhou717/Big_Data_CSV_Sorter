#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "sorter.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>


//volatile int runningThreadCount = 0;
pthread_mutex_t thread_ID_list = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_open_lock = PTHREAD_MUTEX_INITIALIZER;

threadIds * threadIdsHead = NULL;
threadIds* threadIdsRear = NULL;

movie* freePtr(movie* node){
	if(node == NULL){
		return NULL;
	}
	
	free(node->color);
	free(node->director_name);
	free(node->actor_2_name);
	free(node->genres);
	free(node->actor_1_name);
	free(node->movie_title);
	free(node->actor_3_name);
	free(node->plot_keywords);
	free(node->movie_imdb_link);
	free(node->language);
	free(node->country);
	free(node->content_rating);
	free(freePtr(node->next));
	
	
	
	return NULL;

}

//The function will be given a char** variable that holds all of the parsed content to be inputted into each Linked List node. Then the function will initialize and return a node to be inserted at the end of the linked list structure.
movie * createNode(char** catToked, int hasQuotes){
	movie * ptr = (movie*)malloc(sizeof(movie));
	
	ptr->color = strdup(catToked[0]);
	ptr->director_name = strdup(catToked[1]);
	ptr->num_critic_for_reviews = atol(catToked[2]);
	ptr->duration = atol(catToked[3]); 
	ptr->director_facebook_likes = atol(catToked[4]);
	ptr->actor_3_facebook_likes = atol(catToked[5]);
	ptr->actor_2_name = strdup(catToked[6]);
	ptr->actor_1_facebook_likes = atol(catToked[7]);
	ptr->gross = atol(catToked[8]);
	ptr->genres = strdup(catToked[9]);
	ptr->actor_1_name = strdup(catToked[10]);
	ptr->movie_title = strdup(catToked[11]);
	ptr->num_voted_users = atol(catToked[12]);
	ptr->cast_total_facebook_likes = atol(catToked[13]);
	ptr->actor_3_name = strdup(catToked[14]);
	ptr->facenumber_in_poster = atol(catToked[15]);
	ptr->plot_keywords = strdup(catToked[16]);
	ptr->movie_imdb_link = strdup(catToked[17]);
	ptr->num_user_for_reviews = atol(catToked[18]);
	ptr->language = strdup(catToked[19]);
	ptr->country = strdup(catToked[20]);
	ptr->content_rating = strdup(catToked[21]);
	ptr->budget = atol(catToked[22]);
	ptr->title_year = atol(catToked[23]);
	ptr->actor_2_facebook_likes = atol(catToked[24]);
	ptr->imdb_score = atof(catToked[25]);
	ptr->aspect_ratio= atof(catToked[26]);
	ptr->movie_facebook_likes = atol(catToked[27]);
	if(hasQuotes == 0){
		ptr->quotes = 0;
	}
	else{
		ptr->quotes = 1;
	}
	
	ptr->next = NULL;

	return ptr;

}

//This method accepts a string and removes all leading and trailing spaces.
char* spaceTrim(char* string){
	char* endOfString;
	
	
	if(string == NULL){			
		return NULL;
	}
	
	//While the value stored at the front of the char* string is a space, increment the string address by the size of a character (1 byte)
	while(*string == ' '){
		string = string + 1;
	}
	
	if(*string == ' '){
		return NULL;
	}
	
	endOfString = string + strlen(string) - 1;
	
	//Same concept as deleting leading spaces, but we are also making sure we stop once we reach the last actual character in the string.
	while(endOfString > string && (*endOfString == ' ')){
		endOfString = endOfString - 1;
	}
	
	//Set null terminator again
	*(endOfString + 1) = '\0';
	
	return string;
}

//This function determines what field we will be sorting by based on the user input.

int sortByCategory(char* sortColumnName){
	if(strcmp(sortColumnName, "color") == 0) {
		return 0;
	}
	else if(strcmp(sortColumnName, "director_name") == 0) {
		return 1;
	}
	else if(strcmp(sortColumnName, "num_critic_for_reviews") == 0) {
		return 2;
	}
	else if(strcmp(sortColumnName, "duration") == 0) {
		return 3;
	}
	else if(strcmp(sortColumnName, "director_facebook_likes") == 0) {
		 return 4;
	}
	else if(strcmp(sortColumnName, "actor_3_facebook_likes") == 0) {
		return 5;
	}
	else if(strcmp(sortColumnName, "actor_2_name") == 0) {
		return 6;
	}
	else if(strcmp(sortColumnName, "actor_1_facebook_likes") == 0) {
		return 7;
	}
	else if(strcmp(sortColumnName, "gross") == 0) {
		return 8;
	}
	else if(strcmp(sortColumnName, "genres") == 0) {
		return 9;
	}
	else if(strcmp(sortColumnName, "actor_1_name") == 0) {
		return 10;
	}
	else if(strcmp(sortColumnName, "movie_title") == 0) {
		return 11;
	}
	else if(strcmp(sortColumnName, "num_voted_users") == 0) {
		return 12;
	}
	else if(strcmp(sortColumnName, "cast_total_facebook_likes") == 0) {
		return 13;
	}
	else if(strcmp(sortColumnName, "actor_3_name") == 0) {
		return 14;
	}
	else if(strcmp(sortColumnName, "facenumber_in_poster") == 0) {
		return 15;
	}
	else if(strcmp(sortColumnName, "plot_keywords") == 0) {
		return 16;
	}
	else if(strcmp(sortColumnName, "movie_imdb_link") == 0) {
		return 17;
	}
	else if(strcmp(sortColumnName, "num_user_for_reviews") == 0) {
		return 18;
	}
	else if(strcmp(sortColumnName, "language") == 0) {
		return 19;
	}
	else if(strcmp(sortColumnName, "country") == 0) {
		return 20;
	}
	else if(strcmp(sortColumnName, "content_rating") == 0) {
		return 21;
	}
	else if(strcmp(sortColumnName, "budget") == 0) {
		return 22;
	}
	else if(strcmp(sortColumnName, "title_year") == 0) {
		return 23;
	}
	else if(strcmp(sortColumnName, "actor_2_facebook_likes") == 0) {
		return 24;
	}
	else if(strcmp(sortColumnName, "imdb_score") == 0) {
		return 25;
	}
	else if(strcmp(sortColumnName, "aspect_ratio") == 0) {
		return 26;
	}
	else if(strcmp(sortColumnName, "movie_facebook_likes") == 0) {
		return 27;
	}
	else {
		printf("Fatal Error: Please input a proper category title\n");
		pthread_exit(0);
	}
}

void * sortFile(void * args){
	char * fileName = NULL;
	fileName = strdup(((sortFileArgs *)args)->fileName);
	char** argv = ((sortFileArgs *)args)->argv;
	int sortingBy = ((sortFileArgs *)args)->sortingBy;
	char * path = NULL;
	path = strdup(((sortFileArgs *)args)->path);
	int colLoc = ((sortFileArgs *)args)->colLoc;

	printf("%u,", (unsigned int) pthread_self());
	
	//printf("current path = %s\n", path);
	
	//printf("REAR TID %u\n", (unsigned int) threadIdsRear->tid);
	
	//printf("outPath in sortFile = %s\n", path);
	movie * headMovies = NULL;
	
	//printf("current fileName = %s\n", fileName);
	
	pthread_mutex_lock(&file_open_lock);
	char* filePath = (char*)malloc(sizeof(char) * strlen(path) + 30);
	memcpy(filePath, path, strlen(path));
	strcat(filePath, "/");
	strcat(filePath, fileName);
	FILE* filePointer = fopen(filePath, "r");
	
	if(filePointer == NULL){
		printf("Fatal Error: The file <%s> does not exist.\n", filePath);
		pthread_exit(0);
	}
	free(filePath);
	char* categories = "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes";
	char* line = (char*)malloc(sizeof(char)*500);
	memset(line, '\0', 500);
	
	fgets(line, 500, filePointer);
	
	//Remove any trailing characters (\n or \r) and replace them with \0
	line[strcspn(line, "\n\r")] = '\0';
	
	if(strcmp(line, categories) !=  0){
		printf("Fatal Error: The input file \"%s\" does not adhere to specified format\n", fileName);
		fclose(filePointer);
		pthread_exit(0);
	}
	//Create new file name to output to.
	char* newFileName = (char*)malloc(sizeof(char)*((strlen(path) + strlen(fileName) + 30)));
	//Determine where endOfFileName is, subtract 4 (accounts for ".csv"). Then, concatenate "-sorted-<category>.csv".
	int endOfFileName = strlen(fileName) - 4;
	
	char* truncatedFileName = (char*)malloc((sizeof(char)*(strlen(fileName) + 1)));
	truncatedFileName = memcpy(truncatedFileName, fileName, endOfFileName);
	
	truncatedFileName[endOfFileName + 1] = '\0';
	
	//printf("truncated FN = %s\n", truncatedFileName);
	int endOfOutPath = strlen(path);
	memcpy(newFileName, path, endOfOutPath);
	strcat(newFileName, "/");
	strcat(newFileName, truncatedFileName);
	strcat(newFileName, "-sorted-");
	strcat(newFileName, argv[colLoc]);
	strcat(newFileName, ".csv");
	//printf("NEW FILE NAME: %s\n", newFileName);
	
	//printf("Printing out into the file: %s\n", newFileName);
	
	
	


	int i;
	int j;
	int hasQuotes = 0;

	
	
	//printf("\nnewFileName = %s\n", newFileName);
	FILE* outputFile = fopen(newFileName, "r");
	
	
	//If outputFile already exists, don't even bother with rest of method.
	if(outputFile != NULL){
		//printf("Fatal Error: File \"%s\" already exists.\n", newFileName);
		pthread_exit(0);
	}
	else{
		//printf("OUTPUTFILE OPENED\n\n");
		outputFile = fopen(newFileName, "w");
	}
	
	if(line == NULL){
		printf("Fatal Error: The input file is blank.\n");
		pthread_exit(0);
	}

	//Points to the last node in the linked list to add to the list faster.
	movie * rearMovies = NULL;
	rearMovies = (movie*)malloc(sizeof(movie));

	char* tempTok;
	char* quotesDetectedTok;
	char* templine = (char*)malloc(sizeof(char)*500);
	char* parseSeg1;
	char* movie_titleTok;
	char* parseSeg3;

	//catToked = tokenized data corresponding to each category
	char** catToked = (char**)(malloc(sizeof(char*) * 28));

	//Allocates memory for each character of each string. Sets each character to the null string terminator needs to go through 28 fields in the final version
	for(i = 0; i < 28; i++){
		catToked[i] = (char*)(malloc(sizeof(char) * 70));
		for(j = 0; j < 70; j++){
			catToked[i][j] = '\0';
		}
	}

	while(fgets(line, 500, filePointer) != NULL){

		//printf("This is what the line says: %s\n", line);

		memcpy(templine, line, 500);

		//We will begin by tokenize by quotation marks, which will cause one of two things. First, if there are no quotation marks, the line will be 1 whole fragment, and we can proceed as usual. However, if strtok() returns a 1 while tokenizing for quotation marks, the line will be split into 3 tokens: before the movie title, the movie title, and after the movie title.

		tempTok = strsep(&line, "\"");

		/*
		printf("value of tempTok after \" search = %s\n", tempTok);
		printf("templine after strsep is: %s\n", templine);
		printf("value of tempTok before , search = %s\n", tempTok);
		*/

		//If there were no quotations detected, we will proceed as normal.
		//printf("result of strcmp: %d\n", strcmp(tempTok, templine));
		if (strcmp(tempTok, templine) == 0){
			tempTok = strsep(&templine, ",");

			for(i = 0; i < 28 && tempTok != NULL; i++){
	
				tempTok = spaceTrim(tempTok);

				//printf("tempTok = %s ||||\n", tempTok);
	
				if(strcmp(tempTok, "") == 0){
					//printf("blank line found\n");
					catToked[i][0] = '-';
					catToked[i][1] = '1';
				}
				else{
					catToked[i] = tempTok;
				}
				//printf("Content %d of catToked: %s\n", i, catToked[i]);
	
				tempTok = strsep(&templine, ",");
			}
		}
		else{
			parseSeg1 = tempTok;
			movie_titleTok = strsep(&line, "\"");
			parseSeg3 = strsep(&line, "\"");

			quotesDetectedTok = strsep(&parseSeg1, ",");

			for(i = 0; i < 12 && quotesDetectedTok != NULL; i++){
				quotesDetectedTok = spaceTrim(quotesDetectedTok);
	
				catToked[i] = quotesDetectedTok;
	
				quotesDetectedTok = strsep(&parseSeg1, ",");
	
			}

			//We have separated out the movie title, now trim the spaces and put it into its respective field
			movie_titleTok = spaceTrim(movie_titleTok);
			hasQuotes = 1;

			catToked[11] = movie_titleTok;

			quotesDetectedTok = strsep(&parseSeg3, ",");
			//We do this twice as the first token will be NULL as there is still a comma present before the first actual field.
			quotesDetectedTok = strsep(&parseSeg3, ",");

			for(i = 12; i < 28 && quotesDetectedTok != NULL; i++){
	
				quotesDetectedTok = spaceTrim(quotesDetectedTok);
	
				catToked[i] = quotesDetectedTok;
	
				quotesDetectedTok = strsep(&parseSeg3, ",");
			}
		}

		//If there is no head node, allocate space for it and set its values. Set rear pointer = head.

		if(headMovies == NULL){
			//printf("NEW HEAD NODE CREATED\n\n");
			headMovies = createNode(catToked, hasQuotes);

			rearMovies = headMovies;
		}

		else{
			//printf("NEW NODE CREATED\n\n");
			//Allocate memory for new node added to end of list. Set its values, then increment the rear pointer, making the new node the rear pointer for the list.
			rearMovies->next = createNode(catToked, hasQuotes);

			rearMovies = rearMovies->next;
		}	
	
		//Resetting the catToked
		for(i = 0; i < 28; i++){
			for(j = 0; j < 70; j++){
				catToked[i][j] = '\0';
			}
		}

		//Change name, should never use same pointer that we freed in realloc()
		line = realloc(line, 500);
		templine = realloc(templine, 500);
		hasQuotes = 0;
	}
	pthread_mutex_unlock(&file_open_lock);
	//Given the head pointer to the start of the Linked List structure and a category to sort by, perform a merge sort.
	merge(&headMovies, sortingBy);

	movie* currPtr = (movie*)malloc(sizeof(movie));
	currPtr = headMovies;
	
	//printf("\n\nTIME TO PRINT\n\n");
	printNodes(currPtr, outputFile);

	//This section of code will free the allocated memory for each pointer, struct, etc
	//free(categories);
	//free(truncatedFileName);
	//free(newFileName);
	freePtr(headMovies);
	free(rearMovies);
	//free(currPtr);
	//free(line);
	//free(templine);
	//fclose(outputFile);
	
	//fclose(filePointer);
	
	//return childCount;
	
	return NULL;
}

//This function will recursively traverse through the original given file path. If a directory is found within the original directory, the process will fork and recursively call the traverseDirectory() function. If a file is found, the function will determine whether or not it is a CSV, and then fork if it is.
void * traverseDirectory(void * args){
	//printf("ENTER TRAVERSE\n");
	
	char * path = NULL;
	path = strdup(((traverseDirectoryArgs*)args)->path);
	char** argv = ((traverseDirectoryArgs*)args)->argv;
	int sortingBy = ((traverseDirectoryArgs*)args)->sortingBy;
	int existsNewOutDir = ((traverseDirectoryArgs*)args)->existsNewOutDir;
	char * outPath = NULL;;
	outPath = strdup(((traverseDirectoryArgs*)args)->outPath);
	int* totalThreads = ((traverseDirectoryArgs*)args)->totalThreads;
	int colLoc = ((traverseDirectoryArgs*)args)->colLoc;
	
	printf("%u,", (unsigned int) pthread_self());
	
	pthread_mutex_lock(&thread_ID_list);
	if(threadIdsHead == NULL){
		threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
		threadIdsHead->tid = pthread_self();
		threadIdsHead->next = NULL;
		
		threadIdsRear = threadIdsHead;
	}
	else{
		threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
		threadIdsRear = threadIdsRear->next;
		threadIdsRear->tid = pthread_self();
		threadIdsHead->next = NULL;
	}
	pthread_mutex_unlock(&thread_ID_list);
	
	//printf("REAR TID %u\n", (unsigned int) threadIdsRear->tid);
	
	//printf("outPath in traverseDir = %s\n", outPath);
	//flag to see if we printed or not, defaul 0, will be set to current pid after pid printed, must be passed because same process can recursively call traverseDirectory()
	pthread_t tid;
	int err;
	//int pid = -1;
	DIR* directoryPointer;
	struct dirent* currentObject;
	struct stat buffer;
	
	//traverseDirectoryArgs args = malloc(sizeof(traverseDirectoryArgs);
	
	//printf("path to file = %s\n", path);
	
	if((directoryPointer = opendir(path)) == NULL){
		printf("Fatal Error: The directory does not exist.\n");
		//return;
		pthread_exit(0);
	}
	
	//This will change the current working directory (cwd) to the given location, which will be the path we send the function.
	chdir(path);
	
	//We will be reading through everything in the current directory.
	while((currentObject = readdir(directoryPointer)) != NULL){
		
		lstat(currentObject->d_name, &buffer);
		
		//printf("CURRENTOBJECT NAME = %s\n", currentObject->d_name);
		
		if((strcmp(currentObject->d_name, "..") == 0) || (strcmp(currentObject->d_name, ".") == 0)){
					continue;
		}
		//If the current object being read is a directory...
		if(S_ISDIR(buffer.st_mode)){
			//Fork a new process
			fflush(0);
			/*pid = fork();
			if(pid != 0) {
				(*totalThreads)++;
				//printf("\nTraverse: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
			}
			if(pid == 0) {*/
				//printf("TEST TEST TEST TEST: %d\n", getpid());
				//totalProcesses = 0;
				//printf("\nTraverse: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
			
				
				//These are edge names that mess up the traversal (".." make the pointer go up a level)
				if((strcmp(currentObject->d_name, "..") == 0) || (strcmp(currentObject->d_name, ".") == 0)){
					continue;
				}
				
				//We create a char array located in the stack that will hold the path to the new directory we must traverse.
				char* newPath = NULL;
				newPath = getcwd(newPath, 1024);
				
				//This adds the "/" needed before we give another file direction
				strcat(newPath, "/");
				//This adds the new directory name to the new path
				strcat(newPath, currentObject->d_name);
				
				//Repeat process with new directory
				//printf("\nduplicate child prob gets here\n");
				(*totalThreads)++;
				
				
				
				
				//traverseDirectoryArgs args = malloc(sizeof(traverseDirectoryArgs));
				
				//What is i for this thread
				((traverseDirectoryArgs*)args)->path = strdup(newPath);
				//args->argv = argv;
				//args->sortingBy = sortingBy;
				((traverseDirectoryArgs*)args)->existsNewOutDir = existsNewOutDir;
				((traverseDirectoryArgs*)args)->outPath = strdup(outPath);
				((traverseDirectoryArgs*)args)->totalThreads = totalThreads;
				((traverseDirectoryArgs*)args)->colLoc = colLoc;
				
				void * (*traverseFuncPointer)(void*) = traverseDirectory;
				
				err = pthread_create(&tid, NULL, traverseFuncPointer, (void*)args);
				if(err != 0) {
					printf("Fatal error: Thread did not create properly\n");
					//Should it be exit or return?
					pthread_exit(0);
				}
				
				pthread_mutex_lock(&thread_ID_list);
				if(threadIdsHead == NULL){
					threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
					threadIdsHead->tid = tid;
					threadIdsHead->next = NULL;
		
					threadIdsRear = threadIdsHead;
				}
				else{
					threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
					threadIdsRear = threadIdsRear->next;
					threadIdsRear->tid = tid;
					threadIdsHead->next = NULL;
				}
				pthread_mutex_unlock(&thread_ID_list);
				//pthread_join(tid, NULL);
				
				//traverseDirectory(newPath, argv, sortingBy, existsNewOutDir, outPath, totalThreads, colLoc);
				//printf("\nbut does it get here\n");
			
			
			
		}
		//The currentObject is a file so...
		else{
			//Ignore an already sorted file
			if(strstr(currentObject->d_name, "-sorted")){
					continue;
			}
			//Ignore non-csv files
			else if((strstr(currentObject->d_name, ".csv")) == NULL){
					continue;
			}
			//The only remaining option is for the file to be considered for sorting. Thus, fork a new process.
			else{
				//ONLY PRINTS CORRECTLY IF \n IS PRESENT IN BOTH
				fflush(0);
				/*pid = fork();
				if(pid != 0) {
					(*totalThreads)++;
					//printf("\ntraverse-else: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}*/
				/*if(pid == 0) {
					totalProcesses = 0;
					//printf("\ntraverse-else: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}*/
				
				
				(*totalThreads)++;
				pthread_t tid;
				
				sortFileArgs * sortArgs = malloc(sizeof(sortFileArgs));
				
				//printf("currentObject->d_name = %s\n", currentObject->d_name);
				((sortFileArgs*)sortArgs)->fileName = strdup(currentObject->d_name);
				((sortFileArgs*)sortArgs)->argv = argv;
				((sortFileArgs*)sortArgs)->sortingBy = sortingBy;
				((sortFileArgs*)sortArgs)->colLoc = colLoc;
				
				if(existsNewOutDir == 1){
					//sortFile(currentObject->d_name, argv, sortingBy, outPath, colLoc);
					((sortFileArgs*)sortArgs)->path = strdup(outPath);
				}
				else{
					//sortFile(currentObject->d_name, argv, sortingBy, ".", colLoc);
					((sortFileArgs*)sortArgs)->path = getcwd(((sortFileArgs*)sortArgs)->path, 1024);
				}
				
				void * (*sortFuncPointer)(void*) = sortFile;
				
				err = pthread_create(&tid, NULL, sortFuncPointer, (void*)sortArgs);
				if(err != 0) {
					printf("Fatal Error: Something went wrong with thread creation.\n");
					pthread_exit(0);
				}
				
				pthread_mutex_lock(&thread_ID_list);
				if(threadIdsHead == NULL){
					threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
					threadIdsHead->tid = tid;
					threadIdsHead->next = NULL;
		
					threadIdsRear = threadIdsHead;
				}
				else{
					threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
					threadIdsRear = threadIdsRear->next;
					threadIdsRear->tid = tid;
					threadIdsHead->next = NULL;
				}
				pthread_mutex_unlock(&thread_ID_list);
				//pthread_join(tid, NULL);
			}
		}
		//printf("\nare we restarting the loop before printing again? We must be\n");
		
	}
	//Traverse back up to previous file path
	chdir("..");
	closedir(directoryPointer);
	
	//printf("\nI am process: %d. My totalProcesses is : %d\n", getpid(), totalProcesses);
	/*int i = 0;
	int total = totalProcesses;
	for(i = 0; i < total; i++) {
		wait(&status);
		if(WIFEXITED(status)) {
			totalProcesses += WEXITSTATUS(status);
			//printf("\npid: %d, Exit status: %d\n", getpid(), WEXITSTATUS(status));
			//printf("\nI am process: %d. I should be updating totalProcesses, it is now: %d.\n", getpid(),  totalProcesses);
		}
	}*/
	//not sure if we need this addtional wait but just to be safe of any leftover children, let's do it anyways
	//wait(NULL);
	
	//printf("Number of total processes: %d\n", *processCount);
	//return *totalThreads;
	return NULL;
}

//This function prints out the contents of each node in the Linked List.
void printNodes(movie * currPtr, FILE* outputFile)
{
	fprintf(outputFile, "color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes\n");

	int i;
	while(currPtr != NULL){
		
		for(i = 0; i < 28; i++){
			switch(i){
				case 0: 
			
					if(strcmp(currPtr->color, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->color);
					break;
				case 1: 
					if(strcmp(currPtr->director_name, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->director_name); 
					break;
				case 2: 
					if(currPtr->num_critic_for_reviews == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->num_critic_for_reviews); 
					break;
				case 3: 
					if(currPtr->duration == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->duration); 
					break;
				case 4: 
					if(currPtr->director_facebook_likes == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->director_facebook_likes); 
					break;
				case 5: 
					if(currPtr->actor_3_facebook_likes == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->actor_3_facebook_likes); 
					break;
				case 6: 
					if(strcmp(currPtr->actor_2_name, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->actor_2_name); 
					break;
				case 7: 
					if(currPtr->actor_1_facebook_likes == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->actor_1_facebook_likes); 
					break;
				case 8:
					if(currPtr->gross == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->gross); 
					break;
				case 9: 
					if(strcmp(currPtr->genres, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->genres); 
					break;
				case 10: 
					if(strcmp(currPtr->actor_1_name, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->actor_1_name); 
					break;
				case 11:  
					if(strcmp(currPtr->movie_title, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					if(currPtr->quotes == 1){
						fprintf(outputFile,"\"%s\",", currPtr->movie_title);
					} 
					else{
						fprintf(outputFile,"%s,", currPtr->movie_title);
					}
					break;
				case 12: 
					if(currPtr->num_voted_users == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->num_voted_users); 
					break;
				case 13: 
					if(currPtr->cast_total_facebook_likes == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->cast_total_facebook_likes);
					break;
				case 14: 
					if(strcmp(currPtr->actor_3_name, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->actor_3_name);
					break;
				case 15:  
					if(currPtr->facenumber_in_poster == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->facenumber_in_poster); 
					break;
				case 16: 
					if(strcmp(currPtr->plot_keywords, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->plot_keywords); 
					break;
				case 17: 
					if(strcmp(currPtr->movie_imdb_link, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->movie_imdb_link); 
					break;
				case 18: 
					if(currPtr->num_user_for_reviews == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->num_user_for_reviews); 
					break;
				case 19: 
					if(strcmp(currPtr->language, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->language); 
					break;
				case 20: 
					if(strcmp(currPtr->country, "-1") == 0){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->country); 
					break;
				case 21: 
					if(strcmp(currPtr->content_rating, "-1") == 0 || (strcmp(currPtr->content_rating,"-1-1") == 0)){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%s,", currPtr->content_rating); 
					break;
				case 22: 
					if(currPtr->budget == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->budget); 
					break;
				case 23: 
					if(currPtr->title_year == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->title_year); 
					break;
				case 24: 
					if(currPtr->actor_2_facebook_likes == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%ld,", currPtr->actor_2_facebook_likes); 
					break;
				case 25:  
					if(currPtr->imdb_score == -1){
						fprintf(outputFile,",");
						break;
					}
					fprintf(outputFile,"%g,", currPtr->imdb_score); 
					break;
				case 26: 
					if(currPtr->aspect_ratio == -1){
						fprintf(outputFile,",");
						break;
					}
				 	fprintf(outputFile,"%g,", currPtr->aspect_ratio); 
				 	break;
				case 27: 
					if(currPtr->movie_facebook_likes == -1){
						fprintf(outputFile,"\n");
						break;
					}
					fprintf(outputFile,"%ld\n", currPtr->movie_facebook_likes); 
					break;
				default: 
					printf("Fatal Error: The category you would like to sort by is not present.\n");
					pthread_exit(0);
			}
		}
			currPtr = currPtr->next;
	}	
	//fclose(outputFile);
}

//The main function is our driver that will call various functions as necessary. Such functions perform the tasks of creating new Linked List nodes, trimming spaces of strings, determining the category we are sorting by, and printing out the sorted Linked List.
int main(int argc, char ** argv) {
	int * totalThreads = (int *)malloc(sizeof(int));
	*totalThreads = 0;
	int existsNewOutDir = 0;
	int err;
	traverseDirectoryArgs* args = malloc(sizeof(traverseDirectoryArgs));
	//printf("\nargc = %d\n", argc);
	int initPID = getpid();
	printf("Initial PID: %d\n", initPID);
	printf("\tTIDS of all child threads: ");

	//int forkPid;
	//printf("\nDid we pass this check?\n");
	
	/* Start of previous input flags code
	
	//This must ALWAYS be present
	if (strcmp(argv[1], "-c") != 0){
		printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
		exit(0);
	}
	
	int sortingBy = -1;
	sortingBy = sortByCategory(argv[2]);
	
	int forkPid;
	//Default for searchDir should be current directory, default for outputDir should be whatever searchDir is. Think Joe wanted to wait until after input checks to see if he has to do that.
	DIR* searchDir = NULL;
	DIR* outputDir = NULL;
	char* outPath = NULL;
	
	//Checking to see if we are going to be searching in a specific directory instead of current
	if((argc == 5) && (argv[3] != NULL)){
		if((strcmp(argv[3], "-d") != 0) && (strcmp(argv[3], "-o") != 0)){
			printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
			exit(0);
		}
		else if(strcmp(argv[3], "-o") == 0) {
			if(argv[4] == NULL){
				printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> -o <outputdir>\n");
				exit(0);
			}
			else{
				outputDir = opendir(argv[4]);
				if(outputDir == NULL){
					printf("The file directory does not exist\n");
					exit(0);
				}
				existsNewOutDir = 1;
				if(strcmp(argv[4], "./") == 0){
					
					outPath = getcwd(outPath, 1024);
				}
				else if(strstr(argv[4], "../")){
					outPath = getcwd(outPath, 1024);
					outPath = strcat(outPath, "/");
					outPath = strcat(outPath, argv[4]);
					//printf("outPath = %s\n", outPath);
				}
				else{
					outPath = strdup(argv[4]);
				}
				//printf("outPath in main = %s\n", outPath);
			}
		}
		//The 4 output will be -d
		else{
			if(argv[4] == NULL){
				printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
				exit(0);
			}
			else{
				searchDir = opendir(argv[4]);
				if(searchDir == NULL){
					printf("The file directory does not exist\n");
					exit(0);
				}
				fflush(0);
				forkPid = fork();
				if(forkPid != 0) {
					totalProcesses++;
					//printf("\nmain: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}
				if(forkPid == 0) {
					totalProcesses = 0;
					//printf("\nmain: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}
				if(forkPid == 0){
					
					traverseDirectory(argv[4], argv, sortingBy, totalProcesses, 0, existsNewOutDir, ".");
				}
			}
		}
	}
	//Checking to see if we are going to be searching and outputting to a specific directory instead of current
	if((argc == 7) && (argv[5] != NULL)){
		if(strcmp(argv[5], "-o") != 0){
			printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
			exit(0);
		}
		else if(strcmp(argv[3], "-d") != 0){
			printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
			exit(0);
		}
		else{
			if(argv[6] == NULL){
				printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
				exit(0);
			}
			else{
				outputDir = opendir(argv[6]);
				if(outputDir == NULL){
					printf("The file directory does not exist\n");
					exit(0);
				}
				existsNewOutDir = 1;
				
				if(strcmp(argv[6], "./") == 0){
					
					outPath = getcwd(outPath, 1024);
				}
				else if(strstr(argv[6], "../")){
					outPath = getcwd(outPath, 1024);
					outPath = strcat(outPath, "/");
					outPath = strcat(outPath, argv[6]);
					//printf("outPath = %s\n", outPath);
				}
				else{
					outPath = strdup(argv[6]);
				}
				
				//printf("outPath in main = %s\n", outPath);
			}
		
		//The 4 output will be -d
		
			if(argv[4] == NULL){
				printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
				exit(0);
			}
			else{
				searchDir = opendir(argv[4]);
				if(searchDir == NULL){
					printf("The file directory does not exist\n");
					exit(0);
				}
				fflush(0);
				forkPid = fork();
				if(forkPid != 0) {
					totalProcesses++;
					//printf("\nmain: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}
				if(forkPid == 0) {
					totalProcesses = 0;
					//printf("\nmain: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
				}
				if(forkPid == 0){
					
					traverseDirectory(argv[4], argv, sortingBy, totalProcesses, 0, existsNewOutDir, outPath);
				}
			}
		}
	}
	End of previous input flags code*/
	
	
	//The input format will be incorrect if argc is 1, or an even number
	if((argc == 1) || ((argc % 2) == 0)){
		printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
		exit(0);
	}
	int hasCflag = 0;
	int dFlag = 0;
	int oFlag = 0;
	
	int sortingBy = -1;
	
	//Default for searchDir should be current directory, default for outputDir should be whatever searchDir is. Think Joe wanted to wait until after input checks to see if he has to do that.
	DIR* searchDir = NULL;
	DIR* outputDir = NULL;
	char* outPath = NULL;
	
	//storing where the name of the column heading is
	int colLoc = -1;
	
	//This starts on the first argument after ./sorter
	int i = 1;
	for(i = 1; i < argc; i++) {
		switch(i) {
			case 1:
				if(strcmp(argv[i], "-c") == 0) {
					hasCflag++;
					sortingBy = sortByCategory(argv[i+1]);
					colLoc = i+1;
				}
				else if(strcmp(argv[i], "-o") == 0) {
					oFlag++;
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> -o <outputdir>\n");
						exit(0);
					}
					else{
						outputDir = opendir(argv[i+1]);
						if(outputDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						existsNewOutDir = 1;
						if(strcmp(argv[i+1], "./") == 0){
							outPath = getcwd(outPath, 1024);
						}
						else if(strstr(argv[i+1], "../")){
							outPath = getcwd(outPath, 1024);
							outPath = strcat(outPath, "/");
							outPath = strcat(outPath, argv[i+1]);
							//printf("outPath = %s\n", outPath);
						}
						else{
							outPath = strdup(argv[i+1]);
						}
						//printf("outPath in main = %s\n", outPath);
					}
				}
				//The 4 output will be -d
				else if(strcmp(argv[i], "-d") == 0) {
					dFlag++;
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
					}
					else{
						searchDir = opendir(argv[i+1]);
						if(searchDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						fflush(0);
						
						
						/*forkPid = fork();
						if(forkPid != 0) {
							(*totalThreads)++;
							//printf("\nmain: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0) {
							totalProcesses = 0;
							//printf("\nmain: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0){
					
							traverseDirectory(argv[i+1], argv, sortingBy, existsNewOutDir, ".", totalThreads, colLoc);
						}*/
						
						(*totalThreads)++;
						traverseDirectoryArgs* args = malloc(sizeof(traverseDirectoryArgs));
						((traverseDirectoryArgs*)args)->path = strdup(argv[i+1]);
						((traverseDirectoryArgs*)args)->argv = argv;
						((traverseDirectoryArgs*)args)->sortingBy = sortingBy;
						((traverseDirectoryArgs*)args)->existsNewOutDir = existsNewOutDir;
						((traverseDirectoryArgs*)args)->outPath = strdup(outPath);
						((traverseDirectoryArgs*)args)->totalThreads = totalThreads;
						((traverseDirectoryArgs*)args)->colLoc = colLoc;
						
						pthread_t tid;
						
						void * (*traverseFuncPointer)(void*) = traverseDirectory;
						
						err = pthread_create(&tid, NULL, traverseFuncPointer, (void*)args);
						if(err != 0) {
							printf("Fatal error: Thread did not create properly\n");
							pthread_exit(0);
						}
						pthread_mutex_lock(&thread_ID_list);
						if(threadIdsHead == NULL){
							threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
							threadIdsHead->tid = tid;
							threadIdsHead->next = NULL;
		
							threadIdsRear = threadIdsHead;
						}
						else{
							threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
							threadIdsRear = threadIdsRear->next;
							threadIdsRear->tid = tid;
							threadIdsHead->next = NULL;
						}
						pthread_mutex_unlock(&thread_ID_list);
						//pthread_join(tid, NULL);
					}
				}
				else {
					printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
				}
			break;
			case 2:
				if(strcmp(argv[i], "-c") == 0) {
					printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
					exit(0);
				}
			break;
			case 3:
				if(strcmp(argv[i], "-c") == 0) {
					if(hasCflag != 0) {
						printf("Fatal Error: repeated flag. Please only use -c flag once.\n");
						exit(0);
					}
					hasCflag++;
					sortingBy = sortByCategory(argv[i+1]);
					colLoc = i+1;
				}
				else if(strcmp(argv[i], "-o") == 0) {
					if(oFlag != 0) {
						printf("Fatal Error: repeated flag. Please only use -o flag once.\n");
						exit(0);
					}
					oFlag++;
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> -o <outputdir>\n");
						exit(0);
					}
					else{
						outputDir = opendir(argv[i+1]);
						if(outputDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						existsNewOutDir = 1;
						if(strcmp(argv[i+1], "./") == 0){
							outPath = getcwd(outPath, 1024);
						}
						else if(strstr(argv[i+1], "../")){
							outPath = getcwd(outPath, 1024);
							outPath = strcat(outPath, "/");
							outPath = strcat(outPath, argv[i+1]);
							//printf("outPath = %s\n", outPath);
						}
						else{
							outPath = strdup(argv[i+1]);
						}
						//printf("outPath in main = %s\n", outPath);
					}
				}
				//The 4 output will be -d
				else if(strcmp(argv[i], "-d") == 0) {
					if(dFlag != 0) {
						printf("Fatal Error: repeated flag. Please only use -d flag once.\n");
						exit(0);
					}
					dFlag++;
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
					}
					else{
						searchDir = opendir(argv[i+1]);
						if(searchDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						fflush(0);
						/*forkPid = fork();
						if(forkPid != 0) {
							(*totalThreads)++;
							//printf("\nmain: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0) {
							totalProcesses = 0;
							//printf("\nmain: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0){
					
							traverseDirectory(argv[i+1], argv, sortingBy, existsNewOutDir, ".", totalThreads, colLoc);
						}*/
						(*totalThreads)++;
						//traverseDirectoryArgs args = malloc(sizeof(traverseDirectoryArgs));
						((traverseDirectoryArgs*) args)->path = strdup(argv[i+1]);
						((traverseDirectoryArgs*) args)->argv = argv;
						((traverseDirectoryArgs*) args)->sortingBy = sortingBy;
						((traverseDirectoryArgs*) args)->existsNewOutDir = existsNewOutDir;
						((traverseDirectoryArgs*) args)->outPath = getcwd(((traverseDirectoryArgs*) args)->outPath, 1024);
						((traverseDirectoryArgs*) args)->totalThreads = totalThreads;
						((traverseDirectoryArgs*) args)->colLoc = colLoc;
						
						pthread_t tid;
						void * (*traverseFuncPointer)(void*) = traverseDirectory;
						
						err = pthread_create(&tid, NULL, traverseFuncPointer, (void*)args);
						if(err != 0) {
							printf("Fatal error: Thread did not create properly\n");
							exit(0);
						}
						pthread_mutex_lock(&thread_ID_list);
						if(threadIdsHead == NULL){
							threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
							threadIdsHead->tid = tid;
							threadIdsHead->next = NULL;
		
							threadIdsRear = threadIdsHead;
						}
						else{
							threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
							threadIdsRear = threadIdsRear->next;
							threadIdsRear->tid = tid;
							threadIdsHead->next = NULL;
						}
						pthread_mutex_unlock(&thread_ID_list);
						//pthread_join(tid, NULL);
					}
				}
				else {
					printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
				}
			break;
			case 4:
				if(strcmp(argv[i], "-c") == 0) {
					printf("Fatal Error: The format of the input is incorrect. Please use a combination of  the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
					exit(0);
				}
			break;
			case 5:
				if(strcmp(argv[i], "-c") == 0) {
					if(hasCflag != 0) {
						printf("Fatal Error: repeated flag. Please only use -c flag once.\n");
						exit(0);
					}
					hasCflag++;
					sortingBy = sortByCategory(argv[i+1]);
					colLoc = i+1;
				}
				else if(strcmp(argv[i], "-o") == 0) {
					if(oFlag != 0) {
						printf("Fatal Error: repeated flag. Please only use -o flag once.\n");
						exit(0);
					}
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> -o <outputdir>\n");
						exit(0);
					}
					else{
						outputDir = opendir(argv[i+1]);
						if(outputDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						existsNewOutDir = 1;
						if(strcmp(argv[i+1], "./") == 0){
							outPath = getcwd(outPath, 1024);
						}
						else if(strstr(argv[i+1], "../")){
							outPath = getcwd(outPath, 1024);
							outPath = strcat(outPath, "/");
							outPath = strcat(outPath, argv[i+1]);
							//printf("outPath = %s\n", outPath);
						}
						else{
							outPath = strdup(argv[i+1]);
						}
						//printf("outPath in main = %s\n", outPath);
					}
				}
				//The 4 output will be -d
				else if(strcmp(argv[i], "-d") == 0) {
					if(dFlag != 0) {
						printf("Fatal Error: repeated flag. Please only use -d flag once.\n");
						exit(0);
					}
					if(argv[i+1] == NULL){
						printf("Fatal Error: The format of the input is incorrect. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
					}
					else{
						searchDir = opendir(argv[i+1]);
						if(searchDir == NULL){
							printf("The file directory does not exist\n");
							exit(0);
						}
						fflush(0);
						/*forkPid = fork();
						if(forkPid != 0) {
							(*totalThreads)++;
							//printf("\nmain: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0) {
							totalProcesses = 0;
							//printf("\nmain: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
						}*/
						/*if(forkPid == 0){
					
							traverseDirectory(argv[i+1], argv, sortingBy, existsNewOutDir, ".", totalThreads, colLoc);
						}*/
						(*totalThreads)++;
						traverseDirectoryArgs * args = malloc(sizeof(traverseDirectoryArgs));
						((traverseDirectoryArgs*)args)->path = strdup(argv[i+1]);
						((traverseDirectoryArgs*)args)->argv = argv;
						((traverseDirectoryArgs*)args)->sortingBy = sortingBy;
						((traverseDirectoryArgs*)args)->existsNewOutDir = existsNewOutDir;
						((traverseDirectoryArgs*)args)->outPath = getcwd(((traverseDirectoryArgs*)args)->outPath, 1024);
						((traverseDirectoryArgs*)args)->totalThreads = totalThreads;
						((traverseDirectoryArgs*)args)->colLoc = colLoc;
						
						pthread_t tid;
						void * (*traverseFuncPointer)(void*) = traverseDirectory;
						
						err = pthread_create(&tid, NULL, traverseFuncPointer, (void*)args);
						if(err != 0) {
							printf("Fatal error: Thread did not create properly\n");
							exit(0);
						}
						pthread_mutex_lock(&thread_ID_list);
						if(threadIdsHead == NULL){
							threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
							threadIdsHead->tid = tid;
							threadIdsHead->next = NULL;
		
							threadIdsRear = threadIdsHead;
						}
						else{
							threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
							threadIdsRear = threadIdsRear->next;
							threadIdsRear->tid = tid;
							threadIdsHead->next = NULL;
						}
						pthread_mutex_unlock(&thread_ID_list);
						//pthread_join(tid, NULL);
					}
				}
				else {
					printf("Fatal Error: The format of the input is incorrect. Please use the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
						exit(0);
				}
			break;
			case 6:
				if(strcmp(argv[i], "-c") == 0) {
					printf("Fatal Error: The format of the input is incorrect. Please use a combination of  the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
					exit(0);
				}
			break;
			default:
				printf("Fatal Error: This should never happen. Something wrong with for loop.\n");
				exit(0);
		}
	}
	
	
	if(hasCflag == 0) {
		printf("Fatal Error: The format of the input is incorrect. Missing -c flag. Please use a combination of the format: ./sorter.c -c <column heading> <-d thisdir> <-o thatdir>\n");
		exit(0);
	}

	char* cwd = NULL;
	
	//If we were not given a specific directory to traverse, we will utilize the current directory
	if(searchDir == NULL){
		cwd = getcwd(cwd, 1024);
		if(cwd == NULL) {
			printf("Fatal error: Something wrong with default directory. Please fix me.\n");
			exit(0);
		}
		//printf("CWD = %s\n", cwd);
		searchDir = opendir(cwd);
		if(searchDir == NULL){
			printf("Fatal error: Something wrong with default directory. Please fix me.\n");
			exit(0);
		}
		
		fflush(0);
		/*forkPid = fork();
		if(forkPid != 0) {
			(*totalThreads)++;
			//printf("\nmain-2: I JUST FORKED-parent, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
		}*/
		/*if(forkPid == 0) {
			totalProcesses = 0;
			//printf("\nmain-2: I JUST FORKED-child, my pid is : %d, totalProcesses is now: %d\n", getpid(), totalProcesses);
		}*/
		
		/*if(forkPid == 0){
			if(outPath == NULL){
				outPath = ".";
			}
			
			//printf("outPath in main = %s\n", outPath);
			traverseDirectory(cwd, argv, sortingBy, existsNewOutDir, outPath, totalThreads, colLoc);
		}*/
		(*totalThreads)++;
		
		
		((traverseDirectoryArgs*)args)->path = strdup(cwd);
		((traverseDirectoryArgs*)args)->argv = argv;
		((traverseDirectoryArgs*)args)->sortingBy = sortingBy;
		((traverseDirectoryArgs*)args)->existsNewOutDir = existsNewOutDir;
		((traverseDirectoryArgs*)args)->outPath = getcwd(((traverseDirectoryArgs*)args)->outPath, 1024);
		((traverseDirectoryArgs*)args)->totalThreads = totalThreads;
		((traverseDirectoryArgs*)args)->colLoc = colLoc;
		
		pthread_t tid;
		void * (*traverseFuncPointer)(void*) = traverseDirectory;
						
		err = pthread_create(&tid, NULL, traverseFuncPointer, (void*)args);
		if(err != 0) {
			printf("Fatal error: Thread did not create properly\n");
			exit(0);
		}
		pthread_mutex_lock(&thread_ID_list);
		if(threadIdsHead == NULL){
			threadIdsHead = (threadIds*)malloc(sizeof(threadIds));
			threadIdsHead->tid = tid;
			threadIdsHead->next = NULL;
		
			threadIdsRear = threadIdsHead;
		}
		else{
			threadIdsRear->next = (threadIds*)malloc(sizeof(threadIds));
			threadIdsRear = threadIdsRear->next;
			threadIdsRear->tid = tid;
			threadIdsHead->next = NULL;
		}
		pthread_mutex_unlock(&thread_ID_list);
		
		//pthread_join(tid, NULL);
	}
	/*
	if(forkPid != 0){
		wait(NULL);
	}
	*/
	/*i = 0;
	int total = totalProcesses;
	for(i = 0; i < total; i++) {
		wait(&status);
		if(WIFEXITED(status)) {
			totalProcesses += WEXITSTATUS(status);
			//printf("\npid: %d, Exit status: %d\n", getpid(), WEXITSTATUS(status));
			//printf("\nI am process: %d. I should be updating totalProcesses, it is now: %d.\n", getpid(),  totalProcesses);
		}*/
	//wait(NULL);
	/*if(cwd != NULL){
		free(cwd);
	}*/
	fflush(0);
	
	
	//free(totalThreads);
	//Jake: it seems that the first child is returning correctly and getting the proper status, let's do some more testing for children of children
	
	//Joe: Also, implement -o operation. Also, the fatal output error for if the file is already sorter is not necessary and will mess up the output.
	
	//NOTE: fflush(0) is necessary before forking in order to clear the I/O buffer to repeat incredibly repetitive output.
	//printf("END OF MAIN THREADIDSHEAD TID = %u\n", (unsigned int) threadIdsHead->tid);
	
	
	sleep(1);
	//printf("threadIdsHead->tid = %u\n", (unsigned int) threadIdsHead->tid);
	do{
		pthread_join(threadIdsHead->tid, NULL);
		threadIdsHead = threadIdsHead->next;
	}while(threadIdsHead != NULL);
	
	printf("\n\tTotal number of threads: %d\n", *(((traverseDirectoryArgs*)args)->totalThreads));
	return 0;
}
