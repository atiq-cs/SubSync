/*******************************************************
*		Problem Name:			
*		Problem ID:				
*		Occassion:				_ Contest _ _ _
*
*		Algorithm:				
*		Special Case:			
*		Judge Status:			
*		Author:					Saint Atique
*		Notes:					
*								
*******************************************************/
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <cstring>
//#include <new>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>//for cout formatting
#define	INF 2147483648
#define EPS 1e-8
using namespace std;

// Gobal Variables and definitions
FILE* rfp;
FILE* wfp;

typedef struct TimeObjType {
	int hour, minute, second, milisec;
} TimeType;

void LoadTime(char *str, TimeType* T1, TimeType* T2) {
	char *delim=":, ";
	char *token;

	// Get hour
	token = strtok(str, delim);
	T1->hour = atoi(token);

	// Get minute
	token = strtok(NULL, delim);
	T1->minute = atoi(token);

	// Get second
	token = strtok(NULL, delim);
	T1->second = atoi(token);

	// Get mili-second
	token = strtok(NULL, delim);
	T1->milisec = atoi(token);

	// Omit the seperator
	token = strtok(NULL, delim);

	// Get hour
	token = strtok(NULL, delim);
	T2->hour = atoi(token);

	// Get minute
	token = strtok(NULL, delim);
	T2->minute = atoi(token);

	// Get second
	token = strtok(NULL, delim);
	T2->second = atoi(token);

	// Get mili-second
	token = strtok(NULL, delim);
	T2->milisec = atoi(token);
}

void AddTime (TimeType* TimeObj, const TimeType* diff) {
	int temp;

	temp = TimeObj->milisec + diff->milisec;
	if (temp >= 1000) {
		temp -= 1000;
		TimeObj->second++;
	}
	TimeObj->milisec = temp;

	temp = TimeObj->second + diff->second;
	if (temp >= 60) {
		temp -= 60;
		TimeObj->minute++;
	}

	TimeObj->second = temp;

	temp = TimeObj->minute + diff->minute;
	if (temp >= 60) {
		temp -= 60;
		TimeObj->hour++;
	}

	TimeObj->minute = temp;
	TimeObj->hour += diff->hour;
}

void SubtractTime (TimeType* TimeObj, const TimeType* diff) {
	int temp;

	// Check if time is less then offset
	if (TimeObj->hour < diff->hour)
		return;
	if (TimeObj->hour == diff->hour) {
		if (TimeObj->minute < diff->minute)
			return;
		else if (TimeObj->minute == diff->minute) {
			if (TimeObj->second < diff->second)
				return;
			else if (TimeObj->second == diff->second) {
				if (TimeObj->milisec < diff->milisec)
					return;
			}
		}
	}

	temp = TimeObj->milisec - diff->milisec;
	if (temp < 0) {
		temp += 1000;
		TimeObj->second--;
	}
	TimeObj->milisec = temp;

	temp = TimeObj->second - diff->second;
	if (temp < 0) {
		temp += 60;
		TimeObj->minute--;
	}

	TimeObj->second = temp;

	temp = TimeObj->minute - diff->minute;
	if (temp < 0) {
		temp += 60;
		TimeObj->hour--;
	}

	TimeObj->minute = temp;
	TimeObj->hour -= diff->hour;
}

void ShowTime(const TimeType* T1, const TimeType* T2) {
	fprintf(wfp, "%02d:%02d:%02d,%03d", T1->hour, T1->minute, T1->second, T1->milisec);
	fprintf(wfp, " --> %02d:%02d:%02d,%03d\n", T2->hour, T2->minute, T2->second, T2->milisec);
}

bool saIsDigit(char ch) {
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}

bool isNumber(const char* str) {
	int i, len = strlen(str);
	if (len == 0)
		return false;
	else if (len == 1 && str[len-1] == 10)
		return false;

	if (str[len-1] == 10)
		len--;
	for (i=0; i<len; i++)
		if (str[i] < '0' || str[i] > '9') {
			printf("invalied in %s ascii %d in pos %d\n", str, str[i], i);
			return false;
		}
	return true;

}

bool ContainsTimestamp(const char* str) {
	if (strlen(str)<17)
		return false;
	if (saIsDigit(str[0])) {
		if (str[1] == ':')
			return true;
		else if (saIsDigit(str[1]) && str[2] == ':')
			return true;
	}
	return false;
}

void SAOpenFile(char* fpath) {
	int len = strlen(fpath);
	char* readfilepath = NULL;
	char* writefilepath = NULL;

	// Validate file names
	if (!strcmp(&fpath[len-8], "_pre.srt")) {
		puts("Oh! Oversmart! But ok.");
		readfilepath = fpath;
		printf("rpath: %s\n", readfilepath);
		writefilepath = new char[len];
		strncpy(writefilepath, readfilepath, len-8);
		writefilepath[len-8] = '\0';
		strcat(writefilepath, ".srt");
		printf("wpath: %s\n", writefilepath);
	}
	else if (!strcmp(&fpath[len-4], ".srt")) {
		writefilepath = fpath;
		readfilepath = new char[len + 5];
		strncpy(readfilepath, writefilepath, len-4);
		readfilepath[len-4] = '\0';
		strcat(readfilepath, "_pre.srt");
	}
	else {
		puts("Wrong file extension!");
		exit(EXIT_FAILURE);
	}

	// Open files
	rfp= fopen(readfilepath, "r");
	if (!rfp) {
		puts("Read file Open Error! May be file doesn't exist. Please check the path.");
		exit(EXIT_FAILURE);
	}

	wfp= fopen(writefilepath, "w");
	if (!wfp) {
		puts("Read file Open Error! May be file doesn't exist. Please check the path.");
		exit(EXIT_FAILURE);
	}

	if (readfilepath != fpath)
		delete readfilepath;
	if (writefilepath != fpath)
		delete writefilepath;
}

int ReadSequenceNumber(const char* str) {
	printf("Got offset: %s\n", str);
	if (isNumber(str))
		return atoi(str);
	return 0;
}

// ReadDiffTime(argv[2], &DiffTime, &IsSubFast);
void ReadDiffTime(char *difftimestr, TimeType* difftime, bool* isfast) {
	if (*difftimestr == '+') {
		*isfast = true;
		difftimestr++;
	}
	else if (*difftimestr == '-') {
		*isfast = false;
		difftimestr++;
	}
	else if (saIsDigit(*difftimestr) == false)  {
		puts("Provided time is incorrect!");
		exit(EXIT_FAILURE);
	}
	else // still correct; default is true
		*isfast = true;

	char *delim=":, ";
	char *token;

	// Get hour
	token = strtok(difftimestr, delim);
	difftime->hour = atoi(token);

	// Get minute
	token = strtok(NULL, delim);
	difftime->minute = atoi(token);

	// Get second
	token = strtok(NULL, delim);
	difftime->second = atoi(token);

	// Get mili-second
	token = strtok(NULL, delim);
	difftime->milisec = atoi(token);
}

int main(int argc, char* argv[]) {
	bool IsSubFast = true;
	char linestr[300];

	puts("\nSA SybSync 0.94\n");
	if (argc != 4) {
		printf("You provided %d number of cmd args. They are\n", argc);
		for (int i=1; i<argc; i++)
			printf("%d. %s\n", i, argv[i]);

		puts("SubMod: incorrect commandline. Number of arguments should be 3. They are:"
			"\n1. srt file location"
			"\n2. [+/-]H:M:S,M (+ to make delay)"
			"\n3. Sequence Offset (put a zero for default)");
		exit(EXIT_FAILURE);
	}

	// Validate and process arg 1
	SAOpenFile(argv[1]);

	TimeType BeginTime, EndTime, DiffTime;
	// Validate and process arg 2
	ReadDiffTime(argv[2], &DiffTime, &IsSubFast);

	int sequenceOffset = ReadSequenceNumber(argv[3]);
	printf("Got offset here: %d\n", sequenceOffset);

	while (fgets(linestr, 300, rfp)) {
		int len = strlen(linestr);
		if (len<4) {
			//printf("got line %s\n", linestr);
			//if (linestr[len-1] == 10)
				//linestr[len-1] = 0;
			if (isNumber(linestr)) {
				//printf("str: %s is number (%d)\n", linestr, len);
				int x = atoi(linestr) + sequenceOffset;
				fprintf(wfp, "%d\n", x);
				continue;
			}
		}

		if (ContainsTimestamp(linestr) == false) {
			fputs(linestr, wfp);
			continue;
		}

		// remove spaces from beginning and trail
		LoadTime(linestr, &BeginTime, &EndTime);
		if (IsSubFast == true) {
			AddTime(&BeginTime, &DiffTime);
			AddTime(&EndTime, &DiffTime);
		}
		else {
			SubtractTime(&BeginTime, &DiffTime);
			SubtractTime(&EndTime, &DiffTime);
		}
		
		ShowTime(&BeginTime, &EndTime);
	}
	
	fclose(rfp);
	fclose(wfp);

	return 0;
}
