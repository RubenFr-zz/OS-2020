//
// Created by: Ruben Fratty, Shahar Bar
//

#include "Scheduler.h"

TASK
Build(char *line) {
    char cpy[strlen(line)];
    strcpy(cpy, line);

    char *param = strtok(cpy, ",\n");

    int id = atoi(param);
    param = strtok(NULL, ",\n");
    int priority = atoi(param);
    param = strtok(NULL, ",\n");
    int arrival_time = atoi(param);
    param = strtok(NULL, ",\n");
    int burst_time = atoi(param);

    TASK t = {.id = id, .priority = priority, .arrival_time = arrival_time, .burst_time = burst_time, .executed = false};
    return t;
}

TASK *
Table(char *file) {
    FILE *fd;

    if ((fd = fopen(file, "r")) == NULL) {
        fprintf(stderr, "ERROR in opening %s", file);
        exit(1);
    }

//    TASK *tasks[MAX];
//    TASK tasks[MAX];
//    for(int i = 0; i < MAX; ++i)
//        tasks[i] = malloc(sizeof(TASK));
    TASK *tasks = malloc(MAX * sizeof *tasks);


    char buffer[BUFFER_MAX];
    int i;
    for (i = 0; fgets(buffer, BUFFER_MAX, fd) && i < MAX; ++i) {
        TASK t = Build(buffer);
        tasks[i].id = t.id;
        tasks[i].priority = t.priority;
        tasks[i].arrival_time = t.arrival_time;
        tasks[i].burst_time = t.burst_time;
    }
    if (i < MAX) tasks[i].id = -1;  // Indicate when to stop
    return tasks;
}

void
Display(TASK *tasks) {
//    printf("===============PROCESS===TABLE===================\n");
    printf("\n   ID   | Priority |  Arrival Time |  CPU BURST  \n");
    printf("--------+----------+---------------+-------------\n");
    for (int i = 0; i < 10 && tasks[i].id != -1; ++i)
        printf(" %3d    | %5d    | %7d       | %6d    \n",
               tasks[i].id, tasks[i].priority, tasks[i].arrival_time, tasks[i].burst_time);
    printf("\n");
}

void
schedule(TASK *tasks, enum ALGO algo, int Q) {
    TASK *copy = duplicate(tasks);
    int size = len(tasks);
    switch (algo) {
        case FCFS:
            process_FCFS(copy, size);
            break;
        case SJF:
            process_SJF(copy, size);
            break;
        case PRIORITY:
            process_PRIORITY(copy, size);
            break;
        case ROUND_ROBIN:
            process_RR(copy, size, Q, false);   // Without priority
            break;
        case P_ROUND_ROBIN:
            process_RR(copy, size, Q, true);    // With priority
            break;
        default:
            fprintf(stderr, "Wrong Algorithm");
    }
}

void
process_FCFS(TASK *tasks, int size) {
    printf("Scheduling Tasks - FCFS:\n");
    for (int i = 0; i < size; ++i) get_first(tasks, size);      // Non-Preemptive
    printf("\n");
}

void
process_SJF(TASK *tasks, int size) {
    printf("Scheduling Tasks - SJF:\n");
    int clock = 0;
    for (int i = 0; i < size; ++i) clock = get_shortest(tasks, size, clock);    // Non-Preemptive
    printf("\n");
}

void
process_PRIORITY(TASK *tasks, int size) {
    printf("Scheduling Tasks - PRIORITY:\n");
    int clock = 0;
    for (int i = 0; i < size; ++i) clock = get_priority(tasks, size, clock, false);    // Non-Preemptive
    printf("\n");
}


void
process_RR(TASK *tasks, int size, int Q, int Priority) {
    if (Priority) printf("Scheduling Tasks - PRIORITY ROUND ROBIN with Q=%d:\n", Q);
    else printf("Scheduling Tasks - ROUND ROBIN with Q=%d:\n", Q);

    struct Queue *queue = createQueue(MAX);
    int clock = 0;
    bool CPU_busy = false;
    int busy_time = 0;
    int inProcess = -1;

    while (!isEmpty(queue) || CPU_busy || size > 0) {
        for (int i = 0; i < MAX && tasks[i].id != -1; ++i) {
            if (tasks[i].executed || (tasks[i].arrival_time > clock))   // Already queued or not arrived
                continue;
            enqueue(queue, i);
            size--;
            tasks[i].executed = true;
        }
        if (CPU_busy) {
            if (busy_time > 0) {
                clock++;
                busy_time--;
                continue;
            }
            CPU_busy = false;
            if (tasks[inProcess].burst_time > 0)
                enqueue(queue, inProcess);
        }
        if (isEmpty(queue)) clock++;    // No tasks has arrived and CPU available
        else {
            CPU_busy = true;
            inProcess = Priority ? priority_dequeue(queue, tasks) : dequeue(queue);
            busy_time = (tasks[inProcess].burst_time - Q > 0) ? Q : tasks[inProcess].burst_time;
            tasks[inProcess].burst_time -= busy_time;
            printf("<P%d,%d>", tasks[inProcess].id, busy_time);
            clock++;
            busy_time--;
        }
    }
    printf("\n");
}

TASK *
duplicate(TASK *toCopy) {
    TASK *copy = malloc(MAX * sizeof *toCopy);

    int i;
    for (i = 0; toCopy[i].id != -1 && i < MAX; ++i) {
        copy[i].id = toCopy[i].id;
        copy[i].priority = toCopy[i].priority;
        copy[i].arrival_time = toCopy[i].arrival_time;
        copy[i].burst_time = toCopy[i].burst_time;
    }
    if (i < MAX) copy[i].id = -1;   // To know when to stop reading the next task
    return copy;
}

int
len(TASK *tasks) {
    int i = 0;
    while (tasks[i].id != -1) i++;
    return i;
}

void
get_first(TASK *tasks, int size) {
    int i, first = -1;
    int _min = INT_MAX;
    for (i = 0; i < size; ++i) {
        if (tasks[i].executed)
            continue;
        first = (_min > tasks[i].arrival_time) ? i : first;
        _min = min(_min, tasks[i].arrival_time);
    }
    tasks[first].executed = true;
    printf("<P%d,%d>", tasks[first].id, tasks[first].burst_time);
}

int
get_shortest(TASK *tasks, int size, int clock) {
    int i, shortest = -1;
    int _min = INT_MAX;
    for (i = 0; i < size; ++i) {
        if (tasks[i].executed || (tasks[i].arrival_time > clock))
            continue;
        shortest = (_min > tasks[i].burst_time) ? i : shortest;
        _min = min(_min, tasks[i].burst_time);
    }
    tasks[shortest].executed = true;
    printf("<P%d,%d>", tasks[shortest].id, tasks[shortest].burst_time);
    return clock + tasks[shortest].burst_time;
}

int
get_priority(TASK *tasks, int size, int clock, bool RR) {
    int i, highest = -1;
    int _max = INT_MIN;
    for (i = 0; i < size; ++i) {
        if (tasks[i].executed || (tasks[i].arrival_time > clock))
            continue;
        highest = (_max < tasks[i].priority) ? i : highest;
        _max = max(_max, tasks[i].priority);
    }
    if (RR) return highest;
    tasks[highest].executed = true;
    printf("<P%d,%d>", tasks[highest].id, tasks[highest].burst_time);
    return clock + tasks[highest].burst_time;
}

int main() {
    TASK *t1 = Table("../input.txt");
    Display(t1);
    schedule(t1, FCFS, -1);
    schedule(t1, SJF, -1);
    schedule(t1, PRIORITY, -1);
    schedule(t1, ROUND_ROBIN, 2);
    schedule(t1, ROUND_ROBIN, 3);
    schedule(t1, P_ROUND_ROBIN, 2);
    schedule(t1, P_ROUND_ROBIN, 3);
    Display(t1);
    return 0;
}

//// QUEUE IMPLEMENTATION ////
struct Queue {
    int front, rear, size;
    int capacity;
    int *array;
};

struct Queue *
createQueue(int capacity) {
    struct Queue *queue = (struct Queue *) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    queue->array = (int *) malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity
int
isFull(struct Queue *queue) {
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int
isEmpty(struct Queue *queue) {
    return (queue->size == 0);
}

// Function to add an item to the queue. It changes rear and size
void
enqueue(struct Queue *queue, int item) {
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove the front from queue. It changes front and size
int
dequeue(struct Queue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "The Queue is Empty");
        exit(1);
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to remove the item with the highest priority from queue. It changes front and size
int
priority_dequeue(struct Queue *queue, TASK *tasks)
{
    int highest = 0;
    int index_highest = 0;
    int highest_id= -1;
    // FINDING THE HIGHEST PRIORITY TASK IN THE QUEUE
    for (int i = queue->front ; i != (queue->rear + 1) % queue->capacity; i = (i + 1) % queue->capacity){
        if (tasks[queue->array[i]].priority > highest){
            highest = tasks[queue->array[i]].priority;
            highest_id = queue->array[i];
            index_highest = i;
        }
    }
    // REARRANGING THE QUEUE SO THE HIGHEST IS DEQUEUED
    for(int i = mod(index_highest-1, MAX); i != mod(queue->front-1, MAX); i = mod(i-1, MAX))
        queue->array[(i+1) % MAX] = queue->array[i];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return highest_id;
}

// Function to get front of queue
int
front(struct Queue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "The Queue is Empty");
        exit(1);
    }
    return queue->array[queue->front];
}

// Function to get rear of queue
int
rear(struct Queue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "The Queue is Empty");
        exit(1);
    }
    return queue->array[queue->rear];
}
//// END QUEUE IMPLEMENTATION ////

int
min(int a, int b)
{
    return a < b ? a : b;
}

int
max(int a, int b)
{
    return a > b ? a : b;
}

int
mod(int a, int b)
{
    if(b < 0) {
        fprintf(stderr, "b cannot be negative");
        exit(1);
    }
    int r = a % b;
    return r < 0 ? r + b : r;
}