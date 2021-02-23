namespace EE.Internal {
    public interface ICapper {
        bool IsCapped { get; }
        void Cap();
    }
}