using System;
using System.Reflection;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class FirebaseAnalyticsImpl : IFirebaseAnalyticsImpl {
        private readonly MethodInfo _methodSetCurrentScreen;
        private readonly MethodInfo _methodLogEvent;
        private readonly MethodInfo _methodLogEventParameters;
        private readonly Type _typeParameter;
        private readonly ConstructorInfo _constructorLong;
        private readonly ConstructorInfo _constructorDouble;
        private readonly ConstructorInfo _constructorString;

        public FirebaseAnalyticsImpl() {
            var type = Type.GetType("Firebase.Analytics.FirebaseAnalytics, Firebase.Analytics");
            Assert.IsNotNull(type);

            _typeParameter = Type.GetType("Firebase.Analytics.Parameter, Firebase.Analytics");
            Assert.IsNotNull(_typeParameter);

            _methodSetCurrentScreen = type.GetMethod("SetCurrentScreen", new[] {typeof(string), typeof(string)});
            _methodLogEvent = type.GetMethod("LogEvent", new[] {typeof(string)});
            _methodLogEventParameters =
                type.GetMethod("LogEvent", new[] {typeof(string), _typeParameter.MakeArrayType()});
            Assert.IsNotNull(_methodSetCurrentScreen);
            Assert.IsNotNull(_methodLogEvent);
            Assert.IsNotNull(_methodLogEventParameters);

            _constructorLong = _typeParameter.GetConstructor(new[] {typeof(string), typeof(long)});
            _constructorDouble = _typeParameter.GetConstructor(new[] {typeof(string), typeof(double)});
            _constructorString = _typeParameter.GetConstructor(new[] {typeof(string), typeof(string)});
            Assert.IsNotNull(_constructorLong);
            Assert.IsNotNull(_constructorDouble);
            Assert.IsNotNull(_constructorString);
        }

        public void SetCurrentScreen(string screenName, string screenClass) {
            _methodSetCurrentScreen.Invoke(null, new object[] {screenName, screenClass});
        }

        public void LogEvent(string name) {
            _methodLogEvent.Invoke(null, new object[] {name});
        }

        public void LogEvent(string name, (string, object)[] parameters) {
            var firebaseParameters = Array.CreateInstance(_typeParameter, parameters.Length);
            for (var i = 0; i < parameters.Length; ++i) {
                object param = null;
                var (paramName, paramValue) = parameters[i];
                var paramType = paramValue.GetType();
                if (paramType == typeof(bool)) {
                    param = _constructorLong.Invoke(new object[] {paramName, (bool) paramValue ? 1 : 0});
                } else if (paramType == typeof(int) || paramType == typeof(long)) {
                    param = _constructorLong.Invoke(new[] {paramName, paramValue});
                } else if (paramType == typeof(float) || paramType == typeof(double)) {
                    param = _constructorDouble.Invoke(new[] {paramName, paramValue});
                } else if (paramType == typeof(string)) {
                    param = _constructorString.Invoke(new[] {paramName, paramValue});
                } else {
                    Assert.IsFalse(true);
                }
                firebaseParameters.SetValue(param, i);
            }
            _methodLogEventParameters.Invoke(null, new object[] {name, firebaseParameters});
        }
    }
}