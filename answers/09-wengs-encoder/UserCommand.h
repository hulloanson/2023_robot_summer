#ifndef UserCommand_h
#define UserCommand_h

struct UserCommand
{
  short userId;
  char command;
  int p1;
  int p2;
  unsigned long ts;
};

#endif