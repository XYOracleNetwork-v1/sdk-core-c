typedef struct {
  char id[2];
  char* (*Hash)(char*);
  int (*VerifyHash)(char*, char*);
  char* (*GetId)();
} HashProvider;
