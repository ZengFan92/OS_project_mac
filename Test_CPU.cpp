#include <stdio.h>
#include <stdlib.h>

//#include <time.h>
#include <unistd.h>

#include <sys/types.h>
//#include <pthread.h>
#include <sys/syscall.h>

#define ITERATION 100
#define Cyc2Time 1
static __inline__ uint64_t rdtsc(void) {
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
}

// use assembly MACRO to read RDTSC to avoid procedure call overhead
#define read_start_time(hi,lo) \
asm volatile ("CPUID\n\t" \
"RDTSC\n\t" \
"mov %% edx, %0\n\t" \
"mov %% eax, %1\n\t": "=r" (hi), "=r" (lo):: "%rax","%rbx", "%rcx", "%rdx") 

#define read_end_time(hi,lo) \
asm volatile ("RDTSC\n\t" \
"mov %% edx, %0\n\t" \
"mov %% eax, %1\n\t" \
"CPUID\n\t": "=r" (hi), "=r" (lo):: "%rax","%rbx", "%rcx", "%rdx")

//1: Measurement overhead
//1-1: Overhead of reading time
static double ReadTimeOverhead() {
    uint64_t start;
	uint64_t end;
	//long double sum = 0;
    double sum = 0;
	for (int i = 0; i < 1000; ++i) {
        unsigned hi, lo,hi1, lo1;
        read_start_time(hi,lo);
        read_end_time(hi1,lo1);
        start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
        end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
        sum += end - start;
	}
	sum /= 1000.0;
	return sum / Cyc2Time;
}

//1-2: Overhead of using a loop to measure many iterations of an operation
static double LoopOverhead() {
	uint64_t start;
	uint64_t end;
	//uint64_t elapsed;
	//long double sum = 0;
    double sum = 0;
	for (int i = 0; i < 1000; i++) {
        unsigned hi, lo,hi1, lo1;
        read_start_time(hi,lo);
		for (int j = 0; j < 10000; j++) {
			//Empty loop, the only operation is inc(j)
		}
        read_end_time(hi1,lo1);
        start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
        end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
		sum += (end - start) / 10000.0;
	}
	sum /= 1000.0;
	return sum / Cyc2Time;
}

//2: Procedure call overhead
// use noinline attribute to prevent compiler making the function inline
static __attribute__((noinline)) void TestArgument_0 () {}
static __attribute__((noinline)) void TestArgument_1(int a) {}
static __attribute__((noinline)) void TestArgument_2(int a, int b) {}
static __attribute__((noinline)) void TestArgument_3(int a, int b, int c) {}
static __attribute__((noinline)) void TestArgument_4(int a, int b, int c, int d) {}
static __attribute__((noinline)) void TestArgument_5(int a, int b, int c, int d, int e) {}
static __attribute__((noinline)) void TestArgument_6(int a, int b, int c, int d, int e, int f) {}
static __attribute__((noinline)) void TestArgument_7(int a, int b, int c, int d, int e, int f, int g) {}
static double ProcedureOverhead(int num) {
	uint64_t start = 0;
	uint64_t end = 0;
	//uint64_t elapsed;
	//long double sum = 0;
    double sum = 0;
    switch (num) {
        case 0:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_0();
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 1:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_1(1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 2:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_2(1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 3:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_3(1,1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 4:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_4(1,1,1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 5:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_5(1,1,1,1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 6:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_6(1,1,1,1,1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        case 7:
            for (int i = 0; i < 1000; i++) {
                unsigned hi, lo,hi1, lo1;
                read_start_time(hi,lo);
                // function here
                TestArgument_7(1,1,1,1,1,1,1);
                // function ends
                read_end_time(hi1,lo1);
                start = ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
                end = ( (uint64_t)lo1)|( ((uint64_t)hi1)<<32 );
                sum += (end - start);
            }
            break;
        default:
            break;
    }
	
    
	sum /= 1000.0;
	return sum / Cyc2Time;
}

//3: System call overhead
static double SystemOverhead() {
	uint64_t start;
	uint64_t end;
	//long double sum = 0;
    double sum = 0;
	for (int i = 0; i < 1000; i++) {
		start = rdtsc();
		syscall(SYS_getpid);
		end = rdtsc();
		sum += end - start;
	}
	sum /= 1000.0;
	return sum/Cyc2Time;
}

//4: Task creation time
static double TaskCreationTime() {
	//uint64_t start, end, elapsed;
	double total_time = 0;

	for(int i = 0; i < ITERATION; i++){
		uint64_t start, end;
		start = rdtsc();
		pid_t pid = fork();
		end = rdtsc(); //parent's end

		if (pid == 0){ // child
			exit(0);
		}
		else{		//parent
			//wait(NULL);
			//end = rdtsc();
			total_time += (end - start)/Cyc2Time;
		}
	}
	return total_time / ITERATION;
}

//5: Context switch time
uint64_t OneSwitchTime(int *fd) {
    uint64_t start, end, elapsed = 0;
    pid_t pid = fork();

	if (pid == 0){ 	//child
		end = rdtsc();
        write(fd[1], (void*)&end, sizeof(end));
        exit(0);
	}
    else{		//parent
        // start = rdtsc();
        // wait(NULL);
        // read(fd[0], (void*)&end, sizeof(end));
	}
    if(end > start){
        elapsed = (end - start)/Cyc2Time;
    }
    return elapsed;
}

static double ProcessContextSwitchOverhead() {

	int fd[2];
    pipe(fd);
    uint64_t total_time = 0;
    int i = 0;
    while(i < ITERATION) {
        uint64_t elapsed = OneSwitchTime(fd);
        //printf("%llu\n", elapsed);
        if (elapsed > 0) {
            total_time += elapsed;
            i += 1;
        }
    }
    return (double)total_time/ (double)ITERATION;
}


static double PipeOverhead() {
	uint64_t total_time = 0;

	int i = 0;
	for (i = 0; i< ITERATION; i++){
		uint64_t  start, end, elapsed;
		int fd[2];
		pipe(fd);

		start = rdtsc();
		write(fd[1], &start, sizeof(start));
		read(fd[0], &start, sizeof(start));
		end = rdtsc();
		elapsed = (end - start)/Cyc2Time;
		total_time += elapsed;
	}
	return (double)total_time / ITERATION;
}


//Create kernel thread overhead
/*
void *Task() {
	pthread_exit(0);
}
static double KernelOverhead() {
	pthread_t thread;
	uint64_t start;
	uint64_t end;
	uint64_t elapsed;
	start = rdtsc();
	pthread_create(&thread, NULL, Task, NULL);
	end = rdtsc();
	elapsed = (end - start);
	return elapsed / Cyc2Time;
}

//Context switch kernel thread overhead

static uint64_t thread_end;
static uint64_t thread_start;
static uint64_t cs_time;
void *Thread2() {
	thread_end = rdtsc();
	pthread_exit(0);

}
void *Thread1() {
	pthread_t t2;
	pthread_create(&t2, NULL, &Thread2, NULL);
	thread_start = rdtsc();
	pthread_join(t2, NULL);
	pthread_exit(0);
}
static double CSKernelOverhead() {
	pthread_t t1;
	pthread_create(&t1, NULL, &Thread1, NULL);
	pthread_join(t1, NULL);
	cs_time = thread_end - thread_start;
	return cs_time / Cyc2Time;
}
*/
int main(int argc, const char * argv[]) {
	double overhead;

	//Read time overhead
	overhead = ReadTimeOverhead();
	printf("Read Time overhead = %lf nanoseconds\n", overhead);

	//Loop time overhead
	overhead = LoopOverhead();
	printf("Loop overhead = %lf nanoseconds\n", overhead);

	//Procedure call overhead
	for (int i = 0; i < 8; i++) {
		overhead = ProcedureOverhead(i);
		printf("Procedure call overhead with %d arguments = %lf nanoseconds\n", i, overhead);
	}
	//System call overhead
	overhead = SystemOverhead();
	printf("System call overhead = %lf nanoseconds\n", overhead);

	//Task creation time
	overhead = TaskCreationTime();
	printf("Process creation overhead = %lf nanoseconds\n", overhead);

	//Context switch time
	//printf("%lf\n", PipeOverhead());
	overhead = ProcessContextSwitchOverhead()-PipeOverhead();
	printf("Process context switch overhead = %lf nanoseconds\n", overhead);

	//kernel thread
	//overhead = KernelOverhead();
	//printf("kernel thread overhead = %lf nanoseconds\n", overhead);

	//kernel thread context switch time
	//overhead = CSKernelOverhead();
	//printf("kernel thread context switch overhead = %lf nanoseconds\n", overhead);
    //exit(0);
	//return 0;
}
