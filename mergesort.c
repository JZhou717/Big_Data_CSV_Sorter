#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorter.h"


/*The main method for the merge function. This goes through the 4 steps of mergesort and uses helper functions to complete its purpose*/
//We pass a pointer to the poitner of the linked list, which we can de-reference to change the actual list, and an int representing the category that we are sorting by
void merge(movie ** frontAddress, int sortingBy)
{
	movie * front = *frontAddress;
	movie * first = NULL;
	movie * second = NULL;

	//List does not need to be sorted if 1 or 0 element(s)
	if((front == NULL) || (front->next == NULL))
	{
		return;
	}

	//Need a method that splits the list of data points into halves
	split(front, &first, &second);

	//Recursively sort each half-list
	merge(&first, sortingBy);
	merge(&second, sortingBy);

	//Lastly merge them together
	*frontAddress = sorted(first, second, sortingBy);

	return;
}

movie * sorted(movie * first, movie * second, int sortingBy)
{
	//The pointer to the front of our sorted list once we are done merging the two given list-halves	
	movie * result = NULL;
	
	/*Checking to see if one of the lists is empty*/
	if(first == NULL)
	{
		return second;
	}
	else if(second == NULL)
	{
		return first;
	}
	
	//If isNum is 0, then we are sorting by string. If isNum is 1, we are sorting by numbers
	int isNum = 0;
	
	//We are wasting a tiny amount of space to create a variable for the case of longs/doubles or strings
	char * fStrVal = "";
	char * sStrVal = "";
	double fNumVal = 0;
	double sNumVal = 0;
	
	switch(sortingBy)
	{
		case 0: fStrVal = first->color; sStrVal = second->color; break;
		case 1: fStrVal = first->director_name; sStrVal = second->director_name; break;
		case 2: isNum = 1; fNumVal = first->num_critic_for_reviews; sNumVal = second->num_critic_for_reviews; break;
		case 3: isNum = 1; fNumVal = first->duration; sNumVal = second->duration; break;
		case 4: isNum = 1; fNumVal = first->director_facebook_likes; sNumVal = second->director_facebook_likes; break;
		case 5: isNum = 1; fNumVal = first->actor_3_facebook_likes; sNumVal = second->actor_3_facebook_likes; break;
		case 6: fStrVal = first->actor_2_name; sStrVal = second->actor_2_name; break;
		case 7: isNum = 1; fNumVal = first->actor_1_facebook_likes; sNumVal = second->actor_1_facebook_likes; break;
		case 8: isNum = 1; fNumVal = first->gross; sNumVal = second->gross; break;
		case 9: fStrVal = first->genres; sStrVal = second->genres; break;
		case 10: fStrVal = first->actor_1_name; sStrVal = second->actor_1_name; break;
		case 11: fStrVal = first->movie_title; sStrVal = second->movie_title; break;
		case 12: isNum = 1; fNumVal = first->num_voted_users; sNumVal = second->num_voted_users; break;
		case 13: isNum = 1; fNumVal = first->cast_total_facebook_likes; sNumVal = second->cast_total_facebook_likes; break;
		case 14: fStrVal = first->actor_3_name; sStrVal = second->actor_3_name; break;
		case 15: isNum = 1; fNumVal = first->facenumber_in_poster; sNumVal = second->facenumber_in_poster; break;
		case 16: fStrVal = first->plot_keywords; sStrVal = second->plot_keywords; break;
		case 17: fStrVal = first->movie_imdb_link; sStrVal = second->movie_imdb_link; break;
		case 18: isNum = 1; fNumVal = first->num_user_for_reviews; sNumVal = second->num_user_for_reviews; break;
		case 19: fStrVal= first->language; sStrVal = second->language; break;
		case 20: fStrVal = first->country; sStrVal = second->country; break;
		case 21: fStrVal = first->content_rating; sStrVal = second->content_rating; break;
		case 22: isNum = 1; fNumVal = first->budget; sNumVal = second->budget; break;
		case 23: isNum = 1; fNumVal = first->title_year; sNumVal = second->title_year; break;
		case 24: isNum = 1; fNumVal = first->actor_2_facebook_likes; sNumVal = second->actor_2_facebook_likes; break;
		case 25: isNum = 1; fNumVal = first->imdb_score; sNumVal = second->imdb_score; break;
		case 26: isNum = 1; fNumVal = first->aspect_ratio; sNumVal = second->aspect_ratio; break;
		case 27: isNum = 1; fNumVal = first->movie_facebook_likes; sNumVal = second->movie_facebook_likes; break;
		default: printf("Fatal Error: Something went wrong with selecting the category to sort by.\n");
	}
	
	//printf("%s and %s\n", fStrVal, sStrVal);
	
	/*We're going to take the lexicographically first value and store it as the current node. We are going to call this method recursively on both lists minus what we just added so we can keep going in order until one list is null*/
	
	//STILL HAVE TO ACCOUNT FOR INSTANCES OF NULL
	if(isNum == 0)
	{
		if (strcmp(fStrVal, "-1") == 0) //The first value is a NULL value
		{
			result = first;
			result->next = sorted(first->next, second, sortingBy);
		}
		else if (strcmp(sStrVal, "-1") == 0) //The second value is a NUll value
		{
			result = second;
			result->next = sorted(first, second->next, sortingBy);
		}
		else //Neither of the values are NULL
		{
			if(strcmp(fStrVal, sStrVal) <= 0)
			{
				result = first;
				result->next = sorted(first->next, second, sortingBy);
			}
			else
			{
				result = second;
				result->next = sorted(first, second->next, sortingBy);
			}
		}
	}
	else if(isNum == 1)
	{
		if (fNumVal == -1) //The first value is NULL
		{
			result = first;
			result->next = sorted(first->next, second, sortingBy);
		}
		else if (sNumVal == -1) //The second value is NULL
		{
			result = second;
			result->next = sorted(first, second->next, sortingBy);
		}
		else //Neither value is NULL
		{
			if(fNumVal <= sNumVal)
			{
				result = first;
				result->next = sorted(first->next, second, sortingBy);
			}
			else
			{
				result = second;
				result->next = sorted(first, second->next, sortingBy);
			}
		}
	}
	//Returning the merged and sorted result
	return result;
}

/*This function takes a linked list and returns pointers to the two halves of the source list. If there is an odd amount of items in the list, the extra node goes into the front half*/

//WE THINIK THIS SHOULD SAVE ANY UPDATES TO FIRST AND SECOND TO THE ORIGINAL ADDRESS BUT IF THIS DOESN'T WORK PROPERLY, WE KNOW WHERE TO LOOK
void split(movie * front, movie ** first, movie ** second)
{
	//Using a fast and slow iterating pair of pointers to find the midpoint of the linked list
	movie * fast;
	movie * slow;
	//If there are only one or no items in the list so far to sort
	if(front == NULL || front->next == NULL)
	{
		*first = front;
		*second = NULL;
	}
	else //If there are more than just one or no items in the list
	{
		slow = front;
		fast = front->next;
		//Iterating the fast pointer by two nodes each cycle while only iterating slow by one node
		//We keep iterating until fast reaches the end, at which point, slow should be at the halfway point
		while(fast != NULL)
		{
			fast = fast->next;
			if(fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		*first = front;
		*second = slow->next;
		slow->next = NULL;
	}
	return;
}
