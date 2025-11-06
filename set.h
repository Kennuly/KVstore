#ifndef __SET_H__
#define __SET_H__

#define ERROR_CHECK(ret, error, val) \
    do                               \
    {                                \
        if (ret == -1)               \
        {                            \
            perror(error);         \
            return val;              \
        }                            \
    } while (0);

#endif