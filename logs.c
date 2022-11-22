#include "logs.h"

int log_on = 0;

void setLogOn(int wk_logs){
    log_on = wk_logs;
}
//Se il verbose è attivo stampo a video il processo di elaborazione
void MsgBox(char *message, ...){
    if (log_on == 1){
        va_list args;
        va_start(args, message);
        vprintf(message, args); //stampa a video un dato formattato
        va_end(args);
    }
}
//Errori di messaggio nelle varie operazioni dei file
void MsgFileError(char *operation, char *path, int nerror){
    if (strcmp(operation, "o") == 0){ //open
        printf("L'apertura del file '%s' è fallita, codice errore: %d\n", path, nerror);
    }
}

//Print su terminale e scrittura del file di output se --o
void printResult(struct pt_param *main_param){
    FILE *outFile = stdout;
    if (main_param->output_file != NULL){
        outFile = fopen(main_param->output_file, "w");
    }
    for (int x = 0; find_word[x] != NULL; x++){
        fprintf(outFile, "WORD %s \r\n", find_word[x]->word);
        fprintf(outFile, "TOTAL %d \r\n", find_word[x]->tot_occurrences);
        for (int y = 0; find_word[x]->in_path[y] != NULL; y++)
        {
            fprintf(outFile, "FILE %s \r\n", find_word[x]->in_path[y]->path);
            fprintf(outFile, "OCCURRENCES %d \r\n", find_word[x]->in_path[y]->occurrences);
            int shift = 0;
            for (shift = 0; shift < find_word[x]->in_path[y]->occurrences; shift++){
                fprintf(outFile, "%d %d \r\n", find_word[x]->in_path[y]->line[shift], find_word[x]->in_path[y]->position[shift]);
            }
        }
    }
    fclose(outFile);
}


