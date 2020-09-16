using System;

namespace EE {
    public interface IDispatcher {
        void Dispatch(Action action);
    }
}