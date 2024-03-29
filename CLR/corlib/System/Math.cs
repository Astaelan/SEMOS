using System.Runtime.CompilerServices;

#if LOCALTEST
using System;
namespace System_ {
#else
namespace System {
#endif
	public static class Math {

		public const double E = 2.7182818284590452354;
		public const double PI = 3.14159265358979323846;

		#region Abs()

		public static sbyte Abs(sbyte v) {
			if (v == sbyte.MinValue) {
				throw new OverflowException("Value is too small");
			}
			return (v >= 0) ? v : (sbyte)-v;
		}

		public static short Abs(short v) {
			if (v == short.MinValue) {
				throw new OverflowException("Value is too small");
			}
			return (v >= 0) ? v : (short)-v;
		}

		public static int Abs(int v) {
			if (v == int.MinValue) {
				throw new OverflowException("Value is too small");
			}
			return (v >= 0) ? v : -v;
		}

		public static long Abs(long v) {
			if (v == long.MinValue) {
				throw new OverflowException("Value is too small");
			}
			return (v >= 0) ? v : -v;
		}

		public static float Abs(float v) {
			return (v >= 0) ? v : -v;
		}

		public static double Abs(double v) {
			return (v >= 0) ? v : -v;
		}

		#endregion

		#region Min()

		public static sbyte Min(sbyte v1, sbyte v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static short Min(short v1, short v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static int Min(int v1, int v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static long Min(long v1, long v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static byte Min(byte v1, byte v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static ushort Min(ushort v1, ushort v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static uint Min(uint v1, uint v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static ulong Min(ulong v1, ulong v2) {
			return (v1 < v2) ? v1 : v2;
		}

		public static float Min(float v1, float v2) {
			if (float.IsNaN(v1) || float.IsNaN(v2)) {
				return float.NaN;
			}
			return (v1 < v2) ? v1 : v2;
		}

		public static double Min(double v1, double v2) {
			if (double.IsNaN(v1) || double.IsNaN(v2)) {
				return double.NaN;
			}
			return (v1 < v2) ? v1 : v2;
		}

		#endregion

		#region Max()

		public static sbyte Max(sbyte v1, sbyte v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static short Max(short v1, short v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static int Max(int v1, int v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static long Max(long v1, long v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static byte Max(byte v1, byte v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static ushort Max(ushort v1, ushort v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static uint Max(uint v1, uint v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static ulong Max(ulong v1, ulong v2) {
			return (v1 > v2) ? v1 : v2;
		}

		public static float Max(float v1, float v2) {
			if (float.IsNaN(v1) || float.IsNaN(v2)) {
				return float.NaN;
			}
			return (v1 > v2) ? v1 : v2;
		}

		public static double Max(double v1, double v2) {
			if (double.IsNaN(v1) || double.IsNaN(v2)) {
				return double.NaN;
			}
			return (v1 > v2) ? v1 : v2;
		}

		#endregion

		#region Sign()

		public static int Sign(sbyte v) {
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		public static int Sign(short v) {
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		public static int Sign(int v) {
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		public static int Sign(long v) {
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		public static int Sign(float v) {
			if (float.IsNaN(v)) {
				throw new ArithmeticException("NaN");
			}
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		public static int Sign(double v) {
			if (double.IsNaN(v)) {
				throw new ArithmeticException("NaN");
			}
			return (v > 0) ? 1 : ((v < 0) ? -1 : 0);
		}

		#endregion

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static double Sin(double x);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static double Cos(double x);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static double Tan(double x);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static double Pow(double x, double y);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static double Sqrt(double x);
	}
}
