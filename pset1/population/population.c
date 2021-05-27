#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // variables initialization
    int population_start, population_end, population, grow_speed, end = 0;

    // TODO: Prompt for start size
    do
    {
        population_start = get_int("Start size: \n");
    }
    while (population_start < 9);

    // TODO: Prompt for end size
    do
    {
        population_end = get_int("End size: \n");
    }
    while (population_end < population_start);

    // TODO: Calculate number of years until we reach threshold
    population = population_start;
    grow_speed = population_start / 3 - population_start / 4;

    while (population < population_end)
    {
        population = population + grow_speed;
        grow_speed = population / 3 - population / 4;
        end++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", end);
}