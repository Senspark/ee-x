using System;

namespace EE.Internal {
    internal class FirebaseParameter {
        private const string TypeName = "Firebase.Analytics.Parameter, Firebase.Analytics";
        private static readonly Type _type = Type.GetType(TypeName);

        private static readonly ObjectActivator<object> _constructor0 =
            ObjectActivatorUtils.GetActivator<object>(
                _type.GetConstructor(new[] {
                    typeof(string),
                    typeof(string)
                }));

        private static readonly ObjectActivator<object> _constructor1 =
            ObjectActivatorUtils.GetActivator<object>(
                _type.GetConstructor(new[] {
                    typeof(string),
                    typeof(long)
                }));

        private static readonly ObjectActivator<object> _constructor2 =
            ObjectActivatorUtils.GetActivator<object>(
                _type.GetConstructor(new[] {
                    typeof(string),
                    typeof(double)
                }));

        public static Type InternalType { get; } = _type;
        public object Internal { get; }

        public FirebaseParameter(string name, string value) {
            Internal = _constructor0(name, value);
        }

        public FirebaseParameter(string name, long value) {
            Internal = _constructor1(name, value);
        }

        public FirebaseParameter(string name, double value) {
            Internal = _constructor2(name, value);
        }
    }
}