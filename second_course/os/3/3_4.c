int
satsum(int v1, int v2)
{
    enum
    {
        ONE = -(~0),
        MY_INT_MAX = (signed) (((unsigned) -ONE) >> ONE),
        MY_INT_MIN = (signed) ((unsigned) MY_INT_MAX + ONE)
    };
    if (v1 > 0 && v2 > 0 && v1 > MY_INT_MAX - v2) {
        return MY_INT_MAX;
    } else if (v1 < 0 && v2 < 0 && v1 < MY_INT_MIN - v2) {
        return MY_INT_MIN;
    } else {
        return v1 + v2;
    }
}
