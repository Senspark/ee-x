namespace EE.Internal {
    internal interface IJsonConverter {
        bool Initialize();
        T Deserialize<T>(string value);
        string Serialize<T>(T value);
    }
}