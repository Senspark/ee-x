using System;

namespace EE {
    [AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
    public class AnalyticsParameterAttribute : Attribute {
        public string Name { get; }

        public AnalyticsParameterAttribute(string name = null) {
            Name = name;
        }
    }
}