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
		char inChar[MAX_IN_CHAR];

#define MAX_TAPE_CHAR 256
		int numTapeChar;
		char tapeChar[MAX_TAPE_CHAR];

#define MAX_NUM_ACCEPT 256
		int numAccept;
		char *accept[MAX_NUM_ACCEPT];

#define MAX_LEN_STATE 256
		char q0[MAX_LEN_STATE];
		char B;

#define MAX_NUM_TRANS 1024
		int numTrans;
		char *trans[MAX_NUM_TRANS][5];

#define TAPE_MIDDLE 10000
#define MAX_LEN_TAPE (2 * TAPE_MIDDLE)
#define MAX_NUM_TAPE 64
		int numTape = 0;
		char tape[MAX_NUM_TAPE][MAX_LEN_TAPE];
		int head[MAX_NUM_TAPE], leftmost[MAX_NUM_TAPE], rightmost[MAX_NUM_TAPE];

		char curstate[1024];
		char curtape[MAX_NUM_TAPE];
#define POS(idx) (idx + TAPE_MIDDLE)

		void extractToken(int *tot, char *ss[], char *s, char *buf, bool isString)
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
				if(isString)
				{
					ss[(*tot)] = (char *)malloc(strlen(c) + 1);
					ss[(*tot)][0] = '\0';
					strcpy(ss[(*tot)], c);
				}
				else
				{
					s[(*tot)] = c[0];
				}
				c = strtok(NULL, ",");
			}
		}

		_turing()
		{
			numTrans = 0;
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
					if(buf[1] == 'Q')extractToken(&numState, state, NULL, buf, true);
					else if(buf[1] == 'S')extractToken(&numInChar, NULL, inChar, buf, false);
					else if(buf[1] == 'G')extractToken(&numTapeChar, NULL, tapeChar, buf, false);
					else if(buf[1] == 'q')q0[0] = '\0', strcpy(q0, &buf[6]);
					else if(buf[1] == 'B')B = buf[5];
					else if(buf[1] == 'F')extractToken(&numAccept, accept, NULL, buf, true);
					else if(buf[1] == 'N')numTape = atoi((const char*)(&buf[5]));
				}
				else if((buf[0] >= 'a' && buf[0] <= 'z') || (buf[0] >= 'A' && buf[0] <= 'Z') || (buf[0] >= '0' && buf[0] <= '9') || buf[0] == '_')
				{
					numTrans++;
				  char *c = strtok(buf, " ");
					int cnt = -1;
          while(c != NULL)
          {
						cnt++;
						trans[numTrans][cnt] = (char *)malloc(strlen(c) + 1);
          	trans[numTrans][cnt][0] = '\0';
          	strcpy(trans[numTrans][cnt], c);
          	c = strtok(NULL, " ");
          }
				}
			}
			fclose(fp);

			for(int i = 0; i < numTape; i++)
      	for(int j = 0; j < MAX_LEN_TAPE; j++)
      		tape[i][j] = '_';
      for(int i = 0; i < numTape; i++)
			{
      	head[i] = TAPE_MIDDLE;
				leftmost[i] = TAPE_MIDDLE;
				if(i == 0)rightmost[i] = POS(strlen(w));
				else rightmost[i] = TAPE_MIDDLE;
			}
			for(int i = 0; i < strlen(w); i++)
				tape[0][POS(i)] = w[i];
			strcpy(curstate, q0);
		}	
		~_turing()
		{
			for(int i = 1; i <= numState; i++)
				free(state[i]);
			for(int i = 1; i <= numAccept; i++)
				free(accept[i]);
			for(int i = 1; i <= numTrans; i++)
      	for(int j = 0; j <= 4; j++)
      		free(trans[i][j]);
		}
		bool isHalt(char *s)
		{
			for(int i = 1; i <= numAccept; i++)
			{
				if(strcmp(s, accept[i]) == 0)return true;
			}
			return false;
		}
		void forward()
		{
			while(!isHalt(curstate))
			{
				for(int i = 0; i < numTape; i++)
					curtape[i] = tape[i][head[i]];
				curtape[numTape] = '\0';
				int k = 0;
				for(k = 1; k <= numTrans; k++)
				{
					if(strcmp(curstate, trans[k][0]) == 0 && strcmp(curtape, trans[k][1]) == 0)
					{
						for(int j = 0; j < numTape; j++)
						{
							tape[j][head[j]] = trans[k][2][j];
							if(trans[k][3][j] == 'l')
							{
								head[j]--;
								if(head[j] < leftmost[j])leftmost[j] = head[j];
							}
							else if(trans[k][3][j] == 'r')
							{
								head[j]++;
								if(head[j] > rightmost[j])rightmost[j] = head[j];
							}
						}
						strcpy(curstate, trans[k][4]);
						break;
					}
				}
				if(k > numTrans)break;
			}	
			int left = leftmost[0], right = rightmost[0];
			while(left < right && tape[0][left] == '_')left++;
			while(left < right && tape[0][right] == '_')right--;
			right++;
			for(int i = left; i < right; i++)
			{
				printf("%c", tape[0][i]);
			}
			printf("\n");
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

	TM.forward();
	/*
	for(int i = 1; i <= TM.numState; i++)
		printf("%s ", TM.state[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numInChar; i++)
		printf("%c ", TM.inChar[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numTapeChar; i++)
		printf("%c ", TM.tapeChar[i]);
	printf("\n\n");
	
	for(int i = 1; i <= TM.numAccept; i++)
  	printf("%s ", TM.accept[i]);
  printf("\n\n");
	
	printf("%d %s %c", TM.numTape, TM.q0, TM.B);
	printf("\n\n");

	for(int i = 1; i <= TM.numTrans; i++)
	{
		for(int j = 0; j <= 4; j++)
			printf("%s ", TM.trans[i][j]);
		printf("\n");
	}*/
	return 0;
}
