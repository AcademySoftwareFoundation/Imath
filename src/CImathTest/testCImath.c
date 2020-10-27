#define TEST(x)                                                                                    \
    if (argc < 2 || !strcmp (argv[1], #x))                                                         \
        x();

int
main (int argc, char* argv[])
{
    return 0;
}
