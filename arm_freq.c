
#include <stdio.h>
#include <stdint.h>
#include <time.h>

static inline uint64_t getCycles(void)
{
    uint32_t r;
    asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(r)); /* Read PMCCNTR       */
    return ((uint64_t)r) << 6;                            /* 1 tick = 64 clocks */
}

static inline uint32_t getMillisecondCounter(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    return (uint32_t)(t.tv_sec * 1000 + t.tv_nsec / 1000000);
}

static inline int getClockSpeed(void)
{
    const uint64_t cycles = getCycles();
    const uint32_t millis = getMillisecondCounter();
    int lastResult = 0;

    for (;;)
    {
        int n = 1000000;
        while (--n > 0)
        {
        }

        const uint32_t millisElapsed = getMillisecondCounter() - millis;
        const uint64_t cyclesNow = getCycles();

        if (millisElapsed > 1000)
        {
            const int newResult = (int)(((cyclesNow - cycles) / millisElapsed) / 1000);

            if (millisElapsed > 5000 || (lastResult == newResult && newResult > 100))
                return newResult;

            lastResult = newResult;
        }
    }
}

int main()
{
    // PMCR.E (bit 0) = 1 | PMCR.C (bit 2) = 1 | PMCR.D (bit 3) = 1
    asm volatile("mcr p15, 0, %0, c9, c12, 0" ::"r"(1 | 1 << 2 | 1 << 3));

    // PMCNTENSET.C (bit 31) = 1
    asm volatile("mcr p15, 0, %0, c9, c12, 1" ::"r"(1 << 31));

    int clockSpeed = getClockSpeed();
    printf("ARM clock speed: %i MHz\n", clockSpeed);
    return 0;
}
