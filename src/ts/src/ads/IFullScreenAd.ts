import { IAd } from "./IAd";

export enum AdResult {
    /// Used by DefaultFullScreenAd.
    Failed,
    Canceled,
    Completed,

    /// Used by LazyFullScreenAd.
    NotInitialized,
    NotConfigured,

    /// Used by GenericAd.
    Capped,
    NoInternet,
    NotLoaded,
}

export interface IFullScreenAd extends IAd {
    show(): Promise<AdResult>;
}