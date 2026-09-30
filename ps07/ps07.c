#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_STOCK 1000000L
#define SALES_PER_CASHIER 100000L

static long wristbands_remaining = INITIAL_STOCK;
static int yield_between_read_and_write = 0;

static void *sell_wristbands(void *unused)
{
    (void)unused;

    for (long i = 0; i < SALES_PER_CASHIER; ++i) {
        long local = wristbands_remaining;
        --local;

        if (yield_between_read_and_write) {
            sched_yield();
        }

        wristbands_remaining = local;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <cashiers> <yield: 0|1>\n", argv[0]);
        return 1;
    }

    int cashiers = atoi(argv[1]);
    yield_between_read_and_write = atoi(argv[2]);

    if (cashiers < 1 || cashiers > 32 ||
        (yield_between_read_and_write != 0 &&
         yield_between_read_and_write != 1)) {
        fprintf(stderr, "Use 1–32 cashiers and yield 0 or 1.\n");
        return 1;
    }

    pthread_t *threads = calloc((size_t)cashiers, sizeof(*threads));
    if (threads == NULL) {
        perror("calloc");
        return 1;
    }

    for (int i = 0; i < cashiers; ++i) {
        int rc = pthread_create(&threads[i], NULL, sell_wristbands, NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            free(threads);
            return 1;
        }
    }

    for (int i = 0; i < cashiers; ++i) {
        int rc = pthread_join(threads[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(rc));
            free(threads);
            return 1;
        }
    }

    long expected = INITIAL_STOCK - (long)cashiers * SALES_PER_CASHIER;

    printf("Example: festival wristband inventory\n");
    printf("Cashiers: %d; sales per cashier: %ld\n",
           cashiers, SALES_PER_CASHIER);
    printf("Yield between read and write: %s\n",
           yield_between_read_and_write ? "yes" : "no");
    printf("Expected remaining if every sale is counted: %ld\n", expected);
    printf("Observed remaining: %ld\n", wristbands_remaining);

    free(threads);
    return 0;
}
