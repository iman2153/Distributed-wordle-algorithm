#include <iostream>
#include "ecs36b_Exception.h"

// JSON RPC part
#include "hw6client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

//Setting
string team_name = "TLC United";
//-------

//HttpClient httpclient("http://127.0.0.1:8384");
HttpClient httpclient("https://5f43-2601-200-c001-9ff0-d5cd-ca2a-e2c4-2fcc.ngrok.io");
hw6Client myClient(httpclient, JSONRPC_CLIENT_V2);
Json::Value myv;
Json::Value jv_list;
int game_count = 0;

FILE * fpAll,  * fpSol;
char word[6] = {0};
long wordCount = 0;

//Game looping functions
bool is_answer_found();
void game_loop();
void global_loop();
void new_game();
//----------------------

//Algorithm variables---------------------------
#define WORD_LENGTH 5
#define ALPHA_SIZE 27
char wordlist[5*3000];
char *pWordlist = wordlist;
char lastGuess[6];
char mustalpha[6];
char mustnoalpha[6];
char includealpha[ALPHA_SIZE];
char excludealpha[ALPHA_SIZE];
char fullalpha[ALPHA_SIZE] = "abcdefghijklmnopqrstuvwxyz";
int toLower(char * str);
void reset_variables();
void rememberAlpha(const char *theguess, char *feedback);
int isPossible(char *answer);
int robot(char *guess);
//-----------------------------------

//Algorithm-------------------------
int toLower(char * str) {
	int i = 0;
	while (str[i]) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] |= 0x20; //Make lowercase
		}
		++i;
	}
	return i;
}

void reset_variables() {
  pWordlist = wordlist;
  for (int i = 0; i < 6; ++i) {
    lastGuess[i] = '_';
    mustalpha[i] = '_';
    mustnoalpha[i] = '_';
  }
  for (int i = 0; i < ALPHA_SIZE; ++i) {
    includealpha[i] = '_';
    excludealpha[i] = '_';
  }
}

void rememberAlpha(const char *theguess, char *feedback) {
	for (int i = 0; i < WORD_LENGTH; ++i) {
		if (feedback[i] != '_') continue;
		for (int j = 0; j < WORD_LENGTH; ++j) {
			if (feedback[j] == '_') continue;
			if (theguess[i] == theguess[j]) feedback[i] = 'o';
		}
	}
	for(int i = WORD_LENGTH; i >= 0; i--) {
		char v = theguess[i];
		int v_index = v - 'a';
		char f = feedback[i];
		bool skip = false;
		for(int j = i - 1; j >= 0; j--) {
			char v_duplicate = theguess[j];
			if(v == v_duplicate) {
				skip = true;
				break;
			}
		}
		if(f == '#') {
			mustalpha[i] = v;
		}
		if(f == 'o') {
			mustnoalpha[i] = v;
		}
		if(skip) continue;
		if (f == 'o' || f == '#') {
			includealpha[v_index] = v;
		} else if (f == '_' ) {
			excludealpha[v_index] = v;
		}
	}
}

int isPossible(char *answer) {
	for(int i = 0; i < 26; i++) {
		char include = includealpha[i];
		char exclude = excludealpha[i];
		char alpha_v = fullalpha[i];
		bool includeCheck = false;
		for (int answer_i = 0; answer_i < 5; answer_i++) {
			char answer_v = answer[answer_i];
			char must = mustalpha[answer_i];
			if(must != '_' && must != answer_v) return 0;
			char mustno = mustnoalpha[answer_i];
			if(mustno != '_' && mustno == answer_v) return 0;
			if(include != '_' && include == answer_v) {
				includeCheck = true;
			}
			if(alpha_v == answer_v) {
				if(exclude == answer_v) {
					return 0;
				}
			}
		}
		if(include != '_' && !includeCheck) return 0;
	}
	return 1;
}

int robot(char *guess) {
	for(; pWordlist < wordlist + WORD_LENGTH * wordCount; pWordlist += WORD_LENGTH) {
		if(!isPossible(pWordlist)) continue;
		bool repeated = true;
		memcpy(guess, pWordlist, WORD_LENGTH);
		for(int i = 0; i < WORD_LENGTH; i++) {
			if(lastGuess[i] != guess[i]) repeated = false;
		}
		if(repeated) continue;
		memcpy(lastGuess, pWordlist, WORD_LENGTH);
		pWordlist += WORD_LENGTH;
		printf("my guess is：%s\n", guess);
		return 1;
	}
	return 0;
}
//-------------------------------

bool is_answer_found() {
  return myv["check_word"].asString() == "#####";
}

void game_loop() {
  char guess[6] = {0};
  while (!is_answer_found()) {
    if (robot(guess) == 1 && strlen(guess) == WORD_LENGTH) {
      toLower(guess);
      if ((myv["game_id"].isNull() == false) && (myv["game_id"].isString() == true)) {
        try {
          myv = myClient.guess("guess", "Wordle", myv["game_id"].asString(), guess);
        } catch (JsonRpcException &e) {
          cerr << e.what() << endl;
        }
        string feedback_str = myv["check_word"].asString();
        char feedback[feedback_str.length()];
        strcpy(feedback, feedback_str.c_str());
        rememberAlpha(guess, feedback);
      }
    } else {
      printf("%s", guess);
      printf("Invalid word. Must be %i characters\n", 5);
      break;
    }
  }
  if (is_answer_found()) {
    printf("success:  %s\n", guess);
  }
  try {
    myv = myClient.submit("submit", myv["game_id"].asString(), team_name);
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}

void new_game() {
  ++game_count;
  // obtain and new
  try {
    myv = myClient.obtain("obtain", "Wordle", "00000000");
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}

void global_loop() {
  while (game_count < 100) {
    reset_variables();
    new_game();
    game_loop();
    printf("Gain %s points\n", myv["remaining_score"].asString().c_str());
  }
}

int main() {
  //Read Wordlist
  char currentWord[WORD_LENGTH + 1];
  fpAll = fopen("lists/ALL.TXT", "r");
  fpSol = fopen("lists/SOLUTION.TXT", "r");
  fseek(fpSol, 0, SEEK_SET);
	while (fgets(currentWord, WORD_LENGTH + 1, fpSol) != NULL) {
		if (strlen(currentWord) == WORD_LENGTH) {
			memcpy(wordlist + WORD_LENGTH * wordCount, currentWord, WORD_LENGTH);
			++wordCount;
		}
	}
  //-------------------------

  //Initialization starts
  jv_list[0] = "921010218";
  jv_list[1] = "921024194";
  jv_list[2] = "919659542";
  jv_list[3] = "919628939";
  try {
    myv = myClient.set_name("set_name", jv_list, team_name);
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  //Initialization completes

  global_loop();

  fclose(fpAll);
  fclose(fpSol);
  return 0;
}
