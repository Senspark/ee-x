namespace EE {
    public enum AdjustLogLevel {
        /// Enables all logging.
        Verbose,

        /// Enables more logging.
        Debug,

        /// Default setting.
        Info,

        /// Disables info logging.
        Warn,

        /// Disables warnings as well.
        Error,

        /// Disables errors as well.
        Assert,

        /// Disables all log ouput.
        Suppress,
    }
}