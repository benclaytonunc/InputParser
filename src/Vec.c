#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Guards.h"

#include "Vec.h"

/* Constructor / Destructor */

Vec Vec_value(size_t capacity, size_t item_size)
{
    Vec vec = {
        item_size,
        0,
        capacity,
        calloc(capacity, item_size)
    };
    OOM_GUARD(vec.buffer, __FILE__, __LINE__);
    return vec;
}

void Vec_drop(Vec *self)
{
    free(self->buffer);
    self->buffer = NULL;
    self->capacity = 0;
    self->length = 0;
}

/* Accessors */

size_t Vec_length(const Vec *self)
{
    return self->length;
}

void* Vec_ref(const Vec *self, size_t index)
{
    if (index < self->length) {
        return self->buffer + (index * self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
}

void Vec_get(const Vec *self, size_t index, void *out) {
    if ((index < self->length) && index >= 0) {
        //self->buffer[index] = out;
        memcpy(out, Vec_ref(self, index), self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }   
}
void Vec_set(Vec *self, size_t index, const void *value) {
    if (index < self->length && index >= 0) {
        memcpy(Vec_ref(self,index),value, self->item_size);
    } else {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    } 
}

bool Vec_equals(const Vec *self, const Vec *other) {
    if(other->length == self->length) {
        size_t a = self->length;
        a *= self->item_size;
        char * b = (char *) self->buffer;
        char * c = (char *) other->buffer;
        for (size_t i = 0; i < a; i++) {

            if (b[i] != c[i]) {
                return false;    
            }
        }
        return true;
    }
    return false;

}

void Vec_splice(Vec *self, size_t index, size_t delete_count, const void *items, size_t insert_count) {
    size_t ogLength = self->length;
    if (index >= ogLength || index < 0) {
        fprintf(stderr, "%s:%d - Out of Bounds", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    size_t endLength = self->length + insert_count - delete_count;
    char * temp = (char *) self->buffer;

    if (delete_count > 0) {
        for (size_t i = index; i < (index + delete_count); i++) {
            char * empty = NULL;
            Vec_set(self, i, empty);
        }
    }

    self = realloc(self, endLength * self->item_size);
    if(insert_count > 0) {
        int j = 0;
        for (size_t i = index; i < (index + insert_count); i++) {
            Vec_set(self, i, &items[j]);
            j++;
        }
    }
    int k = index + delete_count;
    for (size_t i = index+insert_count; i < endLength; i++) {
        Vec_set(self, i, &temp[k]);
        k++;
    }
}
