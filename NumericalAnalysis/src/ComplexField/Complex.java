package NumericalAnalysis.ComplexField;


	public class Complex
	{
		private double real;
		private double immaginary;
		public class Argument
		{
                    public boolean	isTheNullVector;
                    public double	degrees;
                    public double	radiants;
		};// end  struct Argument
		private final double epsilon = +4.94065645841247E-324;
		// read only property
		public double get_real()
		{
                    return this.real;
		}
		// read only property
		public double get_immaginary()
		{
                    return this.immaginary;
		}
		// Constructors
		// no default constructor. The complex number must be initialized in both its real and immaginary part.
		public Complex( double real, double immaginary)
		{
                    this.real = real; 
                    this.immaginary = immaginary;
		}// end constructor

		public Complex( Complex that)//copy-constructor
		{
                    this.real = that.real;
                    this.immaginary = that.immaginary;
		}// end copy-constructor
		// end Constructors

		// operators
		// operator +
		public static Complex operator_add ( Complex left, Complex right)
		{// (a,b)+(c,d)= (a+c) + i (b+d)
			return new Complex( left.real + right.real,
				left.immaginary + right.immaginary    );
		}// binary +
		public static Complex operator_add ( double left, Complex right)
		{// (a )+(c,d)= (a+c) + i ( d)
			return new Complex( left  + right.real,
				+ right.immaginary    );
		}// binary +
		public static Complex operator_add ( Complex left, double right)
		{// (a,b)+(c )= (a+c) + i (b )
			return new Complex( left.real + right,
				left.immaginary     );
		}// binary +
		public static Complex operator_add ( Complex z)
		{// +(a,b) = (+a,+b)
			return z;// == new Complex( z.real, z.immaginary );
		}// unary +
		// end operator +

		// operator -
		public static Complex operator_sub ( Complex left, Complex right)
		{// (a,b)-(c,d)= (a-c) + i (b-d)
			return new Complex( left.real - right.real,
				left.immaginary - right.immaginary    );
		}// end binary -
		public static Complex operator_sub ( double left, Complex right)
		{// (a )-(c,d)= (a-c) + i ( -d)
			return new Complex( left  - right.real,
				- right.immaginary    );
		}// end binary -
		public static Complex operator_sub ( Complex left, double right)
		{// (a,b)-(c )= (a-c) + i (b )
			return new Complex( left.real - right ,
				left.immaginary          );
		}// end binary -
		public static Complex operator_sub ( Complex z)
		{// -(a,b) = (-a,-b)
			return new Complex( -z.real, -z.immaginary );
		}// end unary -
		// operator -

		// operator *
		public static Complex operator_mul (Complex left, Complex right)
		{// (a,b)(c,d)= (ac-bd) + i (ad+bc)
			return new Complex( left.real*right.real - left.immaginary*right.immaginary,
				left.real*right.immaginary + left.immaginary*right.real         );
		}
                //
		public static Complex operator_mul ( double left, Complex right)
		{// (a,0)(c,d)= (ac ) + i (ad )
                    return new Complex( left *right.real,
                        left *right.immaginary  );
		}
		public static Complex operator_mul ( Complex left, double right)
		{// (a,0)(c,d)= (ac ) + i (ad )
                    return new Complex( left.real*right,
                        left.immaginary*right);
		}
		// end operator *

		// operator /
		public static Complex operator_div ( Complex left, Complex right)
		{// (a,b) / (c,d)= (a,b)/(c,-d) * (c,-d)/(c,d) = (a,b)(c,-d)/(c^2,d^2)
			//  es.   1/i = -i
			// implementazione:
			// - prodotto del dividendo per il coniugato del divisore
			// - dividere parte reale e parte immaginaria del dividendo per il moduloquadro del divisore
                        Complex numerator = Complex.operator_mul(left, right.coniugate() );
			double den = Complex.modulusSquare(right);
			Complex result = numerator.operator_div(numerator,den);
			return result;
		}
		public static Complex operator_div ( double left, Complex right)
		{// (a ) / (c,d)= (a )/(c,-d) * (c,-d)/(c,d) = (a,0)(c,-d)/(c^2+d^2,0)
			//  es.   1/i = -i
			// implementazione:
			// - prodotto del dividendo per il coniugato del divisore
			// - dividere parte reale e parte immaginaria del dividendo per il moduloquadro del divisore
                        double denominatorSquareModulus = Complex.modulusSquare(right);// abs^2
                        Complex result = Complex.operator_mul(right.coniugate(), new Complex(left,0.0));
			return new Complex( result.real/denominatorSquareModulus, result.immaginary/denominatorSquareModulus);
		}
		public static Complex operator_div ( Complex left, double right)
		{// (a,b) / (c,0)= (a,b)/(c,0) = (a,b)(c,0)/(c^2,0)
			//  es.   1/i = -i
			// implementazione:
			// - dividere parte reale e parte immaginaria per il divisore( right)
			return new Complex( left.real/right, left.immaginary/right);
		}
		// end operator /


        


        /// <summary>
        ///  operator ^ represents Power; eg. a^b==Exp( b*Log(a))
        /// </summary>
        /// <param name="theBase">base</param>
        /// <param name="theExp">exponent</param>
        /// <returns>power</returns>
		public static Complex operator_pow ( Complex theBase, Complex theExp)
		{// (a,b) ^ (c,d)= Exp( (c,d)*Log((a,b)) )
                    //  es.   2^3 = 2^(3*Log2(2))
                    // implementazione:
                    return(
                        new Complex( Functions.Exp( Complex.operator_mul(theExp,  Functions.Log( theBase)
                                    ) //end mul
                                )// end Exponential call
                            )// end new
                    );// end return
		}// end operator ^


		public static Complex operator_pow ( double theBase, Complex theExp)
		{// (a,0) ^ (c,d)= Exp( (c,d)*Log((a )) )
			//  es.   2^3 = 2^(3*Log2(2))
			// implementazione:
			return(
                            new Complex( Functions.Exp(  Complex.operator_mul(theExp,
                                Functions.Log( new Complex( theBase, 0.0) )
                              ) //end mul
                            )// end Exponential call
                        )// end new
                    );// end return
		}// end operator ^
                
		public static Complex operator_pow ( Complex theBase, double theExp)
		{// (a,b) ^ (c )= Exp( (c,0)*Log((a,b)) )
			//  es.   2^3 = 2^(3*Log2(2))
			// implementazione:
			return(
				new Complex( Functions.Exp(
                                    Complex.operator_mul( theExp, Functions.Log( theBase))
				)// end Exponential call
                            )// end new
			);// end return
		}
		// end operator ^

//		// operator(s) if(Complex)
//		// Returns true if the considered complex numer has immaginary-part coefficient zero.
//		public static bool operator true( Complex z)
//		{
//			if( Math.abs( z.immaginary) < epsilon)
//			{
//				return true;
//			}
//			return false;
//		}

//		// Returns false if the considered complex numer has immaginary-part coefficient NON zero.
//		public static bool operator false( Complex z)
//		{
//			if( Math.abs( z.immaginary) < epsilon)
//			{
//				return true;
//			}
//			return false;
//		}
//		// end operator if(Complex)
//
//		// operator cast, explicit
//		public static explicit operator double( Complex z)
//		{
//			if( Math.abs( z.immaginary) < epsilon)
//			{
//				return z.get_real;
//			}
//			throw new System.Exception( "the considered complex number is NOT real. Cannot convert");
//		}
//		// end operator cast, explicit

		// helper method modulus
		public double modulus()
		{// the Euclidean-R2-length
                    return Math.sqrt( 
                        Math.pow( this.real, 2.0)		+
                        Math.pow( this.immaginary, 2.0)
                        );
		}//
		//
		public static double modulus( Complex z)
		{// the Euclidean-R2-length
			return Math.sqrt( 
				Math.pow( z.real, 2.0)		+
				Math.pow( z.immaginary, 2.0)
				);
		}//
		//
		public static double modulusSquare( Complex z)
		{// the Euclidean-R2-length
                    return 
                        Math.pow( z.real, 2.0)		+
                        Math.pow( z.immaginary, 2.0);
		}// modulo quadro.
		// end helper method modulus

		public Complex coniugate()
		{// (a,b).coniugate = (a,-b)
                    return new Complex( this.real, -this.immaginary);
		}// end coniugate

		public Argument argument()
		{// this.argument() represents the angle formed by the (this.real, this.immaginary) R2 vector with
			// the abscissa positive semi-axis, with variation between (-Pi, Pi]
			Argument theArgument = new Argument();
			// if x==0
			if( Math.abs( this.real) < epsilon )
			{
				if( Math.abs( this.immaginary) < epsilon )
				{
					theArgument.isTheNullVector = true;
					return theArgument;
				}
				else if( this.immaginary > 0.0)
				{
					theArgument.radiants = Math.PI/2.0;
					theArgument.isTheNullVector = false;
				}
				else if( this.immaginary < 0.0)
				{
					theArgument.radiants = -Math.PI/2.0;
					theArgument.isTheNullVector = false;
				}
			}// end  if x==0
			else if( this.real > 0.0)
			{// if x>0
				theArgument.radiants = Math.atan( this.immaginary / this.real );
				theArgument.isTheNullVector = false;
			}// end if x>0
			else if( this.real < 0.0)
			{// if x<0
				if( Math.abs( this.immaginary) < epsilon )
				{
					theArgument.radiants = Math.PI;
					theArgument.isTheNullVector = false;
				}
				else if( this.immaginary > 0.0)
				{
					theArgument.radiants = Math.PI + Math.atan( this.immaginary / this.real );
					theArgument.isTheNullVector = false;
				}
				else if( this.immaginary < 0.0)
				{
					theArgument.radiants = -Math.PI + Math.atan( this.immaginary / this.real );
					theArgument.isTheNullVector = false;
				}
			}// end if x<0
			// deg:180 = rad:PI  -> deg=rad*180/PI
			theArgument.degrees = theArgument.radiants * 180.0 / Math.PI;
			return theArgument;
		}// end argument method


		public String ToString()
		{
                    String RealPart = String.valueOf( this.real);
                    String ImmaPart = String.valueOf( this.immaginary);
                    return "("+RealPart+") +i*( "+ImmaPart+")";
		}// ToString

                
		//		public override bool Equals(object obj)
		//		{
		//			return object.ReferenceEquals( this, obj);
		//		}

		//		public override bool Equals(object obj)
		//		{
		//			bool result;
		//			Comlex B = (Complex)obj;
		//
		//			if( B.r
		//		}

		// end helper methods

	}// end class
