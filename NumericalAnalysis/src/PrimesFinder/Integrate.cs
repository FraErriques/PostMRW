﻿using System;



namespace PrimesFinder
{

    public static class Integrate
    {
        public delegate double Functional_form(double x);



        public static double f(double x)
        {
            return 1.0 / Math.Log( x);
        }//
        public static double f1(double x)
        {
            return  Math.Sin( x);
        }//
        /// <summary>
        /// restrizione ad R+, dell'integrando della Gamma di Eulero.
        /// </summary>
        /// <param name="z"></param>
        /// <returns></returns>
        public static double IntegrandoGamma(double t)
        {
            double z = 7.0;// Gamma(7)==6!=720
            return Math.Pow(t, z - 1) * Math.Exp(-t);
        }//


        /// <summary>
        /// Trapezium Integration.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="n"></param>
        /// <returns></returns>
        public static double Integrate_equi_trapezium(double a, double b, Int64 n)
        {
            Functional_form functional_form = new Functional_form( f);// NB. default==LogIntegral. Modificare questa riga del sorgente per variare l'integrando.
            double delta = (b - a) / (double)n,
            res = 0.0,
            x = a + delta;// start from inf + delta
            //
            for (; x < b; x += delta)
            {// sum all the internal sides
                res += functional_form(x);
            }
            res *= delta; // multiply them for the common base
            res += (functional_form(a) + functional_form(b)) * 0.5 * delta; // (add extrema) * base/2
            //
            return res;
        }//




        /// <summary>
        /// Integrate_percentile trapezium
        /// </summary>
        /// <param name="measure"></param>
        /// <returns></returns>
        public static double Integrate_percentile_trapezium(double left_bound, double measure, double delta)
        {
            Functional_form functional_form = new Functional_form( f);
            // NB. delta = 0.03  resulted optimal in a test, for 1/ln(t)
            double res = 0.0,
                   x = left_bound;
            //
            for (; res <= measure; x += delta)
            {
                res += 0.5 * delta * (functional_form(x + delta) + functional_form(x));
            }
            //
            return x; // return the value, until which I integrated
        }//


        /// <summary>
        /// Integrate_percentile equilog
        /// </summary>
        /// <param name="measure"></param>
        /// <returns></returns>
        public static double Integrate_percentile_equilog(double left_bound, double measure, double delta)
        {
            Functional_form functional_form = new Functional_form( f);
            // NB. delta = 0.03  resulted optimal in a test, for 1/ln(t)
            double res = 0.0,
                   f_x_delta, f_x,
                   x = left_bound;
            //
            for (; res <= measure; x += delta)
            {
                f_x_delta = functional_form(x + delta);
                f_x = functional_form(x);
                res += delta * (f_x_delta - f_x) / (Math.Log(f_x_delta) - Math.Log(f_x));
            }
            //
            return x; // return the value, until which I integrated
        }//



        /// <summary>
        /// Integrate_equi_log
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="n"></param>
        /// <returns></returns>
        public static double Integrate_equi_log(double a, double b, Int64 n)
        {
            Functional_form functional_form = new Functional_form(f);
            double res = 0.0,
                    delta = (b - a) / (double)n,
                    f_x_delta, f_x,
                    x = a;
            //
            for (; x < b; x += delta)
            {
                f_x_delta = functional_form(x + delta);
                f_x = functional_form(x);
                res += delta * (f_x_delta - f_x) / (Math.Log(f_x_delta) - Math.Log(f_x));
            }
            //
            return res;
        }//


    }//


}
