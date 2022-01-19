
package NumericalAnalysis.ComplexField;


	public class Functions
	{
			
		// a collection of the elementary functions f:C->C

		public static Complex Exp( Complex z)
		{// Exp(x+iy) = Exp(x)*Exp(iy)=Exp(x)*(Cos(y)+i*Sin(y))=(Exp(x)*Cos(y))+i*(Exp(x)*Sin(y))
			return new Complex(
				Math.exp( z.get_real())* Math.cos( z.get_immaginary()),
				Math.exp( z.get_real())* Math.sin( z.get_immaginary())
			);
		}// end Exp

		public static Complex Log( Complex z)
		{// Log(x+iy) = Log((x^2+y^2)^(1/2)) + i*Arg((x,y)) = Log( Modulus(z)) + i*Arg((x,y))
			return new Complex(
				Math.log(// real part
				z.modulus() // real part == Log( Modulus(z))
						),// end real part
				z.argument().radiants// immaginary part == Argument( on the first Riemann sheet)
			);// end return
		}// end Log

		public static Complex Log( double x)
		{// Log(x+iy) = Log((x^2+y^2)^(1/2)) + i*Arg((x,y)) = Log( Modulus(z)) + i*Arg((x,y))
			if( Math.abs( x) > 0.0 )
			{
                            return new Complex( Math.log( x), 0.0 );
			}
			else// x<=0
			{
                            return Functions.Log( new Complex( x, 0.0) );
			}
		}// end Log

//		public static Complex Sin( Complex z)
//		{
//			Complex iZ = z.operator_mul(z, z) * new Complex( 0.0, 1.0);
//			return( new Complex( Exp(iZ) - Exp(-iZ) ) / new Complex( 0.0, 2.0) );
//		}// end Sin
		public static Complex Sin( Complex z)
		{
                    Complex iZ = Complex.operator_mul(z, z);// operator_mul(new Complex(0.0, 1.0));
                    iZ = Complex.operator_mul(iZ, new Complex(0.0, 1.0) );
                    // non static operator avoided Complex iZ = z.operator_mul(z, z).operator_mul(new Complex(0.0, 1.0));
                    //Complex iZ = z.operator_mul(z, z) * new Complex( 0.0, 1.0);
                    //Complex numerator =  new Complex( Exp(iZ) - Exp(-iZ) );
                    Complex numerator =  new Complex( Complex.operator_sub( Exp(iZ), Exp(Complex.operator_sub(iZ)) ) );
                    //Complex res = numerator.operator_div( new Complex( 0.0, 2.0) );
                    Complex res = Complex.operator_div( numerator , new Complex( 0.0, 2.0) );
                    //return() / new Complex( 0.0, 2.0) );
                    return res;
		}// end Sin                

		public static Complex Cos( Complex z)
		{
			//Complex iZ = z * new Complex( 0.0, 1.0);
                    Complex iZ = Complex.operator_mul(z, new Complex( 0.0, 1.0) );
                    Complex numerator = 
                                Complex.operator_add( Exp(iZ), Exp(Complex.operator_sub(iZ)) );
                    
                        // new Complex( 2.0, 0.0) );
                      Complex res = Complex.operator_div( numerator , new Complex( 2.0, 0.0) );
                      return res;
		}// end Cos


        //--------- non elementary functions --------------------------

//
//        /// <summary>
//        /// zeta in { C \ { -n, .., -2, -1, 0 } }
//        /// </summary>
//        /// <param name="zeta"> zeta in C. In R there is an integral by part, that coincides with this product </param>
//        /// <param name="threshold"> cutting threshold of an infinite product </param>
//        /// <returns></returns>
//        public static Complex Gamma_function(
//            Complex zeta,
//            long threshold
//            )
//        {
//            Complex res = null;
//            Complex reciproco_z = new Complex(1.0 / zeta);
//            Complex numeratore = new Complex(1.0, 0.0);// product invariant.
//            Complex denominatore = new Complex(numeratore);// product invariant.
//            //
//            for (long k = 1L; k <= threshold; k++)
//            {
//                // numeratore *= (1+1/n)^z        complex power 
//                numeratore *= new Complex(
//                    new Complex(1.0 + 1.0 / (double)k, 0.0) ^ zeta);
//                // denominatore *= (1+z/n)        complex ratio
//                denominatore *= new Complex(1.0 + zeta / (double)k);
//            }// end loop
//            // finalize:
//            res = new Complex(numeratore / denominatore);
//            // res *= reciproco_z
//            res *= reciproco_z;
//            // ready
//            return res;
//        }// end Gamma_function
//
//
//
//        /// <summary>
//        /// Serie Eta, i.e. variante della serie di Dirichlet.
//        /// La serie di Dirichlet converge per Re(s)=Re( sigma+i t) = sigma > +1
//        /// La serie Eta converge per Re(s)=Re( sigma+i t) = sigma > 0
//        /// La Eta e' utilizzata per valutare la Zeta di Riemann nella striscia critica. A tale scopo
//        /// e' necessario utilizzare anche l'equazione funzionale, che individua un'asse di simmetria nella
//        /// retta critica.
//        /// Sussiste la seguente relazione:
//        /// Zeta(s) = Eta(s) / (1-1/(2^(s-1)))
//        ///     ove s =: (sigma + i*t),  sigma,t in R
//        /// C'e' un polo in s =  (sigma + i*t) = (+1, 0)
//        /// L'equazione funzionale permette di esprimere Zeta(s) in funzione di Zeta(1-s)
//        /// Il fatto che Eta sia convergente per ogni sigma>0( escluso s =  (sigma + i*t) = (+1, 0)), 
//        /// permette dunque di rappresentare Zeta
//        /// su tutto il piano C( escluso s =  (sigma + i*t) = (+1, 0)).
//        /// </summary>
//        /// <param name="threshold"></param>
//        /// <returns></returns>
//        public static Complex eta_Dirichlet(
//            Complex esse,
//            long threshold
//          )
//        {
//            Complex sum = new Complex( 0.0, 0.0);
//            double numeratore = 0.0;// always real.
//            Complex denominatore = null;
//            //
//            for (long n = 1; n < threshold; n++)
//            {
//                numeratore = System.Math.Pow(-1.0, (double)(n + 1.0));
//                denominatore = new Complex(((double)(n)) ^ esse);
//                sum += numeratore / denominatore;
//                //
//                denominatore = null;// gc.
//            }// end for
//            // ready
//            return sum;
//        }// end eta_Dirichlet
//
//
//
//        /// <summary>
//        /// Zeta( sigma + I*t) = Eta( sigma + I*t) / (1-2^(1-(sigma + I*t)))
//        /// 
//        /// NB. as is, only works for Re(s)>0 strictly. For Re(s)smaller_or_equal(0) the functional equation is required.
//        /// TODO iff( Re(esse)smaller_or_equal(0)) return functional_equation(esse)
//        /// TODO iff( Re(esse)greater(1) return Dirichlet series; the one with all positive terms: faster convergence.
//        /// </summary>
//        /// <param name="esse"></param>
//        /// <param name="threshold"></param>
//        /// <returns></returns>
//        public static Complex zeta_Riemann(
//            Complex esse,
//            long threshold
//          )
//        {
//            Complex res = null;
//            //
//            if (
//                esse.get_real > 0.0
//              )
//            {
//                Complex numeratore = eta_Dirichlet(esse, threshold);
//                Complex denominatore = new Complex(
//                    1.0 - (2.0 ^ (1.0 - esse)));// NB. le precedenze vanno esplicitate.
//                res = new Complex(numeratore / denominatore);
//            }
//            //else if (esse.get_real > 1.0) NB. coperto dal caso "eta" : esse.get_real > 0.0
//            //{
//            //    // TODO  return Dirichlet series; the one with all positive terms: faster convergence.
//            //}
//            else // i.e. esse.get_real <= 0.0 ovvero sigma<=0
//            {
//                /* s =: sigma + I*t
//                 * sigma<=0 porta a valutare (1-s) in functional_equation().
//                 * tale valutazione comporta una call-back a zeta_Riemann(1-s).
//                 * tale call-back comporterebbe un'ulteriore chiamata a functional_equation(),
//                 *      qualora Re(1-s)<=0. Cio' si verificherebbe solo per sigma>=1. La prima chiamata a  
//                 *      functional_equation() avviene se e solo se sigma<=0. 
//                 *      L'intersezione vuota fra i due insiemi 
//                 *      garantisce che la call-back non inneschi un ciclo infinito.
//                 */
//                res = Zeta_functionalEquation( esse, threshold);
//            }
//            // ready
//            return res;
//        }// end zeta_Riemann(
//
//
//
//
//        /// <summary>
//        ///  
//        /// functional equation Zeta( s) = f( Zeta( 1-s))
//        /// 1-s =(1,0)-(sigma,t)=(1-sigma,-t)
//        /// Zeta[s] = 2^s Pi^(s - 1) Sin[Pi s/2] Gamma[1 - s] Zeta[1 - s]
//        ///    NB. variables in the implementation are labeled as:
//        ///    factor_a = 2^s
//        ///    factor_b = Pi^(s - 1)
//        ///    factor_c = Sin[Pi s/2] 
//        ///    follow the factors Gamma and Zeta[1 - s]
//        /// </summary>
//        /// <param name="esse"></param>
//        /// <returns></returns>
//        public static Complex Zeta_functionalEquation(
//            Complex esse,
//            long threshold
//          )
//        {
//            Complex factor_a = new Complex(2.0 ^ esse);
//            Complex factor_b = new Complex(System.Math.PI ^ (esse - 1));
//            Complex factor_c = new Complex(
//                Functions.Sin(System.Math.PI * esse / 2.0));
//            Complex factor_gamma = new Complex(
//                Functions.Gamma_function( 1.0 - esse, threshold));
//            Complex factor_zeta = new Complex(
//                Functions.zeta_Riemann( 1.0 - esse, threshold));//NB. zeta is the caller: avoid loops on call-back!
//            Complex res =
//                factor_a
//                * factor_b
//                * factor_c
//                * factor_gamma
//                * factor_zeta;
//            // ready
//            return res;
//        }// end functional_equation()
//
//
	}// end class

