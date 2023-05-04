#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


double eps1 = 0.0001, eps2 = 0.00001; //точность вычисления корней и интегралов

int num_of_iterations = 0; //число итераций при вычислении корня 

//прототипы ассемблерных функций
double f1 (double x);

double f2 (double x);

double f3 (double x);

double f1_der(double);

double f2_der(double x);

double f3_der(double x);

//простенькие функции для тестирования программы
double f4(double x){
    return 1;
}

double f5(double x){
    return x;
}

double f6(double x){
    return 4 - x*x;
}

double f4_der(double x){
    return 0;
}

double f5_der(double x){
    return 1;
}

double f6_der(double x){
    return -2 * x;
}
//массивы функций (чтобы функцию по номеру вызывать)
double (*functions[6])(double) = {&f1, &f2, &f3, &f4, &f5, &f6};
double (*derivs[6])(double) = {&f1_der, &f2_der, &f3_der, &f4_der, &f5_der, &f6_der};

//вычисление пересечения (корня функции  f - g) методом касательных
double root ( double (*f)(double), double (*f_der)(double), double (*g)(double), double (*g_der)(double), double a, double b, double eps1){
  
    double delta1 = (f(b) - g(b)) - (f(a) - g(a)); // пприращение (для знака первой производной)
    double delta2 = (f(a) - g(a) + f(b) - g(b))/2 - f((a + b)/2) - g((a + b)/2); // неравенство йенсена (для знака второй производной)

    if (delta1 * delta2 > 0){ //совпадение знаков, приближаемся справа
        double c = b;
        while ((f(c) - g(c)) * (f(c - eps1) - g(c - eps1)) > 0) {
            c = c - (f(c) -g(c))/(f_der(c) - g_der(c));//ищем точку пересечения касательной из точки с с осью Ох
            num_of_iterations++;
        }
        return c;
    } 
    else{ // разные знаки, приближаемся слева
        double c = a;
   
        while ((f(c) - g(c)) * (f(c + eps1) - g(c + eps1)) > 0) {
            c = c - (f(c) -g(c))/(f_der(c) - g_der(c));//ищем точку пересечения касательной из точки с с осью Ох
            num_of_iterations++;
        }
        return c;
    }
}
//сичтаем интеграл методом симпсона
double integral ( double (*f)(double), double a, double b, double eps2){
    int n = 2; //число отрезков разпиения, умноженное на 2
    double  I_n, I_2n;  //интегральные суммы, прошлая и текущая
    double s0, s1, s2; // сумма первого и последнего/ нечетных/ четных элементов
    s0 = f(a) + f(b);
    s1 = 0.0;
    s2 = f((a + b)/2);
    I_n = s0;
    I_2n = (s0 + s2*4)*(b - a)/3;//первичный рачсёт сумм
    while(fabs(I_n - I_2n)/15.0 > eps2){
        I_n = I_2n;
        
        n *= 2;
        double delta = (b - a)/n; //текущая длина половины отрезка разбиения
        s1 = s2 + s1;//пересчёт суммы нечетных
        s2 = 0;
        for (double i = a + delta; i < b; i += 2*delta ){
            s2 +=f(i);//суммирование новых точек
        }
        I_2n = (s0 + 4*s2 + 2*s1)*delta/3;//новое значение интегральной суммы
    } 
    return I_2n;
}
//разделитель
void print_del(void){
    printf("------------------------------------------------------\n");
}
//печать точек пересечения функций из задания
void print_roots(void){
    double root1 = root(f1, f1_der, f3, f3_der, -1.4, -0.7, eps1);
    double root2 = root(f1, f1_der, f2, f2_der, 0.5, 2, eps1);
    double root3 = root(f3, f3_der, f2, f2_der, 0.5, 1.5, eps1);
    print_del();
    printf("root f1, f3: %lf\nroot f1, f2: %lf\nroot f2, f3: %lf\n", root1, root2, root3);
    print_del();
}
//печать интегралов из задания
void print_integrals(void){
    double root1 = root(f1, f1_der, f3, f3_der, -1.4, -0.7, eps1);
    double root2 = root(f1, f1_der, f2, f2_der, 0.5, 2, eps1);
    double root3 = root(f3, f3_der, f2, f2_der, 0.5, 1.5, eps1);
    double I_f1 = integral(f1, root1, root2, eps2);
    double I_f2 = integral(f2, root3, root2, eps2);
    double I_f3 = integral(f3 ,root1, root3, eps2);
    print_del();
    printf("integral f1: %lf\nintegral f2: %lf\nintegral f3: %lf\n", I_f1, I_f2, I_f3);
    print_del();
}
//печать ответа
void print_answer(void){
double root1 = root(f1, f1_der, f3, f3_der, -1.4, -0.7, eps1);
    double root2 = root(f1, f1_der, f2, f2_der, 0.5, 2, eps1);
    double root3 = root(f3, f3_der, f2, f2_der, 0.5, 1.5, eps1);
    double I_f1 = integral(f1, root1, root2, eps2);
    double I_f2 = integral(f2, root3, root2, eps2);
    double I_f3 = integral(f3 ,root1, root3, eps2);
    double ans = I_f1 - I_f2 - I_f3;
    print_del();
    printf("answer: %lf\n", ans);
    print_del();
}
//печать числа итераций работы roots на данных функциях на данном промежутке
void print_iters(int num_a, int num_b, double a, double b){
    num_of_iterations = 0;
    root(functions[num_a], derivs[num_a], functions[num_b], derivs[num_b], a, b, eps1);
    print_del();
    printf("number of iterations: %d\n", num_of_iterations);
    print_del();
}
//поиск пересечения между двумя функциями(для отладки)
void test_roots(int num_a, int num_b, double a, double b){
    double cur_root = root(functions[num_a], derivs[num_a], functions[num_b], derivs[num_b], a, b, eps1);
    print_del();
    printf("intersection of f%d and f%d on [%lf, %lf] segment: %lf \n", num_a, num_b, a, b, cur_root);
    print_del();
}
//поиск интеграла на данном промежутке(для отладки)
void test_integrals(int num, double a, double b){
    double cur_integral = integral(functions[num], a, b, eps2);
    print_del();
    printf("value of integral of f%d on [%lf, %lf] segment: %lf \n", num, a, b, cur_integral);
    print_del();
}
//печать списка фукнций и команд
void print_help_info(void){
    print_del();
    printf("realised functions:\n");
    printf("f1: y = 1 + 4/(x^2 + 1)\n");
    printf("f2: y = x^3\n");
    printf("f3: y = 2^(-x)\n");
    printf("(for tests) f4: y = 1\n");
    printf("(for tests) f5: y = x\n");
    printf("(for tests) f6: y = 4 - x^2\n");
    print_del();
    printf("to view intersection points use flag: -roots\n");
    printf("to view value of integral on segment between roots use flag: -intergals\n");
    printf("to view answer use flag: -answer\n");
    printf("to view help information use flag: -help\n");
    printf("to view number of iterations you need to calculate intersection use: -iters <number of first func> <number of second func> <left border> <right border>\n");
    printf("borders for functions from task:\n");
    printf("f1, f2: [ 0.5  , 2 ] \n");
    printf("f1, f3: [ -1.4 , -0.7 ] \n");
    printf("f2, f3: [ 0.5 , 1.5 ] \n");
    printf("to find intersection of functions use: -test-roots <number of first func> <number of second func> <left border> <right border>\n");
    printf("to calculate integral on segment use: -test-integrals <number of function> <left border> <right border>\n");
    print_del();
}
//выдать ошибку, если аргументы командной строки некорректны
void print_exeption(void){
    print_del();
    printf("you have used incorrect arguments of command line.\n Please use -help to check list of supported commands\n ");
    print_del();
}

int main (int argc, char ** argv) {
    int num_of_commands = 7;
    char * commands[] = {"-roots", "-integrals", "-answer", "-help", "-iters", "-test-roots" ,"-test-integrals"};//список команд
    int  cur_arg = 1;
  
    while (cur_arg < argc){//обработка аргументов командной строки
        int cur_num = 0;
        for (int i = 0; i < num_of_commands; i++){//соотнести аргумент с его номером для работы switch
            if (strcmp(argv[cur_arg], commands[i]) == 0){
                cur_num = i + 1;
            }
        }
        int f_a, f_b;
        double a, b;
        switch (cur_num)
        {
        case 1: //напечатать корни для функций из задания
            print_roots();
            break;

        case 2: // напечатать интегралы функций из задания
            print_integrals();
            break;

        case 3://напечатать ответ на задачу
            print_answer();
            break;

        case 4: // напечатать список команд
            print_help_info();
            break;
        case 5: // вычислить число итераций root
            f_a = atoi(argv[cur_arg + 1]), f_b = atoi(argv[cur_arg + 2]);
            a = atof(argv[cur_arg + 3]), b = atof(argv[cur_arg + 4]);
            f_a--;
            f_b--;
            cur_arg += 4;
            print_iters(f_a, f_b, a, b);
            break;
        case 6:// вычислить точку пересечения двух выбранных функций
            f_a = atoi(argv[cur_arg + 1]), f_b = atoi(argv[cur_arg + 2]);
            a = atof(argv[cur_arg + 3]), b = atof(argv[cur_arg + 4]);
            f_a--;
            f_b--;
            cur_arg += 4;
            test_roots(f_a, f_b, a, b);
            break;
        case 7:// вычислить интеграл от выбранной функции на введенном промежутке
            f_a = atoi(argv[cur_arg + 1]);
            a = atof(argv[cur_arg + 2]), b = atof(argv[cur_arg + 3]);
            f_a--;
            cur_arg += 3;
            test_integrals(f_a, a, b);
            break;
        default:
        print_exeption(); // ввели неправильную команду
            break;
        }
        cur_arg++;
    }    
    return 0;
}