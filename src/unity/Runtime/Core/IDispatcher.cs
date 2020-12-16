using System;
using System.Collections;

namespace EE {
    public interface IDispatcher {
        void Dispatch(Action action);
        void Dispatch(IEnumerator coroutine);
    }
}