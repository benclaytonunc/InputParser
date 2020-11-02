#include <string.h>

#include "Str.h"
#include "Vec.h"

static char NULL_CHAR = '\0';

Str Str_value(size_t capacity)
{
    Str s = Vec_value(capacity + 1, sizeof(char));
    // TODO: Replace the below lines with a call below to Vec_set
    // once you have Vec_set correctly implemented
    s.length = 1;
    char *buffer = (char*) s.buffer;
    buffer[0] = NULL_CHAR;
    Vec_set(&s, 0, &NULL_CHAR);
    return s;
}

void Str_drop(Str *self)
{
    Vec_drop(self);
}

size_t Str_length(const Str *self)
{
    return Vec_length(self) - 1;
}

const char* Str_cstr(const Str *self)
{
    return (char*) Vec_ref(self, 0);
}

char* Str_ref(const Str *self, const size_t index)
{
    return (char*) Vec_ref(self, index);
}
Str Str_from(const char *cstr) {
   //char* newStr = malloc(sizeof(cstr));
   //Str s = Str_value(sizeof(cstr));
   /*int i = 0;   
   while (cstr != '\0') {
       s.buffer += cstr[i];
       cstr++;
       i++;
   }
   */
   size_t b = strlen(cstr);
    Str s = Str_value(b+1);
   Vec_splice(&s, 0, 0, cstr, b);  
   //memcpy(Str_ref(s, 0), (const void *)cstr, sizeof(cstr));
   return s;
}

void Str_splice(Str *self, size_t index, size_t delete_count, const char *cstr, size_t insert_count) {   

    Vec_splice(self, index, delete_count, cstr, insert_count);          
    Vec_set(self, self->length, &NULL_CHAR);      
}
