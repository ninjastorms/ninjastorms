typedef struct task {
	unsigned int reg[16];
	unsigned int cpsr;
} task_t;

void init_task(task_t *task, unsigned int entrypoint, unsigned int stackbase);
void start_scheduler(void);
void schedule();
