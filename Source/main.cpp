#include <cstdio>
#include <cstdlib>

#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)

#define CURRENT_DAY Day12    //Cos I'm lazy.

#include STR(Days/CURRENT_DAY.h)

int main(int argc, char** argv)
{
    printf("Running " STR(CURRENT_DAY) "...\n");
    CURRENT_DAY day;
    day.Run();

    printf("\nDone!\n");

    system("pause");
}
