#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
using namespace std;
//parse_args
struct option long_options[] = 
{
	{"help", no_argument, NULL, 'h'},
	{"verbose", no_argument, NULL, 'v'},
	{0, 0, 0, 0}
};
extern int optind;
bool verbose = false;
char turingfile[1024] = "\0";
char w[1024] = "\0";

//temporary structure


//build_turing
class _turing
{
	public:
		char buf[10000];

#define MAX_NUM_STATE 256	
		int numState;
		char *state[MAX_NUM_STATE];

#define MAX_IN_CHAR 128
		int numInChar;
		char *inChar[MAX_IN_CHAR];

#define MAX_TAPE_CHAR 256
		int numTapeChar;
		char *tapeChar[MAX_TAPE_CHAR];

#define MAX_NUM_ACCEPT 256
		int numAccept;
		char *accept[MAX_NUM_ACCEPT];

		char q0, B;

#define TAPE_MIDDLE 10000
		char tape[64][2 * TAPE_MIDDLE];
		int head[64];
		int numTape = 0;

		_turing()
		{
			FILE *fp = fopen(turingfile, "r");
			if(fp == NULL)
			{
				printf("failed to open the file!\n");
				exit(EXIT_FAILURE);
			}
			while(!feof(fp))
			{
				fscanf(fp, "%[^\n]\n", buf);
				if(buf[0] == '#')
				{
					if(buf[1] == 'Q')extractTokens(&numState, state, &buf[0]);
					else if(buf[1] == 'S')extractTokens(&numInChar, inChar, &buf[0]);
					else if(buf[1] == 'G')extractTokens(&numTapeChar, tapeChar, &buf[0]);
					else if(buf[1] == 'F')extractTokens(&numAccept, accept, &buf[0]);
					else if(buf[1] == 'N')
					{
						int l = strlen(buf), p = 0;
            while(p < l && (buf[p] < '0' || buf[p] > '9'))p++;
						numTape = atoi((const char*)(&buf[p]));
					}
				}
				else if((buf[0] >= 'a' && buf[0] <= 'z') || (buf[0] >= 'A' && buf[0] <= 'Z') || (buf[0] >= '0' && buf[0] <= '9') || buf[0] == '_')
				{
				}
			}
			fclose(fp);
		}	


		void extractTokens(int *tot, char *s[], char *buf)
    {
    	(*tot) = 0;
      int l = strlen(buf), p = 0;
      char *c = NULL;
      while(p < l && buf[p] != '{')p++;
      while(l >= 0 && buf[l] != '}')l--;
      buf[l] = '\0';
      c = strtok(&buf[p + 1], ",");
      while(c != NULL)
      {
      	(*tot)++;
    		s[(*tot)] = (char *)malloc(strlen(c) + 1);
      	s[(*tot)][0] = '\0';
      	strcpy(s[(*tot)], c);
      	c = strtok(NULL, ",");
      }
    }

		~_turing()
		{
			//free memory previously allocted in _turing()
		}

};


void parse_args(int argc, char *argv[])
{
	int o = 0;
	while((o = getopt_long(argc, argv, "hv", long_options, NULL)) != -1)
	{
		if(o == 'h')
		{
			printf("usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
			exit(0);
		}
		else if(o == 'v')verbose = true;
		else 
		{
			printf("usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n");
			exit(0);	
		}
	}
	sprintf(turingfile, "%s", argv[optind]);
	sprintf(w, "%s", argv[optind + 1]);
}
int main(int argc, char *argv[])
{
	parse_args(argc, argv);
	_turing TM;
	for(int i = 1; i <= TM.numState; i++)
		printf("%s ", TM.state[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numInChar; i++)
		printf("%s ", TM.inChar[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numTapeChar; i++)
		printf("%s ", TM.tapeChar[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numAccept; i++)
  	printf("%s ", TM.accept[i]);
  printf("\n\n");
	
	printf("%d", TM.numTape);
	printf("\n\n");
	return 0;
}
