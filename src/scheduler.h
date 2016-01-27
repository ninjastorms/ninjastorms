typedef struct task {
	unsigned int reg[16];
	unsigned int cpsr;
} task_t;

extern task_t *current_task;
extern task_t *other_task;

void init_task(task_t *task, unsigned int entrypoint, unsigned int stackbase);
void start_scheduler(task_t *tasks[]);
void schedule(void);
