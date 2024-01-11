char *serial_out = (char *)0x100;

void print(const char *str);

int main(void)
{
    print("TEST\n");
    return 0;
}

void print(const char *str) 
{
    char *current = (char*)str;
    while (*current++) {
        *serial_out = *current;
    }
}
