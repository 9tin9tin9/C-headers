#ifndef _OPTS_H_
#define _OPTS_H_

#include "../container/hashmap.h"
#include "../container/vec.h"
#include "for.h"
#include <stdbool.h>

struct ParseOpt_Opt{
    char* opt;
    //  expectes how many args after an opt
    const int argc;
};

static inline size_t __pareOpt_argvContainsOpt(int, char**, char*);
static inline size_t __parseOptLen(struct ParseOpt_Opt* opts);

// opts ends with { NULL, 0 }
HMap(Vec(char*))
parseOpt(
    int argc,
    char** argv,
    char*** nonOptArg,
    struct ParseOpt_Opt* opts)
{
    HMap(Vec(char*)) result = HMap_new(Vec(char*));
    bool usedArgv[argc];
    memset(usedArgv, 0, sizeof(usedArgv));
    int usedArgc = 0;

    size_t optc = __parseOptLen(opts);
    
    foreach_range(i, 0, optc){
        struct ParseOpt_Opt* opt = &opts[i];
        size_t pos = __pareOpt_argvContainsOpt(argc, argv, opt->opt);
        if (pos){
            // does not report error if size(optargs) != opt->argc
            Vec(char*) optargs = Vec_new(char*);
            foreach_range(j, 0, opt->argc){
                size_t index = pos+1+j;
                if (index >= argc || argv[index][0] == '-'){
                    break;
                }
                Vec_push_back(optargs, argv[index]);
                usedArgv[index] = 1;
                usedArgc++;
            }
            HMap_insert(result, opt->opt, optargs);
        }
    }

    if (nonOptArg){
        *nonOptArg = malloc((usedArgc+1)*sizeof(char*));
        size_t ptr = 0;
        foreach_range(i, 0, argc){
            if (!usedArgv[i]){
                (*nonOptArg)[ptr++] = argv[i];
            }
        }
        nonOptArg[ptr] = NULL;
    }

    return result;
}

static inline size_t
__parseOptLen(struct ParseOpt_Opt* opts){
    size_t count = 0;
    struct ParseOpt_Opt end = { NULL, 0 };
    for (struct ParseOpt_Opt* it = opts;
         memcmp(it, &end, sizeof(end));
         it++, count++) ;
    return count;
}

// linear search
static inline size_t
__pareOpt_argvContainsOpt(int argc, char** argv, char* opt){
    foreach_range(i, 0, argc){
        if (!strcmp(argv[i], opt)){
            return i;
        }
    }
    return 0;
}

#endif
