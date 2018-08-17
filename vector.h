/**
 * @Author: Nate Brune
 * @Date:   10-Aug-2018
 * @Email:  nate.brune@xyo.network
 * @Project: XYO-C-SDK
 * @Filename: vector.h
 * @Last modified by:   Nate Brune
 * @Last modified time: 10-Aug-2018
 * @Copyright: XY - The Findables Company
 */
 // vector.h

 #define VECTOR_INITIAL_CAPACITY 2

 // Define a vector type
 typedef struct {
   int size;      // slots used so far
   int capacity;  // total available slots
   char *data;     // array of bytes we're storing
 } Vector;

 void vector_init(Vector *vector);

 void vector_append(Vector *vector, int value);

 int vector_get(Vector *vector, int index);

 void vector_set(Vector *vector, int index, int value);

 void vector_double_capacity_if_full(Vector *vector);

 void vector_free(Vector *vector);
