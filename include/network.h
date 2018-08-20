#include <stddef.h>

typedef struct {
  /*
   * Protocol specific implementation of our proactive network concept.
   * This should begin a scan, connect directly to a peer, or do what
   * is needed in order to begin a connection given just a string.
   * This should return a unique identifier for that connection so
   * that the unique identifier can be referenced later in SendData.
   */
  int (*RequestConnection)(char*);
  char* (*SendData)(char*, int); // Send a given string to a given connection.
  void (*Disconnect)(int); // disconnect from a given connection.
} proactiveNetworkProvider;

typedef struct {
  /*
    * Again here Listen assumes many steps. The general idea being,
    * do what is necessary in order to set up a listener on a given
    * network type. When someone connects to the server, it should
    * call the callback specified by the void* argument with the data
    * that the client sent. Returns a unique identifier for that connection.
    */
  int (*Listen)(void*);
  int (*Disconnect)(int); // Disconnect from a given connection.
} reactiveNetworkProvider;
