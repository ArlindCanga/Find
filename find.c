#include "find.h"
int ToDo = 0;
char *wk_word_path;
char *excludeext;

void StartFind(struct pt_param *main_param){
    CheckParam(main_param);
    switch (ToDo){
        case 1:  
        //find
            wk_word_path = main_param->word_file; // mi salvo il path delle word, evito di rileggerlo
            excludeext = main_param->extension; //extension da escludere
            takeWords(main_param->word_file);
            ScanPaths(main_param->input_file);
            sortByOcc();        // Sort in odrine decrescente per occorrenze
            printResult(main_param); //print Stampa a terminale e scrive su file
            break;
        case 2: 
        // -- report --show
            FromReport(main_param->report, main_param->show,main_param->nTimes);
            break;
        case 3:
        // --report --show --file
            FromFile(main_param->report, main_param->show,main_param->nTimes,main_param->otherfile);
            break;
    }
}

// Controllo che i parametri di input siano validi
void CheckParam(struct pt_param *main_param){
    int CanGo = 1;
    if (main_param->word_file == NULL){
        CanGo = 0;
    }
    if (main_param->input_file == NULL){
        CanGo = 0;
    }
    if (CanGo == 0){
        if (main_param->report != NULL){
            if (main_param->show == NULL){
                printf("Non posso fare un report se non mi dici la parola da cercare.\n");
                CanGo = 1;
            }
            else {
                CanGo = 1;
                ToDo = 2;
                if (main_param->otherfile != NULL){
                    ToDo = 3;
                }
            }
        }
    }
    else
    {
        ToDo = 1;
    }  
    if (CanGo == 0){
        printf("Errore! I parametri inseriti non sono corretti.\n");
        exit(1);
    }
    setLogOn(main_param ->logs);
}
//Funzione utile al qsort
int myCompare(const void *s1, const void *s2){
    struct find_word **pa = (struct find_word**)s1;
    struct find_word **pb = (struct find_word**)s2;
    return strcmp((*pa)->word, (*pb)->word);
}
//read words -> qsort
void takeWords(char *wordspath){
    FILE *wordfile;
    wordfile = fopen(wordspath, "r"); //apro file delle parole da cercare
    if (wordfile == NULL){
        MsgFileError("o", wordspath, errno);
        exit(1);
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int nWord = 0;
    while ((nread = getline(&line, &len, wordfile)) != -1){
        if (line[nread - 1] == '\n'){ 
            line[nread - 1] = '\0'; //Tolgo '\n'
        }
        find_word[nWord] = malloc(sizeof(find_word));
        find_word[nWord]->word = malloc(strlen(line) + 1); 
        strcpy(find_word[nWord]->word, line);              
        nWord++;
    }
    fclose(wordfile);
    qsort (find_word, nWord , sizeof(struct find_word*), myCompare); //qsort-> ordinamento alfabetico
}
// Leggo il file dei path -> scansiono la directory -> find words nelle directory
void ScanPaths(char *input_path){
    FILE *pathfile;              
    pathfile = fopen(input_path, "r"); 
    if (pathfile == NULL){
        MsgFileError("o", input_path, errno);        
        exit(0);
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t readLine;
    while ((readLine = getline(&line, &len, pathfile)) != -1) {
        if (line[readLine - 1] == '\n'){ 
            line[readLine - 1] = '\0';
        }
        char *path_r = strtok(line, " ");
        path_r = strtok(NULL, " "); // testo se ho un \r dopo il path
        //line = AbsoluteP(line);
        if (path_r != NULL)
            scan_dir(line, path_r); //scan con r
        else
            scan_dir(line, " "); 
    }
    fclose(pathfile); //chiud    
}
// Scannerizza una directory e cerca le words nei file presenti
void scan_dir(char *path, char *flag){ 
    DIR *dirp; 
    struct dirent *direntp;
    dirp = opendir(path); 
    if (!(dirp = opendir(path))){
        printf("L'apertura di '%s' è fallita, codice errore: '%d'. Non posso cercare qui\n",path, errno);
    }
    else
    {
        while ((direntp = readdir(dirp)) != NULL){
            char dir[PATH_SIZE];                                                
            snprintf(dir, sizeof(dir), "%s/%s", path, direntp->d_name); 
            if (direntp->d_type == DT_DIR){
                if (strcmp(direntp->d_name, ".") == 0 
                    || strcmp(direntp->d_name, "..") == 0 //dir vuota
                    || strcmp(direntp->d_name, wk_word_path) == 0){ //file delle words, non devo cercare qui   
                    continue;
                }
                else{
                    if (strcmp(flag, "r") == 0) // se ho 'r' diventa uno scan ricorsivo
                        scan_dir(dir, flag); // 
                }
            }
            else{
                if (Exclude(dir) == 0){
                    get_matches(dir); //prendo occorrenze del file
                }
            }
        }
    }
}

int Exclude(char *path){
    int ret = 0;
    char *s = strrchr(path, '.'); //prendo l'estensione
    if (s){
        char *ext = strtok(excludeext, "_");
        while (ext != NULL){
            if (strcmp(s + 1, ext) == 0){
                ret = 1;
                break;
            }
            ext = strtok(NULL, "_");
        }
    }
    return ret;
}

void sortByOcc(){
    int x, y, z;
    for (z = 0; find_word[z] != NULL; z++){
        for (x = 0; x < find_word[z]->tot_occurrences - 1; x++){
            for (y = 0; y < find_word[z]->tot_occurrences - x - 2; y++){
                if (find_word[z]->in_path[y]->occurrences < find_word[z]->in_path[y + 1]->occurrences){
                    struct in_path *wk_find = find_word[z]->in_path[y];
                    find_word[z]->in_path[y] = find_word[z]->in_path[y + 1];
                    find_word[z]->in_path[y + 1] = wk_find;
                }
            }
        }
    }
}
/* ----------- Report --------- */
void FromReport(char *reportFile, char *reportWord, int times){
    FILE *repFile;
    repFile = fopen(reportFile, "r");
    if (repFile == NULL){
        MsgFileError("o", reportFile, errno);  
        exit(1);      
    }
    char line[LINE_SIZE];
    char saveLine[LINE_SIZE];
    char wkvar[PATH_SIZE];
    char path[PATH_SIZE];
    int FileOcc;
    char *token;
    char word[WORD_SIZE];
    strcpy(word, "WORD ");
    strcat(word, reportWord);
    while (fgets(line, PATH_SIZE, repFile) != NULL){
        if (strstr(line, word) != NULL){
            strcpy(saveLine, line);
            //printf("%s", line);
            while ((fgets(line, PATH_SIZE, repFile) != NULL) && (sscanf(line, "WORD %s\n", wkvar) == 0)){
                if (sscanf(line, "FILE %s\n", path) == 1){
                    fgets(line, PATH_SIZE, repFile);
                    token = strtok(line, " ");
                    token = strtok(NULL, " ");
                    FileOcc = atoi(token);
                    if (times<=FileOcc){
                        printf("%s", saveLine);
                        printf("FILE  %s\n", path);
                        printf("OCCURRENCES %d\n\n", FileOcc);
                    }
                }
            }
        }
    }
    fclose(repFile);
}

void FromFile(char *reportFile, char *reportWord, int times, char* otherFile){
    FILE *repFile;
    repFile = fopen(reportFile, "r");
    if (repFile == NULL){
        MsgFileError("o", reportFile, errno);  
        exit(1);      
    }
    char line[LINE_SIZE];
    char wkvar[LINE_SIZE];
    char buffer[LINE_SIZE];
    char *absolutePath = realpath(otherFile, buffer);
    char filetocheck[LINE_SIZE] = "FILE ";
    strcat(filetocheck, absolutePath);
    unsigned int posizione;
    unsigned int row;
    int FileOcc;
    char *token;
    char word[WORD_SIZE];
    strcpy(word, "WORD ");
    strcat(word, reportWord);
    strcat(filetocheck, " \r\n");
    while (fgets(line, LINE_SIZE, repFile) != NULL){
        if (strstr(line, word) != NULL){
            printf("%s", line);
            while ((fgets(line, LINE_SIZE, repFile) != NULL) && (sscanf(line, "WORD %s\n", wkvar) == 0)){
                if (strcmp(filetocheck, line) == 0){
                    fgets(line, LINE_SIZE, repFile);
                    token = strtok(line, " ");
                    token = strtok(NULL, " ");
                    FileOcc = atoi(token);
                    while ((fgets(line, LINE_SIZE, repFile) != NULL) && (--FileOcc>=0)){
                        sscanf(line, "%i %i",&row,&posizione);                        
                        printf("Linea:%i  Posizione:%i\n", row, posizione);
                    }
                }
            }
        }
  }
  fclose(repFile);
}