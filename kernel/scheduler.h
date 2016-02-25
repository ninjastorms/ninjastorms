typedef struct task {
	unsigned int reg[16];
	unsigned int cpsr;
} task_t;

extern task_t *current_task;

void add_task(void* entrypoint);
void start_scheduler(void);
void schedule(void);
