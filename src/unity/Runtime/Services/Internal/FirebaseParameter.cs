using System;
using System.Reflection;

namespace EE.Internal {
    internal class FirebaseParameter {
        private const string TypeName = "Firebase.Analytics.Parameter, Firebase.Analytics";
        private static readonly Type _type = Type.GetType(TypeName);

        private static readonly ConstructorInfo _constructor0 =
            _type.GetConstructor(new[] {
                typeof(string),
                typeof(string)
            });

        private static readonly ConstructorInfo _constructor1 =
            _type.GetConstructor(new[] {
                typeof(string),
                typeof(long)
            });

        private static readonly ConstructorInfo _constructor2 =
            _type.GetConstructor(new[] {
                typeof(string),
                typeof(double)
            });

        public static Type InternalType { get; } = _type;
        public object Internal { get; }

        public FirebaseParameter(string name, string value) {
            Internal = _constructor0.Invoke(new object[] {name, value});
        }

        public FirebaseParameter(string name, long value) {
            Internal = _constructor1.Invoke(new object[] {name, value});
        }

        public FirebaseParameter(string name, double value) {
            Internal = _constructor2.Invoke(new object[] {name, value});
        }
    }
}