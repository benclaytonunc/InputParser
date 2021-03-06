#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/*
 * Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */


TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
       Vec_ref(&v, 3);
    }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, getSingle) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    v.length = 1;
    int16_t a = 0;
    Vec_get(&v,0,&a);
    ASSERT_EQ(buffer[0], a);
}

TEST(VecImpl, getMultiple) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    v.length = 3;
    int16_t a = 0;
    Vec_get(&v,0,&a);
    int16_t b = 0;
    Vec_get(&v, 1, &b);
    ASSERT_EQ(buffer[0], a);
    ASSERT_EQ(buffer[1], b);
    int16_t c = 0;
    Vec_get(&v, 2, &c);
    ASSERT_EQ(buffer[2], c);
}


TEST(VecImpl, getOutofBoundsNegative) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    v.length = 3;
    int16_t b = 0;
    
     ASSERT_DEATH({
       Vec_get(&v, -1, &b);
    }, ".* - Out of Bounds");
        
}


TEST(VecImpl, getOutofBoundsOver) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    v.length = 3;
    int16_t b = 0;
    
     ASSERT_DEATH({
       Vec_get(&v, 5, &b);
    }, ".* - Out of Bounds");
        
}

TEST(VecImpl, ArraySet) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 15;
    buffer[1] = 7;
    buffer[2] = 2;
    v.length = 3;
    int16_t b = 0;
    Vec_set(&v, 0, &b);
    ASSERT_EQ(buffer[0], b);
} 

TEST(VecImpl, SingleSet) {
    Vec v = Vec_value(1, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    int16_t x = 1;
    Vec_set(&v, 0, &x);
    ASSERT_EQ(1, v.length);
    int16_t y = 2;
    Vec_set(&v, 1, &y);
    ASSERT_EQ(2, v.length);
    
    int16_t *buffer = (int16_t*)v.buffer;
    ASSERT_EQ(x, buffer[0]);
    ASSERT_EQ(y, buffer[1]);
    Vec_drop(&v);
}


TEST(VecImpl, setOverBounds) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 15;
    buffer[1] = 7;
    buffer[2] = 2;
    v.length = 3;
    int16_t b = 0;
       ASSERT_DEATH({
          Vec_set(&v, 5, &b);
       }, ".* - Out of Bounds");
        
} 

TEST(VecImpl, SetUnderBounds) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 15;
    buffer[1] = 7;
    buffer[2] = 2;
    v.length = 3;
    int16_t b = 0;
       ASSERT_DEATH({
          Vec_set(&v, -1, &b);
       }, ".* - Out of Bounds");
        
} 


TEST(VecImpl, equalsMultiple) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    
    v.length = 2;
    
    Vec w = Vec_value(3, sizeof(int16_t));
    int16_t *buff2 = (int16_t*) w.buffer;
    buff2[0] = 10;
    buff2[1] = 20;
    
    w.length = 2;
    ASSERT_EQ(Vec_equals(&v, &w), true);
}

TEST(VecImpl, equalsSingle) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10; 
    v.length = 1;
    
    Vec w = Vec_value(3, sizeof(int16_t));
    int16_t *buff2 = (int16_t*) w.buffer;
    buff2[0] = 10;
    
    w.length = 1;
    ASSERT_EQ(Vec_equals(&v, &w), true);
} 

TEST(VecImpl, equalsDiffLengths) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    v.length = 3;
    
    Vec w = Vec_value(3, sizeof(int16_t));
    int16_t *buff2 = (int16_t*) w.buffer;
    buff2[0] = 10;
    buff2[1] = 20;
    
    w.length = 2;
    ASSERT_EQ(Vec_equals(&v, &w), false);
}

TEST(VecImpl, equalsDiffValues) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 30;
    buffer[1] = 20;
    
    v.length = 2;
    
    Vec w = Vec_value(3, sizeof(int16_t));
    int16_t *buff2 = (int16_t*) w.buffer;
    buff2[0] = 10;
    buff2[1] = 20;
    
    w.length = 2;
    ASSERT_EQ(Vec_equals(&v, &w), false);
}

TEST(VecImpl, vec_splice_switch) {

    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 0, 0, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 800;
    result[1] = 900;
    result[2] = 100;
    result[3] = 200;
    result[4] = 300; 
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
    ASSERT_EQ(bufferAgain[3], result[3]);
    ASSERT_EQ(bufferAgain[4], result[4]);
}



TEST(VecImpl, vec_splice_append) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 3, 0, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 100;
    result[1] = 200;
    result[2] = 300;
    result[3] = 800;
    result[4] = 900; 
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
    ASSERT_EQ(bufferAgain[3], result[3]);
    ASSERT_EQ(bufferAgain[4], result[4]);
}

TEST(VecImpl, vec_splice_insertInMiddle) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 2, 0, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 100;
    result[1] = 200;
    result[2] = 800;
    result[3] = 900;
    result[4] = 300; 
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
    ASSERT_EQ(bufferAgain[3], result[3]);
    ASSERT_EQ(bufferAgain[4], result[4]);
}

TEST(VecImpl, vec_splice_replace_eqLen) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
     
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 0, 2, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 800;
    result[1] = 900;
    result[2] = 300;
    
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
    ASSERT_EQ(bufferAgain[3], result[3]);
    ASSERT_EQ(bufferAgain[4], result[4]);
}

TEST(VecImpl, vec_splice_replace_longerLen) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
     
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 0, 3, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 800;
    result[1] = 900; 
    
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    
}

TEST(VecImpl, vec_splice_replace_longerIns) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
     
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 1, 2, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 100;
    result[1] = 800;
    result[2] = 900;
    
    
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
    ASSERT_EQ(bufferAgain[3], result[3]);
    ASSERT_EQ(bufferAgain[4], result[4]);
}

TEST(VecImpl, splicewDelete) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    
    v.length = 3;
    int16_t items[] = {800,900};
    Vec_splice(&v, 1, 2, items, 2);
    int16_t *bufferAgain = (int16_t*) v.buffer;
    Vec fin = Vec_value(5, sizeof(int16_t));
    int16_t *result = (int16_t*)fin.buffer;
    result[0] = 100;
    result[1] = 800;
    result[2] = 900;
       
    ASSERT_EQ(bufferAgain[0], result[0]);
    ASSERT_EQ(bufferAgain[1], result[1]);
    ASSERT_EQ(bufferAgain[2], result[2]);
  }

