typedef struct {
  char id[2];
  void* payload;
  char* (*GetId)(struct tagXYObject*);
  void* (*GetPayload)(struct tagXYObject*);
} XYObject;
