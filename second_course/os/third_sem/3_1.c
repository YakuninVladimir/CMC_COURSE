STYPE
bit_reverse(STYPE value)
{
    UTYPE marker = ~0;
    UTYPE ans = 0;
    UTYPE sourse = value;
    while (marker > 0) {
        ans <<= 1;
        ans ^= (sourse & 1);
        sourse >>= 1;
        marker >>= 1;
    }
    return (STYPE)ans;
}

