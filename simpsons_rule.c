
double
execute_at_every_point_simpsons_rule(
        double (*f)(double,void*),
        double a,
        double h,
        size_t n,
        void* additional_information_to_integration_function)
{
    double b = a + n*x; 
    double prev_x = a;
    double x = a + h;
    double next_x = x + h;
    double f_prev_x = f(prev_x,
        additional_information_to_integration_function);
    double f_x;
    double f_next_x;
    double S = 0;
    double half_of_h = h/2;
    while(next_x < b)
    {
        f_x = f(x, additional_information_to_integration_function);
        f_next_x = 
            f(next_x, additional_information_to_integration_function);
        S += f_prev_x + 4*f_x + f_next_x;
        f_prev_x = f_next_x;
        prev_x = next_x;
        x = prev_x + half_of_h;
        next_x = x + half_of_h;
    }  
    retun
}


