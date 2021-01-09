using System;
using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public static class TaskExtensions {
        public static async Task<T> Then<T, U>(this Task<U> task, Func<U, T> func) {
            return func(await task);
        }

        public static async Task Then<T>(this Task<T> task, Action<T> action) {
            action(await task);
        }

        public static async Task Then(this Task task, Action action) {
            await task;
            action();
        }

        /// <summary>
        /// http://stackoverflow.com/questions/22629951/suppressing-warning-cs4014-because-this-call-is-not-awaited-execution-of-the
        /// </summary>
        public static void Forget(this Task task, Action<Exception> callback = null) {
            task.ContinueWith(t => {
                Debug.LogError(t.Exception?.StackTrace);
                callback?.Invoke(t.Exception);
            }, TaskContinuationOptions.OnlyOnFaulted);
        }
    }
}