unsigned long hash(char *s) {
    unsigned long result = 0;
    
    unsigned char ch;
    
    while ((ch = *s++)) {
    	result *= 31;
        result += (result<<1) + (result<<4) + (result<<7) + (result<<8) + (result<<24);

        result ^= ch;
    }
    
    return result;
}