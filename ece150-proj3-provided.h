#ifndef _ECE150_PROJ_3_PROVIDED
#define _ECE150_PROJ_3_PROVIDED

struct uri {
    char *name;
    unsigned size, priority;
    uri *next;
};

struct timestamp {
    unsigned hh, mm, ss;
};


struct request {
    char *name;
    timestamp t;
    request *next;
};

timestamp maxClientDelay(uri *, request *);

#endif
