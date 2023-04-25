#include <stdio.h>
#include <stdlib.h>
#include<math.h>

double eps1 = 0.000001, eps2 = 0.0000001;

double f1 (double x){
    return 1.0 + 4.0 / (x * x + 1.0);
}

double f2 (double x){
    return  x * x * x;
}

double f3 (double x){
    return pow(2, -x);
}

double f1_der(double x){
    return -8.0 * x / (pow(x, 4) + 2 * x * x + 1.0);
}

double f2_der(double x){
    return 3 * x * x;
}

double f3_der(double x){
    return -1 * pow(2, -x) * log(2);
}


double root ( double (*f)(double), double (*f_der)(double), double (*g)(double), double (*g_der)(double), double a, double b, double eps1){
  
    double delta1 = (f(b) - g(b)) - (f(a) - g(a));
    double delta2 = (f(a) - g(a) + f(b) - g(b))/2 - f((a + b)/2) - g((a + b)/2);

    if (delta1 * delta2 > 0){
        double c = b;
        while ((f(c) - g(c)) * (f(c - eps1) - g(c - eps1)) > 0) {
            c = c - (f(c) -g(c))/(f_der(c) - g_der(c));
        }
        return c;
    } 
    else{
        double c = a;
   
        while ((f(c) - g(c)) * (f(c + eps1) - g(c + eps1)) > 0) {
            c = c - (f(c) -g(c))/(f_der(c) - g_der(c));
        }
        return c;
    }
}

double integral ( double (*f)(double), double a, double b, double eps2){
    int n = 10;
    double * list1;
    double * list2;
    double I_n, I_2n;
    list1 = calloc(n + 1, 8);
    for (int i = 0; i <= n; i++){
        list1[i] = f(a + (b - a) * i / n);
    }
    I_n = list1[0];
    I_n += list1[n];
    for (int i = 1; i < n; i++){
        I_n += i % 2 == 0 ? list1[i] * 2 : list1[i] * 4;
    }

    I_n *= (b - a)/(3.0 * n);
    I_2n = I_n;
    do
    {
        I_n = I_2n;
        n *= 2;
        list2 = calloc(n + 1, 8);
        for (int i = 0; i < n; i++){
            list2[i] = i % 2 == 0 ? list1[i/2] : f(a + (b - a) * i / n);
        }
        I_2n = list2[0];
        I_2n += list2[n];
        for (int i = 1; i < n; i++){
            I_2n += i % 2 == 0 ? list2[i] * 2 : list2[i] * 4;
        }
        free(list1);
        list1 = list2;
        I_2n *= (b - a)/(3.0 * n);
        //printf("%lf %lf\n", I_n, I_2n);
    } while(fabs(I_n - I_2n)/15.0 >= eps2);
    return I_2n;
}

void print_roots(void){
    double root1 = root(f1, f1_der, f3, f3_der, -1.4, -0.7, eps1);
    double root2 = root(f1, f1_der, f2, f2_der, 0.5, 2, eps1);
    double root3 = root(f3, f3_der, f2, f2_der, 0.5, 1.5, eps1);
    printf("root f1, f3: %lf\nroot f1, f2: %lf\nroot f2, f3: %lf\n", root1, root2, root3);
}

void print_integrals(void){
    double root1 = root(f1, f1_der, f3, f3_der, -1.4, -0.7, eps1);
    double root2 = root(f1, f1_der, f2, f2_der, 0.5, 2, eps1);
    double root3 = root(f3, f3_der, f2, f2_der, 0.5, 1.5, eps1);
    double I_f1 = integral(f1, root1, root2, eps2);
    double I_f2 = integral(f3, root3, root2, eps2);
    double I_f3 = integral(f3 ,root1, root3, eps2);
    printf("integral f1: %lf\nintegral f2: %lf\nintegral f3: %lf\n", I_f1, I_f2, I_f3);
    double ans = I_f1 - I_f2 - I_f3;
    printf("answer: %lf\n", ans);
}

int main (void) {
    print_roots();
    print_integrals(); 
    return 0;
}