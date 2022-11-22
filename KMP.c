#include "KMP.h"

int arrInd = 0;
int indexFind = 0;
// Scorro le words[] -> KMP su pathFile
void get_matches(char *pathFile){
	MsgBox("Inizio elaborazione file: %s\n", pathFile);
	FILE *filePath;
	filePath = fopen(pathFile, "r");
	if (filePath == NULL){
		MsgFileError("o", pathFile, errno); 
		exit(0);
	}
	size_t length = 0;
	ssize_t nread;
	char *line = NULL;
	int i = 0;
	int nShift;
	int nLine;
	for (arrInd = 0; find_word[arrInd] != NULL; arrInd++){
		nShift = 0;
		nLine = 1;
		for (indexFind = 0; find_word[arrInd]->in_path[indexFind] != NULL; indexFind++)  //scorro le words
		{}
		fseek(filePath, 0, SEEK_SET);
		find_word[arrInd]->in_path[indexFind] = malloc(sizeof(struct in_path));
		find_word[arrInd]->in_path[indexFind]->path = malloc((strlen(pathFile)) + 1);
		strcpy(find_word[arrInd]->in_path[indexFind]->path, pathFile);
		find_word[arrInd]->in_path[indexFind]->occurrences = 0;
		find_word[arrInd]->in_path[indexFind]->position[nShift] = 0;
		while ((nread = getline(&line, &length, filePath)) != -1){
			if (line[nread - 1] == '\n'){ 
				line[nread - 1] = '\0';//rimuove '\n'
			}
			if (KMPcheck(line, find_word[arrInd]->word, strlen(line), strlen(find_word[arrInd]->word), nShift, nLine) == 1){
				find_word[arrInd]->in_path[indexFind]->line[nShift] = nLine;
				nShift++;
			}
			nLine++;
		}
		find_word[arrInd]->tot_occurrences += find_word[arrInd]->in_path[indexFind]->occurrences;
	}
	MsgBox("Fine elaborazione file: %s\n", pathFile);
	fclose(filePath); 
}
//Core dell'algoritmo KMP
int KMPcheck(const char *text, const char *word, int textLen, int wordLen, int nShift, int nLine){
	int ret = 0;
	// Se word è vuota
	if (*word == '\0' || wordLen == 0)
	{
		find_word[arrInd]->in_path[indexFind]->position[nShift] = 0;
		find_word[arrInd]->in_path[indexFind]->occurrences += 1;
		ret = 1;
	}
	// Se text (line) è vuota
	if (*text == '\0' || wordLen > textLen)
	{
		return -1;
	}
	int next[wordLen + 1];
	for (int i = 0; i < wordLen + 1; i++)
		next[i] = 0;
	for (int i = 1; i < wordLen; i++){
		int j = next[i + 1];
		while (j > 0 && word[j] != word[i])
			j = next[j];
		if (j > 0 || word[j] == word[i])
			next[i + 1] = j + 1;
	}
	int add = 0;
	for (int i = 0, j = 0; i < textLen; i++){
		if (*(text + i) == *(word + j)){
			if (++j == wordLen){
				find_word[arrInd]->in_path[indexFind]->position[nShift + add] = (i - j + 1);
				find_word[arrInd]->in_path[indexFind]->occurrences += 1;
				ret = 1;
				if (add > 0){ //se ho due corrispondenze nella stessa riga
					find_word[arrInd]->in_path[indexFind]->line[nShift +add] = nLine;				
				}
				++add;
			}
		}
		else if (j > 0){
			j = next[j];
			i--; 
		}
	}
	return ret;
}
