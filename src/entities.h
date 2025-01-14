#ifndef ENTITIES_H
#define ENTITIES_H

#include "utils.h"

void frog(Buffer buffer, Item item);
void moveFrog(Buffer* buffer, Item item, int direction);
void crocodile(Buffer buffer, Item item);
void bullet(Buffer buffer, Item item);
void timer(Buffer buffer, Item item);

#endif // #ENTITIES_H