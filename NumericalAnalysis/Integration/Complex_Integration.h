#ifndef COMPLEX_INTEGRATION_H
#define COMPLEX_INTEGRATION_H


//the present namespace is intended to provide Contour-Integrals in the Complex plane, as in:
//Integrate[f(x+I*y)dz]==Integrate[ (u(x+I*y)+I*v(x+I*y))*(dx+I*dy)]==
//Integrate[(u,v)(dx,dy)]== Integrate[u_dx-v_dy +I*(u_dy+v_dx)]==
//Integrate[u(x,y)_dx]-Integrate[v(x,y)_dy] +I*{Integrate[u(x,y)_dy]+Integrate[v(x,y)_dx]}
//Oss. In the last two lines the notation Integrate[f()_dm] has been assumed, to indicate that the CoChain f()
//is integrated in the Measure dm.
//for the purposes of Numerical Analysis, a single Integrate[f(z)_dz] leads to four real integrals, as shown
//three lines above.
//The Chain has to be parametrized, in a single real variable (it's a one dimensional variety) and the CoChain has
//to be restricted to the Chain(on each smooth portion of it).
//The differentials of the coordinate-functions of the chain, will introduce measure terms.
//Eg. u(z)==u(x+I*y)==u(x[t]+I*y[t]) -> u(z)_dx==u(x[t]+I*y[t])_x'[t]dt
//where the factor x'[t] is a coonsequence of the Diffeomorphism of integrating in dx==d(x[t])==x'[t]dt
//The subject matter of the Diffeomorphism of u(z)_dx has to applied to all four Real integrals:
// { u_dx,u_dy, v_dx,v_dy}
//
//notes from Wiki:
//Calcolare gli integrali con metodi diretti significa calcolarli con metodi simili a quelli usati per gli
//integrali curvilinei di funzioni a più variabili. Ciò significa che usiamo il seguente metodo:
//    - parametrizzazione del contorno
//        Il contorno è parametrizzato da una funzione derivabile che manda numeri reali in numeri complessi, oppure il
//        contorno viene diviso in pezzi che vengono parametrizzati separatamente.
//    - sostituzione della parametrizzazione nell'integranda
//        Sostituire la parametrizzazione nell'integranda trasforma l'integrale in uno di una variabile reale.
// la nota riporta inoltre il metodo di cercare la primitiva ed applicare il Thm di Stokes; cosa non praticabile in un contesto
// di AnalisiNumerica, nel quale non sia disponibile un parser e l'algoritmo di Risch; cose queste che appartengono al calcolo simbolico,
// implementato da prodotti di Computer-Algebra, quali Wolfram-Mathematica. Nel contesto dell'Analisi Numerica si utilizzano invece discretizzazioni
// quali l'Integrale di Riemann.
//
//notes from Calculus2015 :
// Trapezium Integration. NB.: (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
// Purpose of this routine -Trapezia- is to provide a numerical solution, suitable for the cases where no primitive can be found.
// The technique of Trapezia is to subdivide the domain in "n" equal Deltas and evaluate the integrand at the extrema of each trapezium
// instead of evaluating a primitive at the boundary, as in Stokes' theorems. Therefore the first step is to choose a contour, over which
// the integration will be performed. Due to Cauchy theorem, for olomorphic functions there's no involvment of the contour when integrating
// over omotopic paths. Nonetheless the utility of numerical methods -as Trapezia- is to allow the evaluation even of functions
// not endowed of an elementary primitive. For that purpose, we cannot rely on the behaviour (of the primitive) at the extrema. We have to
// explicitly follow a path that connects (in a Jordan way) the extrema. First step will be the choice of a contour and of its parametrization.
// In the C-plane the contour will be as L:{x(t)=f(t), y(t)=g(t)} and so dx=x'(t)dt , dy=y'(t)dt. Due to this, the two coordinate functions:
// u=u(x,y) and v(x,y) will have to be restricted to the path as in u=u(x(t),y(t)) , v=v(x(t),y(t))
// Therefore the classical:  (u(x,y)+I*v(x,y) )*(dx+I*dy)==u*dx-v*dy + I*( u*dy+v*dx)
// will become: u(x(t),y(t))*dx-v(x(t),y(t))*dy + I*( u(x(t),y(t))*dy+v(x(t),y(t))*dx) which will be
// u(x(t),y(t))*x'(t)dt-v(x(t),y(t))*y'(t)dt + I*( u(x(t),y(t))*y'(t)dt+v(x(t),y(t))*x'(t)dt)
// the two not-homogeneous quantities that must be calculated separately are:
//      u(x(t),y(t))*x'(t)dt-v(x(t),y(t))*y'(t)dt   for the Real-part
//      u(x(t),y(t))*y'(t)dt+v(x(t),y(t))*x'(t)dt   for the Immaginary-part
//
// The routine in this class is intended to be for elementary functions, so they will accept just the complex argument and no other parameter whatsoever.
// The user will have to provide function-pointers (i.e. delegates) to the following methods:
// double x(double t) it's the x-coordinate function -abscissa- of the parametrization for the Jordan curve, in the C-plane
// double y(double t) it's the y-coordinate function -ordinate- of the parametrization for the Jordan curve, in the C-plane
// double dx(double t) it's the x-differential; it has to contain everything that is a factor of dt es. dx=2*t*dt -> dx returns(2*t), accepting t.
// double dy(double t) it's the y-differential; it has to contain everything that is a factor of dt es. dy=3*Sin(t)*dt -> dy returns(3*Sin(t)), accepting t.
// The user will have to provide also function-pointers to:
// double u(double x, double y) which will be fed as u(x(t),y(t)) and the same for v(x(t),y(t)).
// This class instead will provide:
// a method double RealPartIntegral which performs: u*dx-v*dy in terms of u(x(t),y(t))*dx(t)-v(x(t),y(t))*dy(t)
// the RealPartIntegral method will accept: t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y)
// a method double ImmaginaryPartIntegral which performs: I*( u*dy+v*dx) in terms of u(x(t),y(t))*dy(t)+v(x(t),y(t))*dx(t)
// the ImmaginaryPartIntegral method will accept: t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y)
// NB. in future implementations the management method, which calls both {RealPartIntegral,ImmaginaryPartIntegral} could spawn separate threads for them and then join the results.
// So this class instead will provide a management method -the only public one- wich will have signature:
// ComplexField.Complex ContourIntegral(  t_inf, t_sup, n_card_trapezia, f_pointer_x(t), f_pointer_y(t), f_pointer_dx(t), f_pointer_dy(t), f_pointer_u(x,y), f_pointer_v(x,y) )
// It will call  both {Integrate_equi_trapezium_RealPart,Integrate_equi_trapezium_ImmaginaryPart}.
// Have fun.
//
// As an aid to successive implementation, I'm providing a set of the function that are needed by this algorithm. They are u,v,x,y,dx,dy
// the functions choosen for the example are f(z)=z which implies u(x,y)=x, v(x,y)=y; the choice for the contour is x(t)=t,y(t)=2*t+1,dx=dt which means dx=1
// dy=2*dt which means dy=2.

namespace Complex_Integration{

typedef double (*coordinate_func_ptr)(double);// this will work fot x[t] and y[t]
typedef double (*ComplexImage_func_ptr)(double,double);// this will work fot u=u(x,y) and v=v(x,y)


}// nmsp

#endif // COMPLEX_INTEGRATION_H
