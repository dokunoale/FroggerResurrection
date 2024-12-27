/**
 * @file struct.h
 * @brief This file contains structures used globally throughout the program.
 */
#ifndef STRUCT_H
#define STRUCT_H

/**
 * @brief The structure for an item.
 * @note Defined in struct.h  
 */
typedef struct {
    unsigned int line;
    int column;
    int type;
} Item;

#endif // STRUCT_H