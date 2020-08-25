
/*Spelling checker*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <conio.h>
#include <ctype.h>

#define mlength 20
#define CHAR_SIZE 28
#define words_size 500000
#define large 400000

long long int dict_len;
long long int flen;

char dict[large][89];

struct Trie
{
	int isLeaf;
	struct Trie* character[CHAR_SIZE];
};
/*
"Trie" for storing words from dictionary.
The words from the dictionary are loaded to the trie for once.
*/
int k;
int finalcount;

struct trie2
{
	int isLeaf;
	struct trie2* character[CHAR_SIZE];
};
/*
trie2 for storing words from user file so that it is ensured a word is not repeatedly checked
from the user file.
*/

char words_user_file[words_size][100]={""};
char temp_lowercase[words_size][100];
struct trie2* convert_to_lower();
struct Trie* form(FILE*);
struct Trie* ignore_user_words(struct Trie*);
void validate_userfile(struct Trie*,FILE*);
struct Trie* getNewTrieNode();
void insert(struct Trie *, char*);
int search(struct Trie* , char*);
int haveChildren(struct Trie*);
struct trie2* getNewTrie2Node();
void insert2(struct trie2 *, char*);
int search2(struct trie2*, char*);
int haveChildren2(struct trie2*);
int min(int, int , int);
int number_of_operations(char [], char [], int , int );
void correct(char [],FILE *,bool);

struct Trie* getNewTrieNode()
{
	struct Trie* node = (struct Trie*)malloc(sizeof(struct Trie));
	node->isLeaf = 0;

	for (int i = 0; i < CHAR_SIZE; i++)
		{
		    node->character[i] = NULL;
		}
	return node;
}

void insert(struct Trie *head, char* str)
{
	struct Trie* curr = head;
	while (*str)
	{
            if(*str==39)
        {
            if (curr->character[26] == NULL)
            {

          curr->character[26] = getNewTrie2Node();
            }
            curr = curr->character[26];
        }
        else if(*str=='-')
        {

            if (curr->character[27] == NULL)
            {

          curr->character[27] = getNewTrie2Node();
            }
            curr = curr->character[27];
        }
        else
            {
		if (curr->character[*str - 'a'] == NULL)
        {
			curr->character[*str - 'a'] = getNewTrie2Node();
        }
        curr = curr->character[*str - 'a'];

        }


		str++;
	}
	curr->isLeaf = 1;
}
/*
the characters that are to be inserted in the trie are lowercase alphabet and "'"
the memory is allocated for 28 blocks 26 blocks for lowercase alphabet
and other two are for "'" and "-" characters
ch==39 specfies ascii value of "'" character.
*/
int search(struct Trie* head, char* str)
{
	if (head == NULL)
		{
		    return 0;
		}

	struct Trie* curr = head;
	while (*str)
	{

        if(*str==39)
		{
		    curr = curr->character[26];
		}
		else if(*str=='-')
        {
            curr = curr->character[27];
        }
		else
            {
                curr = curr->character[*str - 'a'];
            }
		if (curr == NULL)
			{
			    return 0;
			}
		str++;
	}
	return curr->isLeaf;
}

int haveChildren(struct Trie* curr)
{
	for (int i = 0; i < CHAR_SIZE; i++)

		{
		    if (curr->character[i])
			return 1;
		}
	return 0;
}

struct trie2* getNewTrie2Node()
{
	struct trie2* node = (struct trie2*)malloc(sizeof(struct trie2));
	node->isLeaf = 0;

	for (int i = 0; i < CHAR_SIZE; i++)
	{

	node->character[i] = NULL;
	}
	return node;
}

void insert2(struct trie2 *head, char* str)
{
	struct trie2* curr = head;
	while (*str)
	{
            if(*str==39)
        {
            if (curr->character[26] == NULL)
            {

          curr->character[26] = getNewTrie2Node();
            }
            curr = curr->character[26];
        }
        else if(*str=='-')
        {

            if (curr->character[27] == NULL)
            {

          curr->character[27] = getNewTrie2Node();
            }
            curr = curr->character[27];
        }
        else
            {
		if (curr->character[*str - 'a'] == NULL)
        {
			curr->character[*str - 'a'] = getNewTrie2Node();
        }
        curr = curr->character[*str - 'a'];

        }


		str++;
	}
	curr->isLeaf = 1;
}

int search2(struct trie2* head, char* str)
{
	if (head == NULL)
    {
		return 0;
    }
	struct trie2* curr = head;
	while (*str)
	{

        if(*str==39)
        {
		curr = curr->character[26];
        }
        else if(*str=='-')
        {
         curr = curr->character[27];
        }
		else
        {
            curr = curr->character[*str - 'a'];
        }
		if (curr == NULL)
        {
			return 0;
        }
		str++;
	}
	return curr->isLeaf;
}
int haveChildren2(struct trie2* curr)
{
	for (int i = 0; i < CHAR_SIZE; i++)
    {
		if (curr->character[i])
        {
			return 1;
        }
    }
	return 0;
}

int main()
{
    printf("\n\n\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SPELLING ERROR DETECTION AND CORRECTION~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n\n\t\t\t Detects incorrectly spelt words and gives suggestions or most similar words\n\t\t\t for the incorrectly spelt words. ");
    char cp[100];
    printf("\n\n\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    FILE *filep=fopen("dictionary1.txt","r");
    //file containing words in oxford dictionary.
    if(filep==NULL)
    {
        printf("\nerror reading dictionary file");
        exit(0);
    }
    struct Trie *head;
    head=form(filep);
    //constructing Trie from words of dictionary and loading them.
    printf("\n\n\t\t\tEnter the file name:");
    scanf("%s",cp);
    FILE *userp=fopen(cp,"r+");
    //user text file which has to be validated.
    FILE *Final=fopen("Final.txt","w");

    if(userp==NULL)
    {
        printf("\nerror reading  file - %s",cp);
        exit(0);
    }
    if(Final==NULL)
    {
        printf("\nerror opening file - Final.txt");
        exit(0);
    }
    head=ignore_user_words(head);
    int p=0;
    int i=0;
    int j=0;

    char ch;
    do
    {
        ch=fgetc(userp);
        if(isalpha(ch)||ch==39)
        {

            words_user_file[i][j]=ch;
            //constructing array of words from text file given by user.
            j=j+1;
        }
        else
        {
            j=0;
            i=i+1;
            k=i;
        }

    }while(ch!=EOF);
    k=k+1;
    struct trie2 *head2;
    head2=convert_to_lower();
    //converting all the words in user file to lower case.
    validate_userfile(head,Final);
    //printf("\n\t\t\tThe user file is validated and written in - Final.txt file...........");

   fclose(filep);
   fclose(userp);
   fclose(Final);

    return 0;
}

struct trie2* convert_to_lower()
{
    struct trie2* head2=getNewTrie2Node();
    char s[1000];
  int i,j;
  j=0;
  for(i=0;i<k+1;i++)
    {
        if(strcmp(words_user_file[i],"")!=0)
        {
           // printf("%s - ",words_user_file[i]);
            strcpy(s,strlwr(words_user_file[i]));
           // printf("%s - ",s);
            if(search2(head2,s)==0)
            {
                insert2(head2,s);
                strcpy(temp_lowercase[j],s);
                j++;
            }
        }
    }
    finalcount=j;
    return head2;
}
struct Trie* form(FILE *fp)
{
    char s[89];
    long long int i=0;
    struct Trie* head = getNewTrieNode();
    while(!(feof(fp)))
    {
     fscanf(fp,"%s",dict[i]);
    //printf("%s ",s);
        insert(head,dict[i]);
        i++;
    }
    dict_len=i;
    return head;
}

struct Trie* ignore_user_words(struct Trie* head)
{
    struct Trie *temp=head;
    printf("\n\t\t\tEnter number of words that have to be ignored:");
    int n;
    scanf("%d",&n);
    if(n>0)
    {
      printf("\n\t\t\tEnter the words:\n");
      int i;
      char s[1000];
      for(i=0;i<n;i++)
      {
          printf("\t\t\t");
          scanf("%s",s);
          insert(temp,s);
          strcat(s,"'s");
          insert(temp,s);
      }
    }
    return temp;
}
//268435456
void validate_userfile(struct Trie *head,FILE *fp)
{
    int n;
    int count=0;
    int i;
    int a[10000];
    char chp;
    bool flag=true;
    printf("\n\t\t\tDo you want to write the results to Final.txt file: yes(y), no(n):");
    scanf(" %c",&chp);
    if(chp=='y'||chp=='Y')
    {
        flag=true;
    }
    else if(chp=='n'||chp=='N')
    {
        flag=false;
    }
    for(i=0;i<finalcount;i++)
    {
            if(search(head,temp_lowercase[i])==0)
            {
                a[count]=i;
                count=count+1;
            }
    }
    if(count==0)
    {
        if(flag)
        {
            fprintf(fp,"\n\n\t\t\t%s %s %s %s %s\n","All","words","are","spelt","correctly.....");
        }
        printf("\n\n\t\t\t%s %s %s %s %s\n","All","words","are","spelt","correctly.....");
    }
    else
    {
        if(flag)
        {
            fprintf(fp,"\n\t\t\t%s %s %s %s %s %s %s\n", "The", "words", "which", "are", "spelt", "incorrectly" ,"are:");
        }
        printf("\n\t\t\t%s %s %s %s %s %s %s\n", "The", "words", "which", "are", "spelt", "incorrectly" ,"are:");

        for(i=0;i<count;i++)
        {
            if(flag)
            {
                fprintf(fp,"\n%d.%s ",i+1,temp_lowercase[a[i]]);
            }
                      printf("\n%d.%s ",i+1,temp_lowercase[a[i]]);
            if(flag)
            {
                fprintf(fp,"\nPossible correct words that are close to %s are:",temp_lowercase[a[i]]);
            }
            printf("\nPossible correct words that are close to %s are:",temp_lowercase[a[i]]);
            correct(temp_lowercase[a[i]],fp,flag);
        }
    }
    if(flag)
    {
        printf("\nThe results are also written to Final.txt file");
    }
}
void correct(char s[],FILE *fp,bool flag)
{

    long long int i=0;
    while(i<dict_len)
    {
        if(number_of_operations(s,dict[i],strlen(s),strlen(dict[i]))==1)
        {
            if(flag)
            {
                fprintf(fp,"%s  ",dict[i]);
            }
            printf("%s  ",dict[i]);
        }
        i++;
    }
    if(flag)
    {
        fprintf(fp,"\n\n");
    }
    printf("\n\n");
}
/*
Gives suggestions for incorrectly spelt words.
*/
int number_of_operations(char s1[], char s2[], int s1_len, int s2_len)
{

	int matrix[s1_len + 1][s2_len + 1];

	for (int i = 0; i <= s1_len; i++)
        {
		for (int j = 0; j <= s2_len; j++)
		 {
			if (i == 0)
            {
				matrix[i][j] = j;
            }

			else if (j == 0)
            {
				matrix[i][j] = i;
            }

			else if (s1[i - 1] == s2[j - 1])
            {

				matrix[i][j] = matrix[i - 1][j - 1];
            }

			else
            {
				matrix[i][j] = 1 + min(matrix[i][j - 1],
								matrix[i - 1][j],
								matrix[i - 1][j - 1]);
            }
		}
	}

	return matrix[s1_len][s2_len];
}

/*
Memoization matrix for finding number of operations to be performed
to change first string to second string using dynamic programming approach.
*/

int min(int x, int y, int z)
{
	int p;
	p=(x<y)?x:y;
	return (p<z)?p:z;

}
