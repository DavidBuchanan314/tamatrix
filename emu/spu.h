#ifndef SPU_H
#define SPU_H

#define QUEUE_LEN 8192

unsigned char value_q[QUEUE_LEN];
unsigned int offset_q[QUEUE_LEN];
unsigned int q_idx;
unsigned int cpu_offset;
unsigned char out_state;
int cpudiv;

int spuInit();
void q_append(char val);

#endif
