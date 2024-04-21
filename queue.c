#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "queue.h"

int queue_isempty(struct queue *q)
{
	return (q->size == 0);
}

int queue_isfull(struct queue *q)
{
	return (q->size == NPROC);
}

int queue_push(struct queue *q, struct proc *p)
{
	if(queue_isfull(q))
		return -1;
	p->qlev = q->level;
	q->arr[q->rear] = p;
	q->rear = (q->rear + 1) % NPROC;
	q->size += 1;
	return 0;
}

int queue_delete(struct queue *q, struct proc *p)
{
	int i;
	for(i = q->front; i != q->rear; i = (i + 1) % NPROC) {
		if(q->arr[i]->pid == p->pid) {
			goto delete_ele_found;
		}
	}
	return -1;

delete_ele_found:
	for(; i != q->rear; i = (i + 1) % NPROC) {
		q->arr[i] = q->arr[(i + 1) % NPROC];
	}
	q->rear = (q->rear - 1 + NPROC) % NPROC;
	q->size -= 1;
	return 1;
}

struct proc* queue_front(struct queue *q)
{
	if(queue_isempty(q))
		return 0;
	return q->arr[q->front];
}

struct proc* queue_top(struct queue *q)
{
	int i;
	struct proc *p = 0;
	if(queue_isempty(q))
		return 0;
	for(i = q->front; i != q->rear; i = (i + 1) % NPROC) {
		if(q->arr[i]->state != RUNNABLE)
			continue;
		if(p == 0 || p->priority < q->arr[i]->priority)
			p = q->arr[i];
	}
	// cprintf("pid: [%d], chan: [%x], state: [%d]\n", q->arr[q->front]->pid, q->arr[q->front]->chan, q->arr[q->front]->state);
	return p;
}

void queue_init(struct queue *q, int qlev, int time_quantum, char* name)
{
	q->level = qlev;
	q->time_quantum = time_quantum;
	q->front = q->rear = q->size = 0;
	memset(q->arr, 0, sizeof(q->arr));
	safestrcpy(q->name, name, sizeof(q->name));
}

int queue_size(struct queue *q, enum procstate state)
{
	int i, size = 0;
	for(i = q->front; i != q->rear; i = (i + 1) % NPROC) {
		if(q->arr[i]->state == state)
			size++;
	}
	return size;
}

void queue_move(struct queue *from, struct queue *to, struct proc *p)
{
	if(queue_delete(from, p) < 0)
		panic("queue_move: delete failed");
	if(queue_push(to, p) < 0)
		panic("queue_move: push failed");

	if(DEBUG_MODE)
		cprintf("queue_move: [%s(%d)] %d (%d -> %d)\n", p->name, p->pid, p->state, from->level, to->level);
}

void queue_print(struct queue *q)
{
	int i;
	cprintf("Queue %s (size: %d): ", q->name, q->size);
	for(i = q->front; i != q->rear; i = (i + 1) % NPROC)
		cprintf("[%d(%s)](%d) ", q->arr[i]->pid, q->arr[i]->name, q->arr[i]->state);
	cprintf("\n");
}
