char *serial_out = (char *)0x100;

int main(void)
{
    *serial_out = 'T';
    *serial_out = 'E';
    *serial_out = 'S';
    *serial_out = 'T';
    *serial_out = '!';
    *serial_out = 0xFF;
    return 0;
}
