typedef struct{
  char* (*Hash)(char*);
  int (*VerifyHash)(char*, char*);
  char* (*GetId)();
} HashProvider;
