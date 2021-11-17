using System;
using System.Threading.Tasks;

using UnityEngine;

namespace EE {
    public static class Utils {
        public static string ToString(bool value) {
            return value ? "true" : "false";
        }

        public static bool ToBool(string value) {
            return value == "true";
        }

        public static async void NoAwait(Func<Task> callable) {
            try {
                var task = callable();
                await task;
            } catch (Exception ex) {
                Debug.LogException(ex);
            }

            // Old implementation.
            // var task = callable();
            // task.Forget();
        }

        public static async void NoAwait(Func<Task> callable, CancellationToken cancelToken) {
            try {
                var taskCompletionSource = new TaskCompletionSource<bool>();
                cancelToken.Register(() => taskCompletionSource.TrySetCanceled());
                
                var mainTask = callable();
                var completedTask = await Task.WhenAny(mainTask, taskCompletionSource.Task);
                if (completedTask == mainTask) {
                    taskCompletionSource.TrySetResult(true);
                }
            } catch (Exception ex) {
                Debug.LogException(ex);
            }
        }
    }
}