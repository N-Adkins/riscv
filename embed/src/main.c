void print(const char *text);

char *serial = (char *)0x1000000;

int main(void)
{
    print("Test\n");
    return 0;
}

void print(const char *text)
{
    char *current = (char*)text;
    while (*current != '\0') {
        *serial = *current;
        current++; 
    }
}
