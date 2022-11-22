#include <stdio.h>
#include <getopt.h>
#include "find.h"


int main(int argc, char **argv)
{
    struct option long_options[] = {
        {"words", required_argument, 0, 'w'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"exclude", required_argument, 0, 'e'},
        {"verbose", no_argument, 0, 'v'},
        {"report", required_argument, 0, 'r'},
        {"show", required_argument, 0, 's'},
        {"file", required_argument, 0, 'f'},
        {NULL, 0, NULL, 0}
    };
    struct pt_param wk_param = {NULL};
    int option_index = 0;
    int i;
    char *wk_var;
    while ((i = getopt_long(argc, argv, "w:i:e:o:v:r:s:f", long_options, &option_index)) != -1){ //Controllo parametri di input
        switch (i)
        {
        case 'w':
            //wk_param.word_file = AbsoluteP(optarg); // check e trasformazione in path assoluto
            wk_param.word_file = optarg; 
            break;
        case 'i':
            wk_param.input_file = optarg;
            break;
        case 'o':
            wk_param.output_file = optarg;
            break;
        case 'e':
            wk_param.extension = optarg;
            break;
        case 'v':
            wk_param.logs = 1;
            break;
        case 'r':
            wk_param.report = optarg;
            break;
        case 's':
            wk_param.show = optarg;
            wk_param.nTimes = 1;
            if (optind < argc){
                wk_param.nTimes = atoi(argv[optind]);
            }
            break;
        case 'f':
            wk_param.otherfile = optarg;
            break;
        }
    }
    StartFind(&wk_param);
    return 0;
}
