
#ifndef SENSOR_H
#define SENSOR_H

int openTemp(int);
int readTemp(long *, int *);
void closeTemp();
int setReadStart(int);

#endif
