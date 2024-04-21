#include "types.h"
#include "param.h"
#include "defs.h"

// array based
// circular
// proc queue
struct queue {
	struct proc *arr[NPROC];
	int front, rear;
	int size;
	int time_quantum;	// allocated time quantum
	char name[10];	  // queue name (debugging)
	int level;	  	  // queue level
};

// all method require ptable.lock
int queue_isempty(struct queue *q);
int queue_isfull(struct queue *q);
int queue_push(struct queue *q, struct proc *p);
int queue_delete(struct queue *q, struct proc *p);
struct proc* queue_front(struct queue *q);
struct proc* queue_top(struct queue *q);
void queue_init(struct queue *q, int qlev, int time_quantum, char* name);
void queue_move(struct queue *from, struct queue *to, struct proc *p);
void queue_print(struct queue *q);
