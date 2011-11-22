extern (C) void Kernel(uint pMultiBootMagic,
                       void * pMultiBootData)
{
    char * ptr = cast(char*)0x000B8000;
    *ptr = '@';
    while(true) { }
}
