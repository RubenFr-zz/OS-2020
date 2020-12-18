//
// Created by rubenf on 18/12/2020.
//

#ifndef SCHEDULER_SCHEDULER_H
#define SCHEDULER_SCHEDULER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT_MAX 2147483647
#define INT_MIN (-2147483648)
#define MAX 10
#define BUFFER_MAX 255

typedef struct {
    int id;             // Unique ID of the Task
    int priority;       // Priority level
    int arrival_time;
    int burst_time;
    bool executed;      // If the task has been executed
} TASK;

enum ALGO {
    FCFS = 1,    // First Come First Serve - Non-Preemptive
    SJF = 2,    // Shortest Job First - Non-Preemptive
    PRIORITY = 3,    // Non-Preemptive
    ROUND_ROBIN = 4,    // Non-Preemptive
    P_ROUND_ROBIN = 5     // Priority Round Robin - Non-Preemptive
};

// QUEUE
struct Queue *createQueue(int capacity);

int isFull(struct Queue *queue);

int isEmpty(struct Queue *queue);

void enqueue(struct Queue *queue, int item);

int dequeue(struct Queue *queue);

int priority_dequeue(struct Queue *queue, TASK *tasks);

int front(struct Queue *queue);

int rear(struct Queue *queue);
// END QUEUE

TASK Build(char *line);

TASK *Table(char *file);

void Display(TASK *tasks);

void schedule(TASK *tasks, enum ALGO algo, int Q);

void process_FCFS(TASK *tasks, int size);

void process_SJF(TASK *tasks, int size);

void process_PRIORITY(TASK *tasks, int size);

void process_RR(TASK *tasks, int size, int Q, int Priority);

void get_first(TASK *tasks, int size);

int get_shortest(TASK *tasks, int size, int clock);

int get_priority(TASK *tasks, int size, int clock, bool RR);

TASK *duplicate(TASK *toCopy);

int len(TASK *tasks);

int min(int a, int b);

int max(int a, int b);

int mod(int a, int b);

#endif //SCHEDULER_SCHEDULER_H
