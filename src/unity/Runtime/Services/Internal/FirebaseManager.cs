using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    internal static class FirebaseManager {
        private static Task<bool> _initializer;

        public static Task<bool> Initialize() => _initializer = _initializer ?? (_initializer = InitializeImpl());

        private static async Task<bool> InitializeImpl() {
            // https://firebase.google.com/docs/unity/setup
            var typeFirebaseApp = Type.GetType("Firebase.FirebaseApp, Firebase.Core");
            if (typeFirebaseApp == null) {
                throw new ArgumentException("Cannot find Firebase.FirebaseApp");
            }
            var methodCheckAndFixDependenciesAsync = typeFirebaseApp.GetMethod("CheckAndFixDependenciesAsync");
            Assert.IsNotNull(methodCheckAndFixDependenciesAsync);

            var status = await (Task<object>) methodCheckAndFixDependenciesAsync.Invoke(null, new object[] { });

            var typeDependencyStatus = Type.GetType("Firebase.DependencyStatus, Firebase.Core");
            Assert.IsNotNull(typeDependencyStatus);
            var fieldDependencyStatusAvailable = Enum.Parse(typeDependencyStatus, "Available");
            if (status == fieldDependencyStatusAvailable) {
                return true;
            }
            Debug.LogError($"Could not resolve all Firebase dependencies");
            return false;
        }
    }
}