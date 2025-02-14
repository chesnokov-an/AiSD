#ifndef ERR_H
#define ERR_H

typedef enum err{
	ERR_OK = 0,
	ERR_EOF = -1,
	ERR_MEM = 1,
	ERR_VAL = 2,
	ERR_NULL = 3,
}err;

#endif
