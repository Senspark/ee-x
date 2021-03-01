using System;
using System.Collections;
using System.Runtime.CompilerServices;
using System.Runtime.ExceptionServices;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE {
    public static class IEnumeratorAwaitExtensions {
        public class SimpleCoroutineAwaiter<T> : INotifyCompletion {
            private Exception _exception;
            private Action _continuation;
            private T _result;

            public bool IsCompleted { get; private set; }

            public T GetResult() {
                Assert.IsTrue(IsCompleted);
                if (_exception != null) {
                    ExceptionDispatchInfo.Capture(_exception).Throw();
                }
                return _result;
            }

            public void Complete(T result, Exception ex) {
                Assert.IsFalse(IsCompleted);
                IsCompleted = true;
                _exception = ex;
                _result = result;
                if (_continuation != null) {
                    Thread.RunOnLibraryThread(_continuation);
                }
            }

            public void OnCompleted(Action continuation) {
                Assert.IsNull(_continuation);
                Assert.IsFalse(IsCompleted);
                _continuation = continuation;
            }
        }

        private static class InstructionWrappers {
            public static IEnumerator ReturnSelf<T>(SimpleCoroutineAwaiter<T> awaiter, T instruction) {
                yield return instruction;
                awaiter.Complete(instruction, null);
            }

            public static IEnumerator ResourceRequest(
                SimpleCoroutineAwaiter<UnityEngine.Object> awaiter, ResourceRequest instruction) {
                yield return instruction;
                awaiter.Complete(instruction.asset, null);
            }
        }

        public static SimpleCoroutineAwaiter<AsyncOperation> GetAwaiter(this AsyncOperation instruction) {
            return GetAwaiterReturnSelf(instruction);
        }

        public static SimpleCoroutineAwaiter<UnityEngine.Object> GetAwaiter(this ResourceRequest instruction) {
            var awaiter = new SimpleCoroutineAwaiter<UnityEngine.Object>();
            Thread.RunOnLibraryThread(() => Thread.Dispatch(InstructionWrappers.ResourceRequest(awaiter, instruction)));
            return awaiter;
        }

        private static SimpleCoroutineAwaiter<T> GetAwaiterReturnSelf<T>(T instruction) {
            var awaiter = new SimpleCoroutineAwaiter<T>();
            Thread.RunOnLibraryThread(() => Thread.Dispatch(InstructionWrappers.ReturnSelf(awaiter, instruction)));
            return awaiter;
        }
    }
}