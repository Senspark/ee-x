using System;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    internal static class FirebaseManager {
        private static Task<bool> _initializer;

        public static Task<bool> Initialize() => _initializer = _initializer ?? (_initializer = InitializeImpl());

        private static async Task<bool> InitializeImpl() {
            Debug.Log($"FirebaseManager: InitializeImpl");

            // https://firebase.google.com/docs/unity/setup
            var typeFirebaseApp = Type.GetType("Firebase.FirebaseApp, Firebase.App");
            if (typeFirebaseApp == null) {
                throw new ArgumentException("Cannot find Firebase.FirebaseApp");
            }
            var methodCheckAndFixDependenciesAsync =
                typeFirebaseApp.GetMethod("CheckAndFixDependenciesAsync");
            Assert.IsNotNull(methodCheckAndFixDependenciesAsync);

            var typeDependencyStatus = Type.GetType("Firebase.DependencyStatus, Firebase.App");
            Assert.IsNotNull(typeDependencyStatus);

            var task = (Task) methodCheckAndFixDependenciesAsync.Invoke(null, null);
            await task.ConfigureAwait(false);
            var status = task.GetType().GetProperty("Result")?.GetValue(task);
            Debug.Log($"FirebaseManager: CheckAndFixDependenciesAsync result = {status}");

            var fieldDependencyStatusAvailable = Enum.Parse(typeDependencyStatus, "Available");
            if (fieldDependencyStatusAvailable.Equals(status)) {
                var methodDefaultInstance = typeFirebaseApp.GetProperty("DefaultInstance");
                var app = methodDefaultInstance?.GetValue(null);
                return true;
            }
            Debug.LogError($"Could not resolve all Firebase dependencies");
            return false;
        }
    }
}