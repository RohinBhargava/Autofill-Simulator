#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include <string.h>
#include "hash.h"
#include <ctype.h>
#define DICT_SIZE (long) -1 * (1 << 31)
#define DICT_MASK (long) (DICT_SIZE - 1)

word_tree** sort(word_tree **left_list, word_tree **right_list, int left_count, int right_count)
{
	word_tree **tree_list = calloc(left_count+ right_count+ 1, sizeof(word_tree *));
	int i = 0;
	int j = 0;
	while (i < left_count && j < right_count)
	{
		if (left_list[i]->name->score > right_list[j]->name->score)
		{
			tree_list[i + j] = left_list[i];
			++i;
		}
		else
		{
			tree_list[i + j] = right_list[j];
			++j;
		}
	}	
	
	while (i < left_count)
	{
		tree_list[i + j] = left_list[i];
		++i;
	}
	
	while (j < right_count)
	{
		tree_list[i + j] = right_list[j];
		++j;
	}
	
	return tree_list;
}

word_tree** merge_sort(word_tree **tree_list, int tree_count)
{
	if (tree_count == 1)
		return tree_list;
	int left = tree_count/2;
	int right = tree_count - left;
	word_tree **left_list = calloc(left, sizeof(word_tree *));
	word_tree **right_list =  calloc(right, sizeof(word_tree *));
	memcpy(left_list, tree_list, left * sizeof(word_tree *));
	memcpy(right_list, &(tree_list[left]), right * sizeof(word_tree *));
	return sort(merge_sort(left_list, left), merge_sort(right_list, right), left, right);
}

word *init_word(char *name)
{
	word *ptr = malloc(sizeof(word));
	ptr->name = name;
	ptr->score = 0;
	return ptr;
}

word_tree *init_word_tree(word* w)
{
	word_tree *wt = malloc(sizeof(word_tree));
	wt->name = w;
	wt->left = NULL;
	wt->children = calloc(10, sizeof(word_tree));
	wt->tree_count = 0;
	return wt;
}

void free_word_tree(word_tree* wt)
{
	int i;
	if(wt->tree_count > 0)
	{
		for (i = 0; i < wt->tree_count; ++i)
			free_word_tree(wt->children[i]);
		free(wt->children);
	}

	if (wt->left != NULL)
	{
		free_word_tree(wt->left);	
	}

	free(wt);
}

word_hash* init_word_hash()
{
	word_hash *wh = malloc(sizeof(word_hash));
	wh->hash = calloc(DICT_SIZE, sizeof(word_tree));
	return wh;
}

void free_word_hash(word_hash *wh)
{
	long i;
	for (i = 0; i < DICT_SIZE; ++i)
		if (wh->hash[i] != NULL)
			free_word_tree(wh->hash[i]);
}

void insert_first_word(word_hash *wh, char *name)
{
	word_tree *new_word_tree = init_word_tree(init_word(name));
	long look = (hash(name) & BIT_MASK);
	if (wh->hash[look] == NULL)
			wh->hash[look] = new_word_tree;
	else if (strcmp(wh->hash[look]->name->name, name) == 0)
		++wh->hash[look]->name->score;
	else if (strcmp(wh->hash[look]->name->name, name) != 0)
		wh->hash[look]->left = new_word_tree;
}

word_tree *retreive_first_word(word_hash *wh, char *name)
{
	word_tree *x = wh->hash[hash(name) & BIT_MASK];
	if (x != NULL)
		while (strcmp(x->name->name, name) != 0)
			x = x->left;
	return x;
}

word_tree *child_word(word_tree* wt, char *name, int n)
{
	int i;
	for (i = 0; i < wt->tree_count; ++i)
	{
		if (strncmp(name, wt->children[i]->name->name, n) == 0)
			return wt->children[i];
	}
	return NULL;
}

word_tree *insert_word(word_tree *wt, char *name)
{
	word_tree *current = child_word(wt, name, strlen(name));

	if (current == NULL)
	{
		current = init_word_tree(init_word(name));
		wt->children[wt->tree_count] = current;
		++wt->tree_count;
	}

	else
	{
		++current->name->score;
		wt->children = merge_sort(wt->children, wt->tree_count);
	}

	return current;
}


bool isacceptedchar(char c)
{
	if (c != '.' && c != '!' && c != '?' && c != '\n' && c != '\t' && c != EOF)
		return true;
	return false;
}

char *get_sentence(FILE * inf)
{
    char *buf = (char *) malloc(100 * sizeof(char));
    int buf_size = 100;
            
    char ch;
        
    int index = 0;
    while (!isacceptedchar(ch = (char) fgetc(inf)) || ch == ' ' || ch == '-' || ch == '\"' || ch == '\'' || isdigit(ch))    
    {
        if (ch == EOF)
        	return NULL;
    }
    
  	do
  	{
  		if (ch == '\"' || ch == '\'')
  			continue;
        buf[index++] = ch;
        if (index == buf_size) 
        {
            buf_size *= 2;
        	buf = (char *) realloc(buf,buf_size * sizeof(char));
        }
    } while (isacceptedchar(ch = (char) fgetc(inf)) && ch != EOF);
    
    buf[index] = '\0';
      
    return buf;
}

void parse_file(FILE * inf, word_hash *wh)
{  
	char *sentence;

    while((sentence = get_sentence(inf)) != NULL)
    {
    	char *t = strtok(sentence, " ");
    	insert_first_word(wh, t);
    	word_tree *current = retreive_first_word(wh, t);
    	char *tok = strtok(NULL, " ");
    	while (tok != NULL)
    	{ 
    		char *cpy = strdup(tok);
			current = insert_word(current, cpy);
			tok = strtok(NULL, " ");
		}
    }
}

word_tree **lister(word_tree* wt, char *name, int n)
{
	int list_size = 10;
	word_tree **list = calloc(list_size, sizeof(word_tree *));
	int i;
	int j = 0;
	for (i = 0; i < wt->tree_count; ++i)
	{
		if (strncmp(name, wt->children[i]->name->name, n) == 0)
		{
			list[j] = wt->children[i];
			++j;
			if (j == list_size)
			{
				list_size *= 2;
				list = (word_tree **) realloc(list, list_size * sizeof(word_tree *));
			}
		}
	}
	return list;
}

void tree_recurse(word_tree *wt, char *cur)
{
	char *new_cur = strdup(cur);
	strcat(new_cur, " ");
	strcat(new_cur, wt->name->name);
	if (wt->tree_count == 0)
	{
		printf("- %s.\n", new_cur);
		return;
	}	
	int i;
	for (i = 0; i < wt->tree_count; ++i)
		tree_recurse(wt->children[i], new_cur);
}

void top_10_results(char *x, word_hash *wh)
{
	char *find = strtok(x, " ");
	char *look_ahead;
	char *accumulate = strdup(x);
	word_tree *found = retreive_first_word(wh, find);

	if (found == NULL)
	{
		printf("Please check your phrase.\n");
		return;
	}

	find = strtok(NULL, " ");
	word_tree **list = NULL;

	while(find != NULL)
	{
		look_ahead = strtok(NULL, " ");

		if (look_ahead != NULL)
		{
			found = child_word(found, find, strlen(find));
			if (found == NULL || strcmp(found->name->name, find) != 0)
			{
				printf("Please check your phrase.\n");
				return;
			}
			strcat(accumulate," ");
			strcat(accumulate, found->name->name);
		}

		else
			list = lister(found, find, strlen(find));

		find = look_ahead;
	}

	int i;
	int j = 0;

	if (list == NULL)
	{
		for (i = 0; i < found->tree_count; ++i)
			tree_recurse(found->children[i], accumulate);
	}

	else
	{
		while(list[j] != NULL)
		{	
			found = list[j];
			char *temp = strdup(accumulate);
			strcat(temp," ");
			strcat(temp, found->name->name);
			for (i = 0; i < found->tree_count; ++i)
				tree_recurse(found->children[i], temp);
			++j;
			free(temp);
		}
	}

	free(find);
	free(accumulate);
}

int main(int argc, char **argv)
{
	FILE * inf;
	int i;
	word_hash *wh = init_word_hash();
	for (i = 1; i < argc; ++i) 
	{
		inf = fopen(argv[i], "r");
		parse_file(inf, wh);
	}
	char * string = malloc(sizeof(char));
	while(1)
	{
		printf("Please enter part of a string you would like to search (enter \"0\" to exit): ");
		fgets(string, 1024, stdin);
		string[strlen(string) - 1] = '\0';
		if (strcmp(string, "0") == 0)
			break;
		top_10_results(string, wh);
	}
	printf("Exiting. Freeing up memory. Please wait.\n");
	free(string);
	// Comment for less security, but faster performance:
	free_word_hash(wh);
	fclose(inf);
}
