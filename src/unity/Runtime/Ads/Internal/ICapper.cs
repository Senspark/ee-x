namespace EE.Internal {
    internal interface ICapper {
        bool IsCapped { get; }
        void Cap();
        void Lock();
        void Unlock();
    }
}