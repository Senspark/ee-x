using System;
using System.Collections;
using System.Collections.Generic;

using UnityEngine;

namespace EE.Internal {
    internal class MainThreadDispatcher : MonoBehaviour, IDispatcher {
        private readonly Queue<Action> _actions = new Queue<Action>();

        public void Dispatch(Action action) {
            lock (_actions) {
                _actions.Enqueue(() => { //
                    StartCoroutine(ActionWrapper(action));
                });
            }
        }

        public void Dispatch(IEnumerator coroutine) {
            StartCoroutine(coroutine);
        }

        private static IEnumerator ActionWrapper(Action action) {
            action();
            yield return null;
        }

        private void Update() {
            lock (_actions) {
                while (_actions.Count > 0) {
                    var action = _actions.Dequeue();
                    action.Invoke();
                }
            }
        }
    }
}